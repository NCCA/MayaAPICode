#include "CustomSphere.h"
#include <boost/format.hpp>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

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



CustomSphere::~CustomSphere()
{
  // as we have no dynamic allocation we don't need to do anything here
}

//----------------------------------------------------------------------------------------------------------------------
MSyntax CustomSphere::newSyntax()
{

  // The syntax object we will be returning
  MSyntax syn;
  // these could be stored as static data instead to save repetition
  syn.addFlag("-h","-help");
  syn.addFlag("-x","-xExtents", MSyntax::kDouble );
  syn.addFlag("-y","-yExtents", MSyntax::kDouble );
  syn.addFlag("-z","-zExtents", MSyntax::kDouble );

  syn.addFlag("-n","-number", MSyntax::kUnsigned );
  syn.addFlag("-mr","-minRadius", MSyntax::kDouble );
  syn.addFlag("-mm","-maxRadius", MSyntax::kDouble );


  // set the currently selected items
  return syn;
 }

const char * g_HelpText="Usage : customCreateSphere \n-h this help\n-x/y/z set the extents\n-n number of spheres\n-mr min radius\n-mm max radius\n";

//----------------------------------------------------------------------------------------------------------------------

MStatus CustomSphere::doIt( const MArgList& _args )
{
  // set some default values
  m_count=100;
  m_xExtent=20;
  m_yExtent=20;
  m_zExtent=20;
  m_minRadius=0.2;
  m_maxRadius=2.5;


  // lets process the command line arguments
  MStatus status;
  MString arg;
  // Create a parser for our arguments using the syntax for this command,
  // and the args passed to the function by the user
  MArgDatabase parser(syntax(),_args,&status);

  if(status != MS::kSuccess)
  {
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"error parsing arguments");
  }
  // if the help flag is set, display some useful info.
  if(parser.isFlagSet("-h"))
  {
    MGlobal::displayInfo(g_HelpText);
    return MS::kSuccess;
  }

  //
  // get the command line arguments that were specified
  //
  if(parser.isFlagSet("-n"))
  {
    parser.getFlagArgument( "-n", 0, m_count );
  }
  if(parser.isFlagSet("-mr"))
  {
    parser.getFlagArgument( "-mr", 0, m_minRadius );
  }
  if(parser.isFlagSet("-mm"))
  {
    parser.getFlagArgument( "-mm", 0, m_maxRadius );
  }
  if(parser.isFlagSet("-x"))
  {
    parser.getFlagArgument( "-x", 0, m_xExtent );
  }
  if(parser.isFlagSet("-y"))
  {
    parser.getFlagArgument( "-y", 0, m_yExtent );
  }
  if(parser.isFlagSet("-z"))
  {
    parser.getFlagArgument( "-z", 0, m_zExtent );
  }

  // now call the redoIt method which actually does the work
	return redoIt();
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief a simple random number in range generator
/// @param _min the min value
/// @param _max the max value
//----------------------------------------------------------------------------------------------------------------------
float randFloat(float _min, float _max)
{
return ((_max-_min)*((float)rand()/RAND_MAX))+_min;
}


//----------------------------------------------------------------------------------------------------------------------
MStatus CustomSphere::redoIt()
{
  // loop for the number of arguments passed in and create some random spheres
  for( unsigned int i = 0; i < m_count; ++i )
	{
    // fist I'm going to create a maya command as follows
    // sphere -name "sphere[n]" where n is the value of i
    std::string cmd;
    float rad=randFloat(m_minRadius,m_maxRadius);
    cmd=boost::str(boost::format("sphere -name \"sphere%d\" -r %f") %i %rad)  ;
    // now execute the command
    MStatus status = MGlobal::executeCommand( cmd.c_str() );
    // and check that is was succesfull
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"Unable to execute sphere command");

    // now move to a random position first grab some positions
    float x=randFloat(-m_xExtent,m_xExtent);
    float y=randFloat(-m_yExtent,m_yExtent);
    float z=randFloat(-m_zExtent,m_zExtent);
    // build the command string
    // move x y z "sphere[n]"
    cmd=boost::str(boost::format("move %f %f %f \"sphere%d\"") %x %y %z %i)  ;
    // execute
    status=MGlobal::executeCommand(cmd.c_str());
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"unable to move object");

	}
  std::string mesg=boost::str(boost::format("%d Spheres created") %m_count)  ;
  MGlobal::displayInfo( mesg.c_str() );
	return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus CustomSphere::undoIt()
{
  // here we undo what was done in the re-do method,
  // this will be called when maya calles the undo method
  for( unsigned int i = 0; i < m_count; ++i )
  {
    std::string cmd;
    // delete the objects as created previously
    cmd=boost::str(boost::format("delete  \"sphere%d\"") %i)  ;
    MStatus status=MGlobal::executeCommand(cmd.c_str());
    // check that is was ok
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"unable to delete objects in undo");

  }
	return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------

bool CustomSphere::isUndoable() const
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void* CustomSphere::creator()
{
  return new CustomSphere();
}

//----------------------------------------------------------------------------------------------------------------------

