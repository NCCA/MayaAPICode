#include "JoyPadNode.h"

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




MTypeId JoyPadNode::m_id( 0x00081053 );
MObject JoyPadNode::m_outputTranslate;
MObject JoyPadNode::m_outputTranslateX;
MObject JoyPadNode::m_outputTranslateY;
MObject JoyPadNode::m_outputTranslateZ;
MObject JoyPadNode::m_updateTranslateXZ;
SDL_Joystick *JoyPadNode::m_js;
SDL_Joystick *getJoystick(int _index);

/// @brief the value to update the ship position by
const static float UPDATE=0.5;
/// @brief the max joystick analogue value
const static int JOYMAX=32767;
/// @brief a sensitivity thereshold for the js.
const static int SENS=3200;



JoyPadNode::JoyPadNode()
{}

JoyPadNode::~JoyPadNode()
{
	destroyMemoryPools();
}

void JoyPadNode::postConstructor()
{
	MObjectArray attrArray;
	attrArray.append( JoyPadNode::m_outputTranslate );
	setRefreshOutputAttributes( attrArray );

	// we'll be reading one set of translate x,y, z's at a time
	createMemoryPools( 24, 3, sizeof(double));
}



void JoyPadNode::threadHandler()
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
      float changeX = 0.0, changeY = 0.0, changeZ =0.0;

      if(m_js != 0)
       {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
        switch (event.type)
        {

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
          }


        }




//        if(SDL_JoystickGetButton(js,XBOXBUTTONBACK) )
//       {
//         MGlobal::displayError("Done");
//         setDone(true);
//       }

        // process the joystick axis 0 is the left stick l->r
        // axis 1 is the left stick u->d defined in the header
        short int x=0;
        x=SDL_JoystickGetAxis(m_js, LEFTSTICKLR);
        short int y=0;
        y=SDL_JoystickGetAxis(m_js, LEFTSTICKUD);


        // filter against a tolerance to get rid of any noise
        if ( ( x <= -SENS ) || (x >= SENS ) )
        {
          changeX=UPDATE*x/float(JOYMAX);
          sprintf(msg,"Change X  %f",changeX);
          MGlobal::displayInfo(msg);

        }
        else changeX=0.0;

        if ( ( y <= -SENS ) || (y >= SENS ) )
        {
          changeY=-UPDATE*y/float(JOYMAX);
          sprintf(msg,"Change Y  %f",changeY);
          MGlobal::displayInfo(msg);

        }
        else changeY=0.0;
        // now check the right stick for the in / out
        // this is index 4 defined in the header
        short int z=-SDL_JoystickGetAxis(m_js, RIGHTSTICKUD);
        if ( ( z <= -SENS ) || (z >= SENS ) )
        {
          changeZ=UPDATE*z/float(JOYMAX);
        }
        else changeZ=0.0;

      }


      double* doubleData = reinterpret_cast<double*>(buffer.ptr());
      doubleData[0] = changeX ; doubleData[1] = changeY; doubleData[2] = changeZ;

    pushThreadData( buffer );
    }
    endThreadLoop();


  }
  setDone( true );

}

void JoyPadNode::threadShutdownHandler()
{
	setDone( true );
}

void* JoyPadNode::creator()
{
	return new JoyPadNode;
}

MStatus JoyPadNode::initialize()
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

	m_outputTranslateX = numAttr.create("outputTranslateX", "otx", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateX");
	m_outputTranslateY = numAttr.create("outputTranslateY", "oty", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateY");
	m_outputTranslateZ = numAttr.create("outputTranslateZ", "otz", MFnNumericData::kDouble, 0.0, &status);
	MCHECKERROR(status, "create outputTranslateZ");
	m_outputTranslate = numAttr.create("outputTranslate", "ot", m_outputTranslateX, m_outputTranslateY,
									 m_outputTranslateZ, &status);
	MCHECKERROR(status, "create outputTranslate");

	m_updateTranslateXZ = numAttr.create( "updateTranslateXZ", "uxz", MFnNumericData::kBoolean);
	CHECK_MSTATUS( numAttr.setKeyable(true) );
	CHECK_MSTATUS( numAttr.setStorable(true) );
	CHECK_MSTATUS( numAttr.setHidden(false) );
	CHECK_MSTATUS( numAttr.setDefault(true) );

	ADD_ATTRIBUTE(m_outputTranslate);
	ADD_ATTRIBUTE(m_updateTranslateXZ);

	ATTRIBUTE_AFFECTS( live, m_outputTranslate);
	ATTRIBUTE_AFFECTS( frameRate, m_outputTranslate);
	ATTRIBUTE_AFFECTS( m_updateTranslateXZ, m_outputTranslate);

	return MS::kSuccess;
}

MStatus JoyPadNode::compute( const MPlug& plug, MDataBlock& block )
{
	MStatus status;
	if( plug == m_outputTranslate || plug == m_outputTranslateX ||
		plug == m_outputTranslateY || plug == m_outputTranslateZ )
	{
		// Find the type of translation we will be doing
		bool xzUpdate  = block.inputValue( m_updateTranslateXZ ).asBool();

		// Access the data and update the output attribute
		MCharBuffer buffer;
		if ( popThreadData(buffer) )
		{
			// Relative data coming in
			double* doubleData = reinterpret_cast<double*>(buffer.ptr());

			MDataHandle outputTranslateHandle = block.outputValue( m_outputTranslate, &status );
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
