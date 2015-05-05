#include "JoyPadNode.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MFnCompoundAttribute.h>
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

//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------
#define CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( stat , message )				\
	if( !status )										\
	{											\
		MString errorString = status.errorString() + " -- " + MString( message );	\
		MGlobal::displayError( errorString );						\
		return MStatus::kFailure;							\
	}											\
//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------

#define CHECK_STATUS_AND_RETURN_IF_FAIL( stat , message )					\
	if( !status )										\
	{											\
		MString errorString = status.errorString() + " -- " + MString( message );	\
		MGlobal::displayError( errorString );						\
	}											\




MTypeId JoyPadNode::m_id( 0x00081053 );
MObject JoyPadNode::m_output;
MObject JoyPadNode::m_leftHatLR;
MObject JoyPadNode::m_leftHatUD;
MObject JoyPadNode::m_rightHatLR;
MObject JoyPadNode::m_rightHatUD;
MObject JoyPadNode::m_leftTrigger;
MObject JoyPadNode::m_rightTrigger;
MObjectArray JoyPadNode::m_buttons;


MObject JoyPadNode::m_sensitivity;



SDL_Joystick *JoyPadNode::m_js;
SDL_Joystick *getJoystick(int _index);

/// @brief the value to update the ship position by
const static float UPDATE=0.5;
/// @brief the max joystick analogue value
const static int JOYMAX=32767;
/// @brief a sensitivity thereshold for the js.



JoyPadNode::JoyPadNode()
{}

JoyPadNode::~JoyPadNode()
{
	destroyMemoryPools();
}

void JoyPadNode::postConstructor()
{
	MObjectArray attrArray;
	attrArray.append(JoyPadNode::m_output);

	setRefreshOutputAttributes( attrArray );

	createMemoryPools( 24, DATABLOCKSIZE, sizeof(double));
}



void JoyPadNode::threadHandler()
{
  if (SDL_Init(  SDL_INIT_JOYSTICK ) < 0 )
  {
    // Or die on error
    //MCHECKERROR(0,"Unable to initialize SDL");
  }
  m_js=getJoystick(0);

  MStatus status;
  int sensitivity;
  MFnDependencyNode dependencyFn( thisMObject() , &status );
  CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable to initialize dependency node" );

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

      MPlug sensitivityPlug = dependencyFn.findPlug( m_sensitivity , true , &status );
      CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get sensitivity plug" );
      // now grab the value and place in our variable
      status = sensitivityPlug.getValue( sensitivity );
      CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get value from sensitivity plug" );

      float lhatUD=0.0;
      float rhatUD=0.0;
      float lhatLR=0.0;
      float rhatLR=0.0;
      if(m_js != 0)
       {
       SDL_Event event;
       // we need to pump the event queue before reading JS values
       while (SDL_PollEvent(&event))
        {

         switch (event.type)
         {
            case SDL_CONTROLLERDEVICEADDED:
              MGlobal::displayError("Joypad added");
            break;

           case SDL_CONTROLLERDEVICEREMOVED:
           MGlobal::displayError("Joypad removed");

           break;


         }

        }

        // process the joystick axis 0 is the left stick l->r
        // axis 1 is the left stick u->d defined in the header
        short int lx=0;
        lx=SDL_JoystickGetAxis(m_js, LEFTSTICKLR);
        short int ly=0;
        ly=SDL_JoystickGetAxis(m_js, LEFTSTICKUD);

        short int rx=0;
        rx=SDL_JoystickGetAxis(m_js, RIGHTSTICKLR);
        short int ry=0;
        ry=SDL_JoystickGetAxis(m_js, RIGHTSTICKUD);

        // filter against a tolerance to get rid of any noise
        if ( ( lx <= -sensitivity ) || (lx >= sensitivity ) )
        {
          lhatLR=UPDATE*lx/float(JOYMAX);
        }
        else lhatLR=0.0;

        if ( ( rx <= -sensitivity ) || (rx >= sensitivity ) )
        {
          rhatLR=UPDATE*rx/float(JOYMAX);
        }
        else rhatLR=0.0;

        if ( ( ly <= -sensitivity ) || (ly >= sensitivity ) )
        {
          lhatUD=-UPDATE*ly/float(JOYMAX);

        }
        else lhatUD=0.0;

        if ( ( ry <= -sensitivity ) || (ry >= sensitivity ) )
        {
          rhatUD=-UPDATE*ry/float(JOYMAX);

        }
        else rhatUD=0.0;

      }

      // make this go from 0 -> JOYMAX as the data is actually from
      // -32768 - 0 - 32766 (short)
      short int leftTrigger=SDL_JoystickGetAxis(m_js,LEFTTRIGGER)+JOYMAX+1;

      short int rightTrigger=SDL_JoystickGetAxis(m_js,RIGHTTRIGGER)+JOYMAX+1;




      double* doubleData = reinterpret_cast<double*>(buffer.ptr());
      doubleData[DBLOCKLEFTHATLR]=lhatLR;
      doubleData[DBLOCKLEFTHATUD]=lhatUD;
      doubleData[DBLOCKRIGHTHATLR]=rhatLR;
      doubleData[DBLOCKRIGHTHATUD]=rhatUD;
      doubleData[DBLOCKLEFTTRIGGER]=leftTrigger;
      doubleData[DBLOCKRIGHTTRIGGER]=rightTrigger;

      doubleData[DBLOCKUPDATEHACK]=rand(); //hack!
      pushThreadData( buffer );
    }
    endThreadLoop();


  }
  setDone( true );

}



