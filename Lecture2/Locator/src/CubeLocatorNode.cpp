#include "CubeLocatorNode.h"


//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------
#define CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( stat , message )				\
	if( !status )										\
	{											\
		MString errorString = status.errorString() + " -- " + MString( message );	\
		MGlobal::displayError( errorString );						\
		return MStatus::kFailure;							\
	}											\
//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------

#define CHECK_STATUS_AND_RETURN_IF_FAIL( stat , message )					\
	if( !status )										\
	{											\
		MString errorString = status.errorString() + " -- " + MString( message );	\
		MGlobal::displayError( errorString );						\
	}											\

/// @brief macro to get rid of compiler warnings
#define UNUSED(arg) (void)arg;

//----------------------------------------------------------------------------------------------------------------------
// to enable the static methods in this class to see these we bring them into
// the file scope here
MObject CubeLocatorNode::m_width;
MObject CubeLocatorNode::m_height;
MObject CubeLocatorNode::m_depth;
MObject CubeLocatorNode::m_volume;
// as these are static initialisers we need to set them here.
MTypeId CubeLocatorNode::m_id( 0x70000 );		// numeric Id of node
const MString CubeLocatorNode::m_typeName( "CubeLocatorNode" );

//----------------------------------------------------------------------------------------------------------------------


MStatus	CubeLocatorNode::initialize()
{
  MStatus status;
	MFnNumericAttribute	numAttr;
	
  // Create a width attribute this will be placed in the maya panel as an "Extra Attribute"
  m_width = numAttr.create( "width", "w", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"width\" attribute" );
	numAttr.setChannelBox( true );			// length attribute appears in channel box

  // Add width attribute to node
  status = addAttribute( m_width );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"width\" attribute to CubeLocatorNode" );

  // repeat for the other attributes
  m_height = numAttr.create( "height", "h", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"height\" attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box

  status = addAttribute( m_height );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"height\" attribute to CubeLocatorNode" );

  m_depth = numAttr.create( "depth", "d", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"depth\" attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box

  status = addAttribute( m_depth );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"depth\" attribute to CubeLocatorNode" );


  // Create an volume attribute
  m_volume = numAttr.create( "volume", "vol", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"volume\" attribute" );
  numAttr.setWritable(false);
  numAttr.setKeyable(false);
  numAttr.setConnectable(false);
  numAttr.setChannelBox( true );			// volume attribute appears in channel box

  // Add length attribute to node
  status = addAttribute( m_volume );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"volume\" to CubeLocatorNode attribute" );

  // Setup "attribute affects" relationship this will call the compute method for the 2nd attribute
  // parameters are const MObject & whenChanges, const MObject & isAffected
  // in this case when any of the w/h/d params are changed we re-calculate the volume.
  attributeAffects( m_width , m_volume );
  attributeAffects( m_height , m_volume );
  attributeAffects( m_depth , m_volume );

	return MStatus::kSuccess;
}

// This method should be overridden in user defined nodes.
// Recompute the given output based on the nodes inputs.
// The plug represents the data value that needs to be recomputed, and the data block holds the storage
// for all of the node's attributes.

MStatus CubeLocatorNode::compute( const MPlug &_plug , MDataBlock &_data )
{
  MStatus status;
  double volume;
  // if the w/h/d params are changed we will get the plug m_volume passed to us
  if( _plug == m_volume)
	{
		// get data handle for length
    MDataHandle dataHandle = _data.inputValue( m_width , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for width plug" );
    // now get the value for the data handle as a double
    volume= dataHandle.asDouble();
    dataHandle = _data.inputValue( m_height , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for height plug" );
    volume*=dataHandle.asDouble();
    dataHandle = _data.inputValue( m_depth , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for depth plug" );
    volume*=dataHandle.asDouble();
    // now set
    // get data handle for the volume (note this is now an output value)
    MDataHandle volumeDataHandle = _data.outputValue( _plug , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for volume plug" );
    // set the value
    volumeDataHandle.setDouble( volume );

    // clean the output plug, ie unset it from dirty so that maya does not re-evaluate it
    _data.setClean( _plug );
    return MStatus::kSuccess;
	}

  return MStatus::kUnknownParameter;
}


//----------------------------------------------------------------------------------------------------------------------
void CubeLocatorNode::draw(
                            M3dView & _view,
                            const MDagPath & _path,
                            M3dView::DisplayStyle _style ,
                            M3dView::DisplayStatus _displayStatus
                          )
{
  // remove unused param warnings
  UNUSED(_path);
  UNUSED(_style);
  UNUSED(_displayStatus);
  // these values will be used to obtain the plug parameters
  double width;
  double height;
  double depth;
  // error status checking
	MStatus status;
  // first grab the dependency node so we can query the plugs
	MFnDependencyNode dependencyFn( thisMObject() , &status );
	CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable to initialize dependency node" );
  // grab the width plug
  MPlug sizePlug = dependencyFn.findPlug( m_width , true , &status );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get width plug" );
  // now grab the value and place in our variable
  status = sizePlug.getValue( width );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get value from width plug" );
  // repeat for the rest
  sizePlug = dependencyFn.findPlug( m_depth , true , &status );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get depth plug" );

  status = sizePlug.getValue( depth );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get depth from width plug" );

  sizePlug = dependencyFn.findPlug( m_height , true , &status );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get depth plug" );

  status = sizePlug.getValue( height );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get  value from depth plug" );


	// Need a refresher?, see chapter 2 of the "Red Book" -- http://www.glprogramming.com/red/chapter02.html
  // this will beging our openGL rendering we are using immediate mode for simplicity
  // we could use more modern GL but for now this will do
  _view.beginGL();
    // we are going to draw our cube by two squares for top and bottom then lines for the
   // side edges
		// Top face
		glBegin( GL_LINE_LOOP );
      glVertex3d(  width , height ,  depth );
      glVertex3d( -width , height ,  depth );
      glVertex3d( -width , height , -depth );
      glVertex3d(  width , height , -depth );
		glEnd();

		// Bottom face
		glBegin( GL_LINE_LOOP );
      glVertex3d(  width , -height ,  depth );
      glVertex3d( -width , -height ,  depth );
      glVertex3d( -width , -height , -depth );
      glVertex3d(  width , -height , -depth );
		glEnd();

		// Sides
		glBegin( GL_LINES );
      glVertex3d(  width ,  height ,  depth );
      glVertex3d(  width , -height ,  depth );

      glVertex3d( -width ,  height ,  depth );
      glVertex3d( -width , -height ,  depth );

      glVertex3d( -width ,  height , -depth );
      glVertex3d( -width , -height , -depth );

      glVertex3d(  width ,  height , -depth );
      glVertex3d(  width , -height , -depth );
		glEnd();

  _view.endGL();

}

//----------------------------------------------------------------------------------------------------------------------
void* CubeLocatorNode::creator()
{
	return new CubeLocatorNode();
}
//----------------------------------------------------------------------------------------------------------------------


