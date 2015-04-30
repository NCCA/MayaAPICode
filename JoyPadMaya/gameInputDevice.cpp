//-
// ==========================================================================
// Copyright 2011 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// Description:
//	This example demonstrates how to use left pad X and Y from
//	a game input device to modify the translate attribute of a node. The
//	MEL example code demonstrates how a poly cube can be moved
//	in X and Y.
//
//	There is an attribute updateTranslateXZ that controls if
//	the example will map game input(x,y) to Maya(x,0,z) or
//	Maya(x,y,0).
//
//	NOTE: Windows only example that requires the Direct X
// SDK to be installed in order to build. In addition, the
// game input device driver must be installed also.
//

/*
	// MEL:
	loadPlugin gameInputDevice;
		string $node = `createNode gameInputDevice`;
		string $cube[] = `polyCube`;
		connectAttr ( $node + ".outputTranslate" ) ( $cube[0] + ".translate" );
		setAttr ( $node + ".live" ) 1;
*/

enum XBOXAxis
{
  LEFTSTICKLR, //0
  LEFTSTICKUD, // 1
  LEFTTRIGGER, // 2
  RIGHTSTICKLR, //3
  RIGHTSTICKUD, // 4
  RIGHTTRIGGER // 5
};


// So mappings are different for the same controller on different os!
enum XBOXButtons
{
  XBOXBUTTONA, // 0
  XBOXBUTTONB, // 1
  XBOXBUTTONX, // 2
  XBOXBUTTONY, // 3
  XBOXBUTTONLBUMP, // 4
  XBOXBUTTONRBUMP, // 5
  XBOXBUTTONLHAT, //6
  XBOXBUTTONRHAT, // 7
  XBOXBUTTONSTART, // 8
  XBOXBUTTONBACK, //9
  NONE, //10 is not allocated
  XBOXDPADUP, // 11
  XBOXDPADDOWN, //12
  XBOXDPADLEFT, //13
  XBOXDPARRIGHT //14
};


#include <maya/MFnPlugin.h>
#include <maya/MTypeId.h>

#include <api_macros.h>
#include <maya/MIOStream.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPxThreadedDeviceNode.h>
#include <SDL.h>
#include <maya/MGlobal.h>

class gameInputDeviceNode : public MPxThreadedDeviceNode
{

public:
						gameInputDeviceNode();
	virtual 			~gameInputDeviceNode();
	
	virtual void		postConstructor();
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void		threadHandler();
	virtual void		threadShutdownHandler();

	static void*		creator();
	static MStatus		initialize();

public:

	static MObject		outputTranslate;
	static MObject 		outputTranslateX;
	static MObject		outputTranslateY;
	static MObject 		outputTranslateZ;
	// Boolean attribute for deciding if we are going
	// to update XZ or XY. Only two inputs from the
	// gameInput in this example
	static MObject		updateTranslateXZ;

	static MTypeId		id;
	static SDL_Joystick *m_js;
private:
};

MTypeId gameInputDeviceNode::id( 0x00081053 );
MObject gameInputDeviceNode::outputTranslate;
MObject gameInputDeviceNode::outputTranslateX;
MObject gameInputDeviceNode::outputTranslateY;
MObject gameInputDeviceNode::outputTranslateZ;
MObject gameInputDeviceNode::updateTranslateXZ;
SDL_Joystick *gameInputDeviceNode::m_js;
SDL_Joystick *getJoystick(int _index);

/// @brief the value to update the ship position by
const static float UPDATE=0.5;
/// @brief the max joystick analogue value
const static int JOYMAX=32767;
/// @brief a sensitivity thereshold for the js.
const static int SENS=3200;



gameInputDeviceNode::gameInputDeviceNode() 
{}

gameInputDeviceNode::~gameInputDeviceNode()
{
	destroyMemoryPools();
}

void gameInputDeviceNode::postConstructor()
{
	MObjectArray attrArray;
	attrArray.append( gameInputDeviceNode::outputTranslate );
	setRefreshOutputAttributes( attrArray );

	// we'll be reading one set of translate x,y, z's at a time
	createMemoryPools( 24, 3, sizeof(double));
}



