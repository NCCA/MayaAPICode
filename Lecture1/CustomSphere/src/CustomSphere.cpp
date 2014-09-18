#include "CustomSphere.h"
#include <boost/format.hpp>

//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------
#define CHECK_STATUS_AND_RETURN_IF_FAIL( status , message )				\
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

MStatus CustomSphere::doIt( const MArgList& _args )
{
	MStatus	status;

	// Verify argument count
  if ( _args.length() != 1 )
	{
		MGlobal::displayError( "Command requires one argument" );
		return MStatus::kFailure;
	}

	// Check aurgument type
  m_count = _args.asInt( 0, &status );
	if( !status )
	{
		MGlobal::displayError( "argument is not an integer" );
		return MStatus::kFailure;
	}

	// Check argument range
	if( m_count <= 0 )
	{
		MGlobal::displayError( "argument must be greater than zero" );
		return MStatus::kFailure;
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
    float rad=randFloat(0.8,4.5);
    cmd=boost::str(boost::format("sphere -name \"sphere%d\" -r %f") %i %rad)  ;
    // now execute the command
    MStatus status = MGlobal::executeCommand( cmd.c_str() );
    // and check that is was succesfull
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"Unable to execute sphere command");

    // now move to a random position first grab some positions
    float x=randFloat(-20,20);
    float y=randFloat(-20,20);
    float z=randFloat(-20,20);
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

