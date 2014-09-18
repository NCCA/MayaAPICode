#include "SineNode.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MTime.h>


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

/// @brief macro to get rid of compiler warnings
#define UNUSED(arg) (void)arg;

//----------------------------------------------------------------------------------------------------------------------
// as these are static initialisers we need to set them here.
MTypeId SineNode::m_id( 0x70001 );		// numeric Id of node
const MString SineNode::m_typeName( "sineNode" );
MObject SineNode::m_amplitude;
MObject SineNode::m_frequency;
MObject SineNode::m_time;
MObject SineNode::m_output;
MObject SineNode::m_functionType;

//----------------------------------------------------------------------------------------------------------------------
MStatus	SineNode::initialize()
{
  MStatus status;
  // the input attibute is a time attribute that will be connected to the global time node attribute,
  // time1.outTime
  //
  //	Any attribute types that are measured in changeable units, ie distance(cm, mm, miles, feet),
  //	angles( degrees, radians ), time( secs, mins, frames ) etc should be created using the
  //	MFnUnitAttribute function set.
  //
  MFnUnitAttribute uAttr;
  m_time =  uAttr.create( "time", "t", MFnUnitAttribute::kTime );
  // since the input attribute will be connected to the time value, there is little point
  // in storing this attribute within the maya binary file. There is also no point in
  // making the attribute keyable.
  uAttr.setStorable(false);
  uAttr.setKeyable(false);
  status = addAttribute( m_time );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"time\" attribute to SineNode" );


  MFnEnumAttribute    enumAttr;
  // note that f is used later as the shortcut for frequency so we use ft
  m_functionType = enumAttr.create( "function", "ft", 0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"function\" attribute" );

  enumAttr.addField( "sine", SINE );
  enumAttr.addField( "cos", COS );
  enumAttr.addField( "complex", COMPLEX );
  enumAttr.setHidden( false );
  enumAttr.setKeyable( true );
  status = addAttribute( m_functionType );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"function type\" attribute to SineNode" );


  MFnNumericAttribute	numAttr;

  m_amplitude = numAttr.create( "amplitude", "a", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"amplitude\" attribute" );
	numAttr.setChannelBox( true );			// length attribute appears in channel box
  // add attribute
  status = addAttribute( m_amplitude );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"amplitude\" attribute to SineNode" );

  // now the freq one
  m_frequency = numAttr.create( "frequency", "f", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"frequency\" attribute" );
  numAttr.setChannelBox( true );			// freq attribute appears in channel box
  // Add  attribute to node
  status = addAttribute( m_frequency );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"freq\" attribute to SineNode" );



  // create the output attribute
  m_output = numAttr.create( "output", "o",MFnNumericData::kDouble,0.0f);

  // make this attribute a read only output value
  numAttr.setReadable(true);
  numAttr.setKeyable(false);
  numAttr.setWritable(false);
  status = addAttribute( m_output );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"output\" attribute to SineNode" );

  attributeAffects(m_time,  m_output);
  attributeAffects(m_functionType,m_output);

	return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
// This method should be overridden in user defined nodes.
// Recompute the given output based on the nodes inputs.
// The plug represents the data value that needs to be recomputed, and the data block holds the storage
// for all of the node's attributes.
//----------------------------------------------------------------------------------------------------------------------
MStatus SineNode::compute( const MPlug &_plug , MDataBlock &_data )
{
  MStatus status;
  if( _plug == m_output)
	{
		// get data handle for length
    MDataHandle timeData = _data.inputValue( m_time , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for time plug" );

    // get data handle for function
    MDataHandle funcData = _data.inputValue( m_functionType , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for function plug" );



    MDataHandle outputData = _data.outputValue(m_output);
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for output plug" );

    // get the input value as a time value
    MTime Time = timeData.asTime();

    // get data handle for length
    MDataHandle dataHandle = _data.inputValue( m_frequency , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for freq plug" );
    // now get the value for the data handle as a double
    double freq = dataHandle.asDouble();
    dataHandle = _data.inputValue( m_amplitude , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for amplitude plug" );
    // now get the value for the data handle as a double
    double amp = dataHandle.asDouble();
    double out;
    switch(funcData.asInt())
    {
      case SINE :  out=amp*sin(freq*M_PI*Time.as(MTime::kSeconds)); break;
      case COS :   out=amp*cos(freq*M_PI*Time.as(MTime::kSeconds)); break;
      case COMPLEX :
                out=amp*cos(freq*M_PI*Time.as(MTime::kSeconds)) +
                    amp*sin(2*freq*M_PI*Time.as(MTime::kSeconds));
      break;
    }


    // set the output to the sine of the time (in seconds)
    outputData.set(out);
    // clean the output plug, ie unset it from dirty so that maya does not re-evaluate it
    _data.setClean( _plug );

    return MStatus::kSuccess;
	}

  return MStatus::kUnknownParameter;
}

//----------------------------------------------------------------------------------------------------------------------
void* SineNode::creator()
{
  return new SineNode();
}
//----------------------------------------------------------------------------------------------------------------------


