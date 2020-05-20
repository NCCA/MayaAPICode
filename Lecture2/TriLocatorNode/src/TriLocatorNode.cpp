#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>

#include "TriLocatorNode.h"

MTypeId TriLocatorNode::s_id( 0x8001c );
MObject TriLocatorNode::m_size;
MString	TriLocatorNode::s_drawDbClassification("drawdb/geometry/TriLocator");
MString	TriLocatorNode::s_drawRegistrantId("TriManipPlugin");



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
	MPlug plug(thisNode, m_size);
	MDistance sizeVal;
	plug.getValue(sizeVal);

	double multiplier = sizeVal.asCentimeters();
	constexpr static float size = 0.5f;
	MPoint corner1(-size,size, -0.0f);
	MPoint corner2(size, -size, 0.0f);

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

	m_size = unitFn.create("size", "sz", MFnUnitAttribute::kDistance);
	unitFn.setDefault(1.0);
	unitFn.setStorable(true);
	unitFn.setWritable(true);

	stat = addAttribute(m_size);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}
	
	return MS::kSuccess;
}


