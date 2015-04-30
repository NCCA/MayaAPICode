#ifndef JOYPADNODE_H__
#define JOYPADNODE_H__

#include <maya/MTypeId.h>

#include <api_macros.h>
#include <maya/MIOStream.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPxThreadedDeviceNode.h>
#include <SDL.h>
#include <maya/MGlobal.h>

class JoyPadNode : public MPxThreadedDeviceNode
{

public:
						JoyPadNode();
	virtual 			~JoyPadNode();

	virtual void		postConstructor();
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void		threadHandler();
	virtual void		threadShutdownHandler();

	static void*		creator();
	static MStatus		initialize();

public:

	static MObject		m_outputTranslate;
	static MObject 		m_outputTranslateX;
	static MObject		m_outputTranslateY;
	static MObject 		m_outputTranslateZ;
	// Boolean attribute for deciding if we are going
	// to update XZ or XY. Only two inputs from the
	// gameInput in this example
	static MObject		m_updateTranslateXZ;

	static MTypeId		m_id;
	static SDL_Joystick *m_js;
private:
};


#endif
