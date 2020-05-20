#include "TriLocatorNode.h"
#include "TriLocatorNodeDrawOverride.h"
#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>


MStatus initializePlugin(MObject obj)
{ 
	MStatus status;
	MFnPlugin plugin(obj, "NCCA", "1.0", "Any");

	status = plugin.registerNode("TriLocatorNode", 
								 TriLocatorNode::s_id, 
								 &TriLocatorNode::creator, 
								 &TriLocatorNode::initialize,
								 MPxNode::kLocatorNode, 
								 &TriLocatorNode::s_drawDbClassification
								 );
	if (!status) 
  {
		status.perror("registerNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		TriLocatorNode::s_drawDbClassification,
		TriLocatorNode::s_drawRegistrantId,
		TriLocatorNodeDrawOverride::Creator);
	if (!status) 
  {
		status.perror("registerDrawOverrideCreator");
		return status;
	}
	
	return status;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(TriLocatorNode::s_id);
	if (!status) 
  {
		status.perror("deregisterNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		TriLocatorNode::s_drawDbClassification,
		TriLocatorNode::s_drawRegistrantId);
	if (!status) 
  {
		status.perror("deregisterNode");
		return status;
	}
	return status;
}
