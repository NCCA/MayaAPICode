#include "CustomSphere.h"
#include <maya/MString.h>
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

	// Check argument type
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
  static const MString create("sphere -name \"sphere^1s\" -r ^2s");
  static const MString move("move ^1s ^2s ^3s \"sphere^4s\"");

  MString cmd,index,radius,x,y,z;
  // loop for the number of arguments passed in and create some random spheres
  for(  int i = 0; i < m_count; ++i )
	{
    // fist I'm going to create a maya command as follows
    // sphere -name "sphere[n]" where n is the value of i
    // and this is why I hate MString!
    radius.set(randFloat(0.8f,4.5f));
    index.set(i);
    cmd.format(create, index, radius);
    // now execute the command
    MStatus status = MGlobal::executeCommand( cmd );
    // and check that is was successful
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"Unable to execute sphere command");

    // now move to a random position first grab some positions
    x.set(randFloat(-20,20));
    y.set(randFloat(-20,20));
    z.set(randFloat(-20,20));
    // build the command string
    // move x y z "sphere[n]"
    cmd.format(move, x, y, z, index);
    // execute
    status=MGlobal::executeCommand(cmd);
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"unable to move object");

	}
  MString mesg,count;
  count.set(m_count);
  mesg.format("Created ^1s spheres", count) ;
  MGlobal::displayInfo( mesg );
	return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus CustomSphere::undoIt()
{
  // here we undo what was done in the re-do method,
  // this will be called when maya calls the undo method
  MString cmd,index;

  for(  int i = 0; i < m_count; ++i )
  {
    index.set(i);
    // delete the objects as created previously
    cmd.format("delete  \"sphere^1s\"", index) ;
    MStatus status=MGlobal::executeCommand(cmd);
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

