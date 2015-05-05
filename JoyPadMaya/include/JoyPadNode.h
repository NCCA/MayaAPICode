#ifndef JOYPADNODE_H__
#define JOYPADNODE_H__

#include <maya/MTypeId.h>

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

	enum DataBlockValues{DBLOCKLEFTHATLR,
											 DBLOCKLEFTHATUD,
											 DBLOCKRIGHTHATLR,
											 DBLOCKRIGHTHATUD,
											 DBLOCKLEFTTRIGGER,
											 DBLOCKRIGHTTRIGGER,
											 DBLOCKUPDATEHACK, // leave this last used for updates
											 DATABLOCKSIZE //this must be last
											};


						JoyPadNode();
	virtual 			~JoyPadNode();

	virtual void		postConstructor();
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void		threadHandler();
	virtual void		threadShutdownHandler();

	static void*		creator();
	static MStatus	initialize();
private:
	enum ANALOGMODE{ABSOLUTE,ADD,SUB};

	static MObject	m_output;
	static MObject	m_leftHatLR;
	static MObject	m_leftHatUD;
	static MObject	m_rightHatLR;
	static MObject	m_rightHatUD;
	static MObject	m_sensitivity;
	static MObject	m_rightTrigger;
	static MObject	m_leftTrigger;
	static MObject	m_analogMode;
	static MObject	m_analogRange;

	static MObjectArray m_buttons;
public :
	static MTypeId		m_id;
	static SDL_Joystick *m_js;
};


#endif
