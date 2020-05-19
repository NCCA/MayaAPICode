#include "TriLocatorNode.h"
#include "TriLocatorNodeDrawOverride.h"
#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>


MStatus initializePlugin(MObject obj)
{ 
	MStatus status;
	MFnPlugin plugin(obj, "NCCA", "1.0", "Any");

	status = plugin.registerNode("TriLocatorNode", 
								 TriLocatorNode::id, 
								 &TriLocatorNode::creator, 
								 &TriLocatorNode::initialize,
								 MPxNode::kLocatorNode, 
								 &TriLocatorNode::drawDbClassification
								 );
	if (!status) 
  {
		status.perror("registerNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		TriLocatorNode::drawDbClassification,
		TriLocatorNode::drawRegistrantId,
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

	status = plugin.deregisterNode(TriLocatorNode::id);
	if (!status) 
  {
		status.perror("deregisterNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
		TriLocatorNode::drawDbClassification,
		TriLocatorNode::drawRegistrantId);
	if (!status) 
  {
		status.perror("deregisterNode");
		return status;
	}
	return status;
}
