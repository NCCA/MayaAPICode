#include "CustomSphere.h"
#include <maya/MString.h>
#include <random>

#define CHECK_STATUS_AND_RETURN_IF_FAIL( stat , message )				\
  if( !status )									\
  {										\
    MString errorString = status.errorString() +  MString( message);	\
    MGlobal::displayError( errorString );					\
    return MStatus::kFailure;						\
  }

std::mt19937 g_RandomEngine;

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
MStatus CustomSphere::redoIt()
{
  const MString create("sphere -name \"sphere^1s\" -r ^2s");
  const MString move("move ^1s ^2s ^3s \"sphere^4s\"");

  MString cmd,index,radius,x,y,z;
  std::uniform_real_distribution<float>radiusDist(0.8f,4.5f);
  std::uniform_real_distribution<float>positionDist(-20,20);
  // loop for the number of arguments passed in and create some random spheres
  for(  int i = 0; i < m_count; ++i )
	{
    // fist I'm going to create a maya command as follows
    // sphere -name "sphere[n]" where n is the value of i
    // and this is why I hate MString!
    radius.set(radiusDist(g_RandomEngine));
    index.set(i);
    cmd.format(create, index, radius);
    // now execute the command
    MStatus status = MGlobal::executeCommand( cmd );
    // and check that is was successful
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"Unable to execute sphere command");

    // now move to a random position first grab some positions
    x.set(positionDist(g_RandomEngine));
    y.set(positionDist(g_RandomEngine));
    z.set(positionDist(g_RandomEngine));
    // build the command string
    // move x y z "sphere[n]"
    cmd.format(move, x, y, z, index);
    // execute
    status=MGlobal::executeCommand(cmd);
    CHECK_STATUS_AND_RETURN_IF_FAIL(status,"unable to move object");

	}
  MString message,count;
  count.set(m_count);
  message.format("Created ^1s spheres", count) ;
  MGlobal::displayInfo( message );
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