void JoyPadNode::threadShutdownHandler()
{
	SDL_Quit();
	setDone( true );
}

void* JoyPadNode::creator()
{
	return new JoyPadNode;
}

MStatus JoyPadNode::initialize()
{


	MStatus status;

	MFnNumericAttribute	numAttr;

  m_sensitivity = numAttr.create( "sensitivity", "sens", MFnNumericData::kInt, 3200, &status );
  numAttr.setMin(150);
  numAttr.setSoftMin(200);
  numAttr.setSoftMax(32000);
  numAttr.setMax(33000);


  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"sensitivity\" attribute" );
  // add attribute
  status = addAttribute( m_sensitivity );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"sensitivity\" attribute to Node" );

  m_leftHatLR = numAttr.create( "leftHatLR", "lhlr", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"leftHatLR\" attribute" );
  // add attribute
  status = addAttribute( m_leftHatLR );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"leftHatLR\" attribute to Node" );

  m_leftHatUD = numAttr.create( "leftHatUD", "lhud", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"leftHatUD\" attribute" );
  // add attribute
  status = addAttribute( m_leftHatUD );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"leftHatUD\" attribute to Node" );


  m_rightHatLR = numAttr.create( "rightHatLR", "rhlr", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"rightHatLR\" attribute" );
  // add attribute
  status = addAttribute( m_rightHatLR );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"rightHatLR\" attribute to Node" );

  m_rightHatUD = numAttr.create( "rightHatUD", "rhud", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"rightHatUD\" attribute" );
  // add attribute
  status = addAttribute( m_rightHatUD );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"rightHatUD\" attribute to Node" );

  m_leftTrigger = numAttr.create( "leftTriger", "ltrig", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"leftTrigger\" attribute" );
  // add attribute
  status = addAttribute( m_leftTrigger );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"leftTrigger\" attribute to Node" );

  m_rightTrigger = numAttr.create( "rightTriger", "rtrig", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"rightTrigger\" attribute" );
  // add attribute
  status = addAttribute( m_rightTrigger );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"rightTrigger\" attribute to Node" );


  int numButtons=SDL_JoystickNumButtons(m_js);
  for(int i=0; i<numButtons; ++i)
  {
    char longstr[40];
    char shortstr[6];
    sprintf(longstr,"button%02d",i);
    sprintf(shortstr,"bt%02d",i);

    m_buttons.append(numAttr.create(longstr,shortstr,MFnNumericData::kBoolean, 0, &status));
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"button\" attribute" );
    // add attribute
    status = addAttribute( m_buttons[i] );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"button\" attribute to Node" );

  }

  m_output = numAttr.create( "dummyOut", "dout", MFnNumericData::kDouble, 0.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"output\" attribute" );
  // add attribute
  numAttr.setKeyable(true);
  numAttr.setStorable(true);
  numAttr.setHidden(true);
  numAttr.setDefault(true);

  status = addAttribute( m_output );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"output\" attribute to Node" );


	attributeAffects( live, m_leftHatLR);
	attributeAffects( live, m_leftHatUD);
	attributeAffects( live, m_rightHatLR);
	attributeAffects( live, m_rightHatUD);
	attributeAffects( live, m_leftTrigger);
	attributeAffects( live, m_rightTrigger);
	attributeAffects( live, m_output);

	attributeAffects( m_output, m_leftHatLR);
	attributeAffects( m_output, m_leftHatUD);
	attributeAffects( m_output, m_rightHatLR);
	attributeAffects( m_output, m_rightHatUD);
	attributeAffects( m_output, m_leftTrigger);
	attributeAffects( m_output, m_rightTrigger);

	attributeAffects( frameRate, m_leftHatLR);
	attributeAffects( frameRate, m_leftHatUD);
	attributeAffects( frameRate, m_rightHatLR);
	attributeAffects( frameRate, m_rightHatUD);
	attributeAffects( frameRate, m_output);
	attributeAffects( frameRate, m_leftTrigger);
	attributeAffects( frameRate, m_rightTrigger);

	attributeAffects( m_sensitivity, m_leftHatLR);
	attributeAffects( m_sensitivity, m_leftHatUD);
	attributeAffects( m_sensitivity, m_rightHatLR);
	attributeAffects( m_sensitivity, m_rightHatUD);
	attributeAffects( m_sensitivity, m_leftTrigger);
	attributeAffects( m_sensitivity, m_rightTrigger);



	return MS::kSuccess;
}

