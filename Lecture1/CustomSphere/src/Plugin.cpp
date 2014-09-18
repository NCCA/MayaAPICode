#include "CustomSphere.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  status = plugin.registerCommand( "customCreateSphere", CustomSphere::creator );
	if ( !status )
	{
    status.perror( "Unable to register command \"customCreateSphere\"" );
		return status;
	}

	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

  status = plugin.deregisterCommand( "customCreateSphere" );
	if ( !status )
	{
    status.perror( "Unable to register command \"customCreateSphere\"" );
		return status;
	}

	return status;
}

//----------------------------------------------------------------------------------------------------------------------

