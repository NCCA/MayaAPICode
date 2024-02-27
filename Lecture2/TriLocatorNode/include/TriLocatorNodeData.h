#ifndef TRILOCATORNODEDATA_H_
#define TRILOCATORNODEDATA_H_

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
		m_normalArray = new MVectorArray(); 
		m_colourArray = new MColorArray();
	} // don't delete after draw
	~TriLocatorNodeData() override 
	{ 
		delete m_normalArray; 
		delete m_colourArray;
	}

	//MColor m_colour;
	MPointArray m_lineArray;
	MPointArray m_triangleArray;
	MVectorArray *m_normalArray; // raw pointers can we use smart ones?
	MColorArray *m_colourArray;
};

#endif