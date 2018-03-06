#include <iomanip>
#include <string>

#include "RibExport.h"
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMatrixData.h>
#include <maya/MIOStream.h>
#include <maya/MItDag.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MComputation.h>
#include <maya/MAnimControl.h>
#include <maya/MPoint.h>
#include <maya/MDataHandle.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MSyntax.h>
#include <maya/MFnMesh.h>
#include <maya/MArgDatabase.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnTransform.h>
#include <maya/MCommonRenderSettingsData.h>
#include <maya/MFnCamera.h>
#include <maya/MFnLight.h>

#include <maya/MItMeshPolygon.h>
#include <boost/format.hpp>
#include <maya/MFnMeshData.h>
#include <maya/MObject.h>

//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aino
//----------------------------------------------------------------------------------------------------------------------
#define CHECK_STATUS_AND_RETURN_IF_FAIL( stat , message )				\
  if( !status )									\
  {										\
    MString errorString = status.errorString() +  MString( message);	\
    MGlobal::displayError( errorString );					\
    return MStatus::kFailure;						\
  }										\

//----------------------------------------------------------------------------------------------------------------------

const char * g_HelpText="Usage : RibExport -f [name of rib file] -c camera to use\n";

MStatus		RibExport::writer( const MFileObject& _file,
                            const MString& _optionsString,
                            FileAccessMode _mode
                            )
{

  // if were in export selected mode check to see if item is selected
  if(_mode == kExportActiveAccessMode)
  {
    MGlobal::displayError("export Selection not yet implemented");
    return MS::kFailure;
  }


  m_exportMesh=true;
  m_exportLights=true;
  m_exportNormals=true;
  m_exportUV=true;
  MGlobal::displayInfo(_optionsString);
  parseCommandOptions(_optionsString);

  MObject node;
  MFnDagNode nodeFn;
  // lets process the command line arguments
  MStatus status;

  getRenderGlobals();
  std::string msg;
  // create an MComputation class for heavy calculation type things
   MComputation Interupter;
  for(uint i=m_startFrame; i<=m_endFrame; i+=m_frameStep)
  {
  // set the start of the heavy computation
   Interupter.beginComputation();
   MTime currframe;
   MAnimControl anim;
   currframe.setValue(i);
   anim.setCurrentTime(currframe);
   // msg=boost::str(boost::format("Exporting frame %0d") % boost::io::group( std::setw(m_framePad), i) )  ;
   // MGlobal::displayInfo(msg.c_str());

   // msg=boost::str(boost::format("%s.%0d.rib") %_file.expandedFullName() % boost::io::group( std::setw(m_framePad), i));
    m_stream.open("/tmp/1.rib");
    if(!m_stream.is_open())
    {
      MGlobal::displayError("error opening file ");
      return MS::kFailure;
    }
    else
    {
      m_stream<<"# rib export Maya Script \n";
    }


    // Display "Camera.exr" "f" "rgba"
    msg=boost::str(boost::format("Display \"%s.%0d.exr\" \"file\" \"rgba\" \n" ) %m_imageName % boost::io::group( std::setw(m_framePad), i) );
    m_stream<<msg.c_str();
    // now the format
    m_stream<<"Format "<<m_imageWidth<<" "<<m_imageHeight<<" "<<m_pixelAspectRatio<<"\n";

    // now the camera
    exportCamera();

    // now we need to start the World and do the camera transform
    m_stream<<"WorldBegin \n";

    if(m_exportLights)
    {
      MItDependencyNodes lightIt(MFn::kLight);
      // keep looping until done
      while(!lightIt.isDone())
      {
      // get a handle to this node
      MObject obj( lightIt.item());
      exportLight(obj);
      // move on to next node
      lightIt.next();
      }
    }

    if(m_exportMesh)
    {
      MItDependencyNodes meshIt(MFn::kMesh);
      // keep looping until done
      while(!meshIt.isDone())
      {

      // get a handle to this node
      MObject obj = meshIt.item();

        exportMesh(obj);
      // move on to next node
      meshIt.next();

      }
    }
    m_stream<<"WorldEnd\n";
    m_stream.close();
    if( Interupter.isInterruptRequested() )
    {
      Interupter.endComputation();
      break;
    }

  } // end of frame



  return MS::kSuccess;
}


RibExport::~RibExport()
{

}


