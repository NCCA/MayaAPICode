#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>

#include "TriLocatorNode.h"
 
MTypeId TriLocatorNode::id( 0x8001c );
MObject TriLocatorNode::size;
MString	TriLocatorNode::drawDbClassification("drawdb/geometry/TriLocator");
MString	TriLocatorNode::drawRegistrantId("TriManipPlugin");



MStatus TriLocatorNode::compute(const MPlug &plug, MDataBlock &data)
{ 
	return MS::kUnknownParameter;
}


bool TriLocatorNode::isBounded() const
{ 
	return true;
}


MBoundingBox TriLocatorNode::boundingBox() const
{   
	// Get the size
	//
	MObject thisNode = thisMObject();
	MPlug plug(thisNode, size);
	MDistance sizeVal;
	plug.getValue(sizeVal);

	double multiplier = sizeVal.asCentimeters();
 
	MPoint corner1(-0.17, 0.0, -0.7);
	MPoint corner2(0.17, 0.0, 0.3);

	corner1 = corner1 * multiplier;
	corner2 = corner2 * multiplier;

	return MBoundingBox(corner1, corner2);
}


void* TriLocatorNode::creator()
{
	return new TriLocatorNode();
}


MStatus TriLocatorNode::initialize()
{ 
	MFnUnitAttribute unitFn;
	MStatus			 stat;

	size = unitFn.create("size", "sz", MFnUnitAttribute::kDistance);
	unitFn.setDefault(10.0);
	unitFn.setStorable(true);
	unitFn.setWritable(true);

	stat = addAttribute(size);
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}
	
//	MPxManipContainer::addToManipConnectTable(id);

	return MS::kSuccess;
}


