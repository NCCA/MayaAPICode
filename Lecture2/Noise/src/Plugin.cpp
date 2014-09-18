#include "NoiseNode.h"
#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  // register our nodes and the commands which will be called
  status = plugin.registerNode( "noiseNode", NoiseNode::m_id, &NoiseNode::creator, &NoiseNode::initialize, MPxNode::kDependNode );
	if (!status)
	{
    status.perror("Unable to register NoiseNode" );
		return status;
	}


	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );


  status = plugin.deregisterNode( NoiseNode::m_id );
	if (!status)
	{
    status.perror( "unable to deregister NoiseNode" );
		return status;
	}

	return status;
}
//----------------------------------------------------------------------------------------------------------------------