MStatus JoyPadNode::compute( const MPlug& plug, MDataBlock& block )
{
	MStatus status;


	if( plug==m_output || plug== m_leftHatLR || plug == m_leftHatUD ||
			plug==m_rightHatLR || plug == m_rightHatUD)
	{

		// Access the data and update the output attribute
		MCharBuffer buffer;
		if ( popThreadData(buffer) )
		{
			// Relative data coming in
		double* doubleData = reinterpret_cast<double*>(buffer.ptr());

		MDataHandle hndleftLR=block.outputValue( m_leftHatLR, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_leftHatLR");
		double &outleftLR=hndleftLR.asDouble();
		outleftLR+=doubleData[DBLOCKLEFTHATLR];

		MDataHandle hndleftUD=block.outputValue( m_leftHatUD, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_leftHatLR");
		double &outleftUD=hndleftUD.asDouble();
		outleftUD+=doubleData[DBLOCKLEFTHATUD];

		MDataHandle hndrightLR=block.outputValue( m_rightHatLR, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_leftHatLR");
		double &outrightLR=hndrightLR.asDouble();
		outrightLR+=doubleData[DBLOCKRIGHTHATLR];

		MDataHandle hndrightUD=block.outputValue( m_rightHatUD, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_leftHatLR");
		double &outrightUD=hndrightUD.asDouble();
		outrightUD+=doubleData[DBLOCKRIGHTHATUD];

		MDataHandle hndleftTrigger=block.outputValue( m_leftTrigger, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_leftTrigger");
		double &outleftTrigger=hndleftTrigger.asDouble();
		outleftTrigger+=doubleData[DBLOCKLEFTTRIGGER];

		MDataHandle hndrightTrigger=block.outputValue( m_rightTrigger, &status );
		CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL(status, "Error in block.outputValue for m_rightTrigger");
		double &outrightTrigger=hndrightTrigger.asDouble();
		outrightTrigger+=doubleData[DBLOCKRIGHTTRIGGER];
		MFnDependencyNode dependencyFn( thisMObject() , &status );
		CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable to initialize dependency node" );
		for(unsigned int i=0; i<m_buttons.length(); ++i)
		{
			MPlug buttonPlug = dependencyFn.findPlug( m_buttons[i] , true , &status );
			CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get button plug" );
			// now grab the value and place in our variable
			status = buttonPlug.setValue( SDL_JoystickGetButton(m_js,i) );
			CHECK_STATUS_AND_RETURN_IF_FAIL( status , "Unable get value from button plug" );

		}

		}
		block.setClean( plug );

			releaseDataStorage(buffer);
			return ( MS::kSuccess );

		}
		else
		{
			return MS::kFailure;
		}

	return MS::kUnknownParameter;

}



SDL_Joystick *getJoystick(int _index)
{
	SDL_Joystick *js;

	std::cerr<<"The name of the joystick is:\n";


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



