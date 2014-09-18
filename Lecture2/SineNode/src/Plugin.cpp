#include "SineNode.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  // register our nodes and the commands which will be called
  status = plugin.registerNode( "sineNode", SineNode::m_id, &SineNode::creator, &SineNode::initialize, MPxNode::kDependNode );
	if (!status)
	{
    status.perror("Unable to register sineNode" );
		return status;
	}


	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );


  status = plugin.deregisterNode( SineNode::m_id );
	if (!status)
	{
    status.perror( "unable to deregister sineNode" );
		return status;
	}

	return status;
}
//----------------------------------------------------------------------------------------------------------------------


