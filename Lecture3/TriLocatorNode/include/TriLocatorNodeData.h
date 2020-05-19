#ifndef TRILOCATORNODEDATA_H_
#define TRILOCATORNODEDATE_H_

#include <maya/MUserData.h>
#include <maya/MColor.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MColorArray.h>
#include <memory>
class TriLocatorNodeData : public MUserData
{
public:
	TriLocatorNodeData() : MUserData(false) 
	{ 
		fNormalsList = new MVectorArray(); 
		fColours = new MColorArray();
	} // don't delete after draw
	~TriLocatorNodeData() override 
	{ 
		delete fNormalsList; 
		delete fColours;
	}

	MColor fColor;
	MPointArray fLineList;
	MPointArray fTriangleList;
	MVectorArray *fNormalsList;
	MColorArray* fColours;
};

#endif