//----------------------------------------------------------------------------------------------------------------------
MStatus RibExport::exportCamera()
{

  MItDependencyNodes it(MFn::kCamera);
  MFnDagNode nodeFn;
  MStatus status;
  // keep looping until done
  while(!it.isDone())
  {

  // get a handle to this node
  MFnCamera cam ( it.item());
  // the active renderable camera is not stored in the render globals but in the
  // camera itself so we get the value here and output if set
  bool renderable;
  cam.findPlug( "renderable" ).getValue( renderable );

  if(renderable)
  {

    m_stream<<"Projection \"perspective\" \"uniform float fov\" [";
    // using the following calculation
    // http://drakeguan.org/node/393
    // FOV = rad_to_deg(2*atan((xres/yres)*12.7/cam.focalLength))
    double fl=cam.focalLength();
    double fov= 2*atan((m_imageWidth/m_imageHeight)*12.7/fl);
    m_stream<<(fov/ M_PI) * 180.0f;
    m_stream<<"]\n";

    MDagPath path;

    // getAllPaths() would be better here (to support instancing).
    // IIRC, cameras get very screwy when instanced, so don't worry too much
    status = cam.getPath(path);
    CHECK_STATUS_AND_RETURN_IF_FAIL(status," error getting path ");

    // if you want the world space (or inverse) matrices it's always better to grab those from the
    // dag path directly. (It avoids the annoyance of having to use the MFnMatrixData class).
    // inclusiveMatrix() is the same as the world matrix.....  If you have the path, everything preceding
    // this bit can be ignored ;)
    MMatrix tx = path.inclusiveMatrixInverse(&status);
    CHECK_STATUS_AND_RETURN_IF_FAIL(status," error getting inverse matrix ");

    // now export first set the identity matrix
    m_stream<<"Identity \n";
    // flip the z for the camera
    m_stream<<"# camera tx \n";
    // now flip the Z for prman (could do this in the transform but this is easier!)
    m_stream<<"Scale 1 1 -1 \n";

    // now the tx matrix
    m_stream<<"ConcatTransform [ ";
    for(int y=0; y<4; ++y)
      for(int x=0; x<4; ++x)
        m_stream<<" "<<tx(y,x)<<" ";
    m_stream<<" ]\n";
    // as we only want one camera we can return from this function now
    return MS::kSuccess;
  }
  // move on to next node
  it.next();

  }
  return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
void RibExport::exportLight(MObject _node)
{
  MGlobal::displayError("you need to implement this!");
}

//----------------------------------------------------------------------------------------------------------------------
MStatus RibExport::exportMesh(MObject _node)
{

  MStatus status;
  // attach the function set to the object
  MFnMesh mesh(_node);
  m_stream<<"TransformBegin\n";
  m_stream<<"Identity \n";

  MDagPath path;

  status = mesh.getPath(path);
  CHECK_STATUS_AND_RETURN_IF_FAIL(status," error getting path ");

  // if you want the world space (or inverse) matrices it's always better to grab those from the
  // dag path directly. (It avoids the annoyance of having to use the MmeshMatrixData class).
  // inclusiveMatrix() is the same as the world matrix.....  If you have the path, everything preceding
  // this bit can be ignored ;)
  MMatrix tx = path.inclusiveMatrix(&status);
  CHECK_STATUS_AND_RETURN_IF_FAIL(status," error getting inverse matrix ");

  m_stream<<"# exporting maya mesh "<<mesh.name()<<"\n";
  // now the tx matrix
  m_stream<<"ConcatTransform [ ";
  for(int y=0; y<4; ++y)
    for(int x=0; x<4; ++x)
      m_stream<<" "<<tx(y,x)<<" ";
  m_stream<<" ]\n";


  // this will hold the returned vertex positions
  MPointArray vts;
  // use the function set to get the points
  mesh.getPoints(vts);
  // now the normals
  MFloatVectorArray nmls;
  // use the function set to get the points
  mesh.getNormals(nmls);

  MStringArray uvsets;
  // get the names of the uv sets on the mesh
  mesh.getUVSetNames(uvsets);
  MStringArray sets;
  // get the names of the uv sets on the mesh
  mesh.getUVSetNames(sets);

  // set a usage flag for texture coords
  bool bUvs = ((sets.length()!=0) && (mesh.numUVs()>0));
  MFloatArray uArray;
  MFloatArray vArray;
  mesh.getUVs(uArray,vArray);

  // attach an iterator to the mesh
  MItMeshPolygon itPoly(_node);

  // now loop through the mesh and export
  while(!itPoly.isDone())
  {
// Polygon "varying float[2] st" [] "vertex point P" [] "varying normal N" []
    auto vc = itPoly.polygonVertexCount();
    // in this case we are going to do normal prman polygons and have options to export uv / normals
    m_stream<<"Polygon ";

    m_stream<<"\"vertex point P\" [";
    for(auto i=0; i<vc; ++i)
    {
      uint index=itPoly.vertexIndex(i);
      m_stream<<vts[index][0]<<" "<<vts[index][1]<<" "<<vts[index][2]<<" ";
    }
    m_stream<<"] ";
    if(m_exportNormals)
    {
      m_stream<<  "\"varying normal N\" [";
      for(auto i=0; i<vc; ++i)
      {
        uint index=itPoly.normalIndex(i);
        m_stream<<nmls[index][0]<<" "<<nmls[index][1]<<" "<<nmls[index][2]<<" ";
      }
      m_stream<<"] ";
    }
    if(bUvs && m_exportUV)
    {
      m_stream<<"\"varying float[2] st\" [";
    // have to get the uv index seperately
      for(auto i=0; i<vc; ++i)
      {
        int index;
        itPoly.getUVIndex(i,index);
        m_stream<<uArray[index]<<" "<<vArray[index]<<" ";
      }
      m_stream<<"] ";

    }

    m_stream<<"\n";
    itPoly.next();
  }

  m_stream<<"TransformEnd\n";
  return MS::kSuccess;
}

void RibExport::getRenderGlobals()
{
  MCommonRenderSettingsData data;
  MRenderUtil::getCommonRenderSettings(data);
  m_startFrame=int(data.frameStart.value());
  m_endFrame=int(data.frameEnd.value());
  m_frameStep=int(data.frameBy);
  m_imageName=data.name;

  if (m_imageName.length() ==0)
  {
    MGlobal::displayWarning("No filename set in render globals, using default");
    m_imageName="noNameSet";
  }
  m_pixelAspectRatio=data.pixelAspectRatio;
  m_imageWidth=data.width;
  m_imageHeight=data.height;
  m_framePad=data.framePadding;

}


void RibExport::parseCommandOptions(const MString& _optionsString)
{

  //	process the option string passed in from the option script
  //
  if (_optionsString.length() > 0)
  {

    int i, length;
    // Start parsing.
    MStringArray optionList;
    MStringArray theOption;

    //	each option is in the form -
    //	[Option] = [Value];
    //
    //	the options are therefore split by the semi-colons
    //
    _optionsString.split(';', optionList);

    //	process each option in the string
    //
    length = optionList.length();
    for( i = 0; i < length; ++i )
    {
      theOption.clear();

      //	split the option using the '=' character. This should make
      //	theOption[0] hold the option name, and theOption[1] hold the
      //	value of the current option
      //
      optionList[i].split( '=', theOption );
      MGlobal::displayInfo(theOption[0]);
      if( theOption[0] == "-normals" && theOption.length() > 1 )
      {
        if(theOption[1].asInt()>0)
          m_exportNormals = true;
        else
          m_exportNormals = false;
      }

      else if( theOption[0] == "-uvs" && theOption.length() > 1 )
      {
        if(theOption[1].asInt()>0)
          m_exportUV = true;
        else
          m_exportUV = false;
      }
      else if( theOption[0] == "-geo" && theOption.length() > 1 )
      {
        if(theOption[1].asInt()>0)
          m_exportMesh= true;
        else
          m_exportMesh = false;
      }

      else if( theOption[0] == "-lights" && theOption.length() > 1 )
      {
        if(theOption[1].asInt()>0)
          m_exportLights= true;
        else
          m_exportLights = false;
      }

     }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void* RibExport::creator()
{
  return new RibExport();
}


//----------------------------------------------------------------------------------------------------------------------
bool  RibExport::haveReadMethod	 () const
{
  return false;
}


//----------------------------------------------------------------------------------------------------------------------
bool RibExport::haveWriteMethod	 () const
{
  return true;
}


//----------------------------------------------------------------------------------------------------------------------
MString RibExport::defaultExtension () const
{
  return "";
}