void gameInputDeviceNode::threadHandler()
{
  MStatus status;
  char msg[100];

  while ( ! isDone() )
  {
    if ( ! isLive() )
         continue;

    MCharBuffer buffer;
    status = acquireDataStorage(buffer);
    if ( ! status )
            continue;

    beginThreadLoop();
    {
      float changeX = 0.0, changeY = 0.0;

      if(m_js != 0)
       {
        int x,y;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
        switch (event.type)
        {
        case SDL_JOYAXISMOTION:
          x=0;
          y=0;
          sprintf(msg,"Joystick %d axis %d value: %d\n",
                                 event.jaxis.which,
                                 event.jaxis.axis, event.jaxis.value);
          //MGlobal::displayInfo(msg);

          if(event.jaxis.which==0) // up down
          {
            x=event.jaxis.value;
          }
          if(event.jaxis.which==1) // lr
          {
            y=event.jaxis.value;
          }

            // filter against a tolerance to get rid of any noise
            if ( ( x <= -SENS ) || (x >= SENS ) )
            {
              changeX=UPDATE*x/float(JOYMAX);
              sprintf(msg,"Change X  %f",changeX);
            //  MGlobal::displayInfo(msg);

            }
            else changeX=0.0;

            if ( ( y <= -SENS ) || (y >= SENS ) )
            {
              changeY=-UPDATE*y/float(JOYMAX);
              sprintf(msg,"Change Y  %f",changeY);
             // MGlobal::displayInfo(msg);

            }
            else changeY=0.0;



          break;


        case SDL_JOYBUTTONDOWN:
            sprintf(msg,"Joystick %d button %d down\n",
                   event.jbutton.which, event.jbutton.button);
           // MGlobal::displayInfo(msg);

            break;
        case SDL_JOYBUTTONUP:
            sprintf(msg,"Joystick %d button %d up\n",
                   event.jbutton.which, event.jbutton.button);
            //MGlobal::displayInfo(msg);

            break;
        case SDL_JOYBALLMOTION:
                       sprintf(msg,"Joystick %d ball %d delta: (%d,%d)\n",
                               event.jball.which,
                               event.jball.ball, event.jball.xrel, event.jball.yrel);
                      // MGlobal::displayInfo(msg);

          break;
        }


        }


                short int xx=0;
                xx=SDL_JoystickGetAxis(m_js, LEFTSTICKLR);
                short int yy=0;
                yy=SDL_JoystickGetAxis(m_js, LEFTSTICKUD);
                sprintf(msg,"FromGetAxis %d %d",xx,yy);
                MGlobal::displayInfo(msg);


//
//      while ( SDL_PollEvent(&event) );
        // check back button to quit

//        if(SDL_JoystickGetButton(js,XBOXBUTTONBACK) )
//       {
//         MGlobal::displayError("Done");
//         setDone(true);
//       }

//        // process the joystick axis 0 is the left stick l->r
//        // axis 1 is the left stick u->d defined in the header
//        int x=0;
//        x=SDL_JoystickGetAxis(js, LEFTSTICKLR);
//        int y=0;
//        y=SDL_JoystickGetAxis(js, LEFTSTICKUD);
//

//        // filter against a tolerance to get rid of any noise
//        if ( ( x <= -SENS ) || (x >= SENS ) )
//        {
//          changeX=UPDATE*x/float(JOYMAX);
//          sprintf(msg,"Change X  %f",changeX);
//          MGlobal::displayInfo(msg);

//        }
//        else changeX=0.0;

//        if ( ( y <= -SENS ) || (y >= SENS ) )
//        {
//          changeY=-UPDATE*y/float(JOYMAX);
//          sprintf(msg,"Change Y  %f",changeY);
//          MGlobal::displayInfo(msg);

//        }
//        else changeY=0.0;
//        // now check the right stick for the in / out
//        // this is index 4 defined in the header
//        int z=SDL_JoystickGetAxis(js, RIGHTSTICKUD);
//        if ( ( z <= -SENS ) || (z >= SENS ) )
//        {
//          zupdate=UPDATE*z/float(JOYMAX);
//        }
//        else zupdate=0.0;

      }


      double* doubleData = reinterpret_cast<double*>(buffer.ptr());
      doubleData[0] = changeX ; doubleData[1] = changeY; doubleData[2] = 0.0;

    pushThreadData( buffer );
    }
    endThreadLoop();


  }
  setDone( true );

}

void gameInputDeviceNode::threadShutdownHandler()
{
	setDone( true );
}

void* gameInputDeviceNode::creator()
{
	return new gameInputDeviceNode;
}

