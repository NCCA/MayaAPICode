#include "QtGui.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
  MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  status = plugin.registerCommand( "QtGuiCPP", QtGui::creator );
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

  status = plugin.deregisterCommand( "QtGuiCPP" );
	if ( !status )
	{
    status.perror( "Unable to register command \"customCreateSphere\"" );
		return status;
	}

	return status;
}

//----------------------------------------------------------------------------------------------------------------------

