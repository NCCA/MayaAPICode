#include "JoyPadNode.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  // register our nodes and the commands which will be called
  status = plugin.registerNode( "JoyPadNode", JoyPadNode::m_id, &JoyPadNode::creator, &JoyPadNode::initialize, MPxNode::kThreadedDeviceNode );
	if (!status)
	{
		status.perror("Unable to register JoyPadNode" );
		return status;
	}


	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );


	status = plugin.deregisterNode( JoyPadNode::m_id );
	if (!status)
	{
		status.perror( "unable to deregister JoyPadNode" );
		return status;
	}

	return status;
}
//----------------------------------------------------------------------------------------------------------------------