MStatus gameInputDeviceNode::initialize()
{
  // Initialize SDL's Video subsystem
    if (SDL_Init(  SDL_INIT_JOYSTICK ) < 0 )
    {
      // Or die on error
      //MCHECKERROR(0,"Unable to initialize SDL");
    }
    m_js=getJoystick(0);



    //
//			int numJoyPads=SDL_NumJoysticks();
//			std::cout<<"Found "<<numJoyPads<<" Joypads\n";
//			if(numJoyPads ==0)
//			{
//				std::cerr<<"This demo needs a Joypad falling back to key control\n";
//			}
//		//	m_js= getJoystick(0);

	MStatus status;
	MFnNumericAttribute numAttr;

	outputTranslateX = numAttr.create("outputTranslateX", "otx", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateX");
	outputTranslateY = numAttr.create("outputTranslateY", "oty", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateY");
	outputTranslateZ = numAttr.create("outputTranslateZ", "otz", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateZ");
	outputTranslate = numAttr.create("outputTranslate", "ot", outputTranslateX, outputTranslateY, 
									 outputTranslateZ, &status);
	MCHECKERROR(status, "create outputTranslate");

	updateTranslateXZ = numAttr.create( "updateTranslateXZ", "uxz", MFnNumericData::kBoolean);
	CHECK_MSTATUS( numAttr.setKeyable(true) );
	CHECK_MSTATUS( numAttr.setStorable(true) );
	CHECK_MSTATUS( numAttr.setHidden(false) );
	CHECK_MSTATUS( numAttr.setDefault(true) );

	ADD_ATTRIBUTE(outputTranslate);
	ADD_ATTRIBUTE(updateTranslateXZ);

	ATTRIBUTE_AFFECTS( live, outputTranslate);
	ATTRIBUTE_AFFECTS( frameRate, outputTranslate);
	ATTRIBUTE_AFFECTS( updateTranslateXZ, outputTranslate);

	return MS::kSuccess;
}

MStatus gameInputDeviceNode::compute( const MPlug& plug, MDataBlock& block )
{
	MStatus status;
	if( plug == outputTranslate || plug == outputTranslateX ||
		plug == outputTranslateY || plug == outputTranslateZ )
	{
		// Find the type of translation we will be doing
		bool xzUpdate  = block.inputValue( updateTranslateXZ ).asBool();

		// Access the data and update the output attribute
		MCharBuffer buffer;
		if ( popThreadData(buffer) )
		{
			// Relative data coming in
			double* doubleData = reinterpret_cast<double*>(buffer.ptr());

			MDataHandle outputTranslateHandle = block.outputValue( outputTranslate, &status );
			MCHECKERROR(status, "Error in block.outputValue for outputTranslate");

			double3& outputTranslate = outputTranslateHandle.asDouble3();
			if ( xzUpdate ) 
			{
				// XZ
				outputTranslate[0] += doubleData[0];
				outputTranslate[1] += doubleData[2];
				outputTranslate[2] -= doubleData[1];
			}
			else
			{
				// XY
				outputTranslate[0] += doubleData[0];
				outputTranslate[1] += doubleData[1];
				outputTranslate[2] += doubleData[2];
			}
			block.setClean( plug );

			releaseDataStorage(buffer);
			return ( MS::kSuccess );
		}
		else
		{
			return MS::kFailure;
		}
	}

	return ( MS::kUnknownParameter );
}

MStatus initializePlugin( MObject obj )
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "gameInputDevice", 
								  gameInputDeviceNode::id,
								  gameInputDeviceNode::creator,
								  gameInputDeviceNode::initialize,
								  MPxNode::kThreadedDeviceNode );
	if( !status ) {
		status.perror("failed to registerNode gameInputDeviceNode");
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode( gameInputDeviceNode::id );
	if( !status ) {
		status.perror("failed to deregisterNode gameInputDeviceNode");
	}

	return status;
}


SDL_Joystick *getJoystick(int _index)
{
SDL_Joystick *js;

std::cerr<<"The name of the joystick is:\n";

//std::cerr<<SDL_JoystickName(_index)<<"\n";

	SDL_JoystickEventState(SDL_ENABLE);
	js=SDL_JoystickOpen(_index);
	char msg[100];
	sprintf(msg,"Num Axis %d",SDL_JoystickNumAxes(js));
	MGlobal::displayInfo(msg);
	sprintf(msg,"Num Buttons %d",SDL_JoystickNumButtons(js));//	Returns the number of joysitck buttons
			MGlobal::displayInfo(msg);
	sprintf(msg,"Num Balls %d",SDL_JoystickNumBalls(js));//	Returns the number of joysitck balls
			MGlobal::displayInfo(msg);
	sprintf(msg,"Num Num Hats %d",SDL_JoystickNumHats(js));//	Returns the number of joysitck hats
MGlobal::displayInfo(msg);
	return js;
}
