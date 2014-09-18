#include "TestNode.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  // register our nodes and the commands which will be called
  status = plugin.registerNode( "TestNode", TestNode::m_id, &TestNode::creator, &TestNode::initialize, MPxNode::kDependNode );
	if (!status)
	{
    status.perror("Unable to register TestNode" );
		return status;
	}

  MGlobal::displayInfo("initialize plugin called");
	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );


  status = plugin.deregisterNode( TestNode::m_id );
	if (!status)
	{
    status.perror( "unable to deregister TestNode" );
		return status;
	}
  MGlobal::displayInfo("uninitialize plugin called");

	return status;
}
//----------------------------------------------------------------------------------------------------------------------


