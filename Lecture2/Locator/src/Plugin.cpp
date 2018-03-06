#include "CubeLocatorNode.h"
#include "CubeLocatorNodeDrawOverride.h"

#include <maya/MFnPlugin.h>

//----------------------------------------------------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "NCCA" , "Any" );

  // register our nodes and the commands which will be called
  status = plugin.registerNode( "cubeLocator", CubeLocatorNode::s_id, &CubeLocatorNode::creator, &CubeLocatorNode::initialize, MPxNode::kLocatorNode );
	if (!status)
	{
		status.perror("Unable to register CubeLocatorNode" );
		return status;
	}

  status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
    CubeLocatorNode::s_drawDbClassification,
    CubeLocatorNode::s_drawRegistrantId,
    CubeLocatorDrawOverride::Creator);
  if (!status)
  {
    status.perror("registerDrawOverrideCreator");
    return status;
  }
	return status;
}

//----------------------------------------------------------------------------------------------------------------------

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );


  status = plugin.deregisterNode( CubeLocatorNode::s_id );
	if (!status)
	{
		status.perror( "unable to deregister CubeLocatorNode" );
		return status;
	}

  status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
    CubeLocatorNode::s_drawDbClassification,
     CubeLocatorNode::s_drawRegistrantId);
    if (!status)
    {
      status.perror("deregisterNode");
      return status;
    }

	return status;
}
//----------------------------------------------------------------------------------------------------------------------


