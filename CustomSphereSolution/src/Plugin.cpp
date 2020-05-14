#include "CustomSphere.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );
  // as we are using a syntax parser we need to register this as well, see
  // http://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/maya/MSyntax.html for more details
  status = plugin.registerCommand( "customCreateSphere", CustomSphere::creator,CustomSphere::newSyntax );
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

