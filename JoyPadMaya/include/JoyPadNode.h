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



						JoyPadNode();
	virtual 			~JoyPadNode();

	virtual void		postConstructor();
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void		threadHandler();
	virtual void		threadShutdownHandler();

	static void*		creator();
	static MStatus	initialize();
private:
	/// @brief this is a dummy output given a random vlaue to trigger the compute
	/// method and update the other joystick values.
	static MObject	m_output;
	/// @brief left hat values  Left to right  (again may change to be dynamic later)
	static MObject	m_leftHatLR;
	/// @brief right hat values Up down
	static MObject	m_leftHatUD;
	/// @brief left hat values Left Right
	static MObject	m_rightHatLR;
	/// @brief left hat values Up down
	static MObject	m_rightHatUD;
	static MObject	m_rightTrigger;
	static MObject	m_leftTrigger;
	/// @brief global trigger threshold sensistivity value used to trigger analog event
	static MObject	m_sensitivity;
	static MObject	m_analogMode;
	static MObject	m_analogRange;
	static MObjectArray m_buttons;
	/// @brief these are the position in the data block for the thread
 /// to pass data from the SDL reading to the compute method
 /// Adding these values will increase DATABLOCKSIZE which is used in the
 /// allocation of the data block memory and __MUST__ always be last
  enum DataBlockValues{DBLOCKLEFTHATLR,
                       DBLOCKLEFTHATUD,
                       DBLOCKRIGHTHATLR,
                       DBLOCKRIGHTHATUD,
                       DBLOCKLEFTTRIGGER,
                       DBLOCKRIGHTTRIGGER,
                       DBLOCKUPDATEHACK, // leave this last used for updates
                       DATABLOCKSIZE //this must be last
                      };
  /// @brief enum used for analog stick modes at present one global version
  /// however may change this to a per hat mode for more flexibility
  enum ANALOGMODE{ABSOLUTE,ADD,SUB};
  /// @brief BUTTON mode determines if it will latch or not may be deprecated.
  enum BUTTONMODE{CONTACT,LATCH};


public :
	static MTypeId		m_id;
	static SDL_Joystick *m_js;
};


#endif
