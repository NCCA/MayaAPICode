#include "NoiseNode.h"
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
// as these are static class attributes we need to set them here so the static methods can see them
// in particular see  http://www.tutorialspoint.com/cplusplus/cpp_static_members.htm
// "By declaring a function member as static, you make it independent of any particular object of the class.
// A static member function can be called even if no objects of the class exist and the static functions are accessed using only
// the class name and the scope resolution operator ::.
// A static member function can only access static data member, other static member
// functions and any other functions from outside the class.
// Static member functions have a class scope and they do not have access
// to the this pointer of the class. You could use a static member function to determine whether some
// objects of the class have been created or not."
//----------------------------------------------------------------------------------------------------------------------
MTypeId NoiseNode::m_id( 0x70003 );		// numeric Id of node
const MString NoiseNode::typeName( "noiseNode" );
MObject NoiseNode::m_amplitude;
MObject NoiseNode::m_output;
MObject NoiseNode::m_tx;
MObject NoiseNode::m_ty;
MObject NoiseNode::m_tz;
MObject NoiseNode::m_noiseType;
MObject NoiseNode::m_scale;
MObject NoiseNode::m_seed;
MObject NoiseNode::m_steps;
MObject NoiseNode::m_persistence;



//----------------------------------------------------------------------------------------------------------------------

NoiseNode::~NoiseNode()
{

}

//----------------------------------------------------------------------------------------------------------------------
// this method creates the attributes for our node and sets some default values etc
//----------------------------------------------------------------------------------------------------------------------
MStatus	NoiseNode::initialize()
{

  MStatus status;
  // in this case we have an enum attribute (combo box) this is used for the different noise types
  MFnEnumAttribute    enumAttr;
  // first we create our attribute and set it to the m_noiseType object
  // note that f is used later as the shortcut for frequency so we use ft
  m_noiseType = enumAttr.create( "noiseType", "nt", 0, &status );
  // now we check that it was ok
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create noise Type attribute" );
  // now set the different combo box values
  enumAttr.addField( "Noise", NOISE );
  enumAttr.addField( "Turbulance", TURBULANCE );
  enumAttr.addField("Complex", COMPLEX);
  // set this so it can be keyable
  enumAttr.setKeyable( true );
  // finally add this to the node
  status = addAttribute( m_noiseType );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add noiseType attribute to noiseNode" );

  // now we are going to add several number attributes
  MFnNumericAttribute	numAttr;
  // this code is quite repetative so but fairly standard
  m_seed = numAttr.create( "seed", "se", MFnNumericData::kInt, 1, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create seed attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box
  // add attribute
  numAttr.setReadable(true);
  // make sure it worked
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "failed to add user callback" );
  // add to node
  status = addAttribute( m_seed );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add seed attribute to NoiseNode" );

  // amplitude
  m_amplitude = numAttr.create( "amplitude", "amp", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"amplitude\" attribute" );
	numAttr.setChannelBox( true );			// length attribute appears in channel box
  // add attribute
  status = addAttribute( m_amplitude );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"amplitude\" attribute to NoiseNode" );
  // scale
  m_scale = numAttr.create( "scale", "sc", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create scale attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box
  // add attribute
  status = addAttribute( m_scale );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add scale attribute to NoiseNode" );

  // steps is an integer so set as kInt
  m_steps = numAttr.create( "step", "stp", MFnNumericData::kInt, 4, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create steps attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box
  // here we set the min and max values
  numAttr.setMin(1);
  numAttr.setMax(12);
  // add attribute
  status = addAttribute( m_steps );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add steps attribute to NoiseNode" );
  // another float
  m_persistence = numAttr.create( "persistence", "per", MFnNumericData::kDouble, 0.25, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to pers steps attribute" );
  numAttr.setChannelBox( true );			// length attribute appears in channel box
  // add attribute
  status = addAttribute( m_persistence );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add steps attribute to NoiseNode" );
  // we don't want these to be visible so use the setHidden flag to false
  // now the position inputs
  m_tx = numAttr.create( "translateX", "tx", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"x pos \" attribute" );
  // Add  attribute to node
  numAttr.setHidden(true);
  status = addAttribute( m_tx );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"tx\" attribute to NoiseNode" );
  // now the position inputs
  m_ty = numAttr.create( "translateY", "ty", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"y pos \" attribute" );
  // Add  attribute to node
  status = addAttribute( m_ty );
  numAttr.setHidden(true);
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"ty\" attribute to NoiseNode" );

  // now the position inputs
  m_tz = numAttr.create( "translateZ", "tz", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"z pos \" attribute" );
  // Add  attribute to node
  status = addAttribute( m_tz );
  numAttr.setHidden(true);
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"tz\" attribute to NoiseNode" );


  // create the output attribute
  m_output = numAttr.create( "output", "out",MFnNumericData::kDouble,0.0f);

  // make this attribute a read only output value
  numAttr.setReadable(true);
  numAttr.setKeyable(false);
  status = addAttribute( m_output );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"output\" attribute to NoiseNode" );
  // this links the different elements together forcing a re-compute each time the values are changed
  attributeAffects(m_noiseType,m_output);
  attributeAffects(m_amplitude,m_output);
  attributeAffects(m_tx,m_output);
  attributeAffects(m_ty,m_output);
  attributeAffects(m_tz,m_output);
  attributeAffects(m_scale,m_output);
  attributeAffects(m_seed,m_output);
  // report all was good
  return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
// This method should be overridden in user defined nodes.
// Recompute the given output based on the nodes inputs.
// The plug represents the data value that needs to be recomputed, and the data block holds the storage
// for all of the node'_scale attributes.
//----------------------------------------------------------------------------------------------------------------------
MStatus NoiseNode::compute( const MPlug &_plug , MDataBlock &_data )
{
  MStatus status;
  // as this is static we can't call non static methods, in this case we only want to re-set the seed
  // but this is expensive (re-builds the tables in the noise node) so we only do this when the seed
  // value changes. To do this, set the s_seeValue variable to the default seed value 1. here
  // then if this is different from the m_seed value from the form we update the tables and re-set this
  // as this is static value is retained between calls (an elegant hack ;0)

  static uint s_seedValue=1;

  // see if we get the output plug
  if( _plug == m_output)
	{

    MDataHandle outputData = _data.outputValue(m_output);
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for output plug" );

    // get data handle for values
    MDataHandle dataHandle = _data.inputValue( m_tx , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for tx plug" );
    // now get the value for the data handle as a double
    double tx = dataHandle.asDouble();

    dataHandle = _data.inputValue( m_ty , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for ty plug" );
    // now get the value for the data handle as a double
    double ty = dataHandle.asDouble();

    dataHandle = _data.inputValue( m_tz , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for tz plug" );
    // now get the value for the data handle as a double
    double tz = dataHandle.asDouble();


    dataHandle = _data.inputValue( m_amplitude , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for amplitude plug" );
    // now get the value for the data handle as a double
    double amp = dataHandle.asDouble();

    dataHandle = _data.inputValue( m_scale , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for scale plug" );
    // now get the value for the data handle as a double
    double scale = dataHandle.asDouble();

    dataHandle = _data.inputValue( m_steps , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for step plug" );
    // now get the value for the data handle as a double
    int steps = dataHandle.asInt();

    dataHandle = _data.inputValue( m_persistence , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for persistence plug" );
    // now get the value for the data handle as a double
    double persistence = dataHandle.asDouble();


    dataHandle = _data.inputValue( m_seed , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for scale plug" );
    // now get the value for the data handle as a double
    unsigned int seed = dataHandle.asInt();
    // see if the seed has changed and re-set the table (see note at top)
    if(seed !=s_seedValue)
    {
      MGlobal::displayInfo("re-setting to new seed");
      s_seedValue=seed;
      m_noise->setSeed(seed);
      m_noise->resetTables();
    }

    // get data handle for type and set the output value
    MDataHandle noiseType = _data.inputValue( m_noiseType , &status );
    CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to get data handle for function plug" );
    double out;

    switch(noiseType.asShort())
    {
      case NOISE :
          out=m_noise->noise(scale,MPoint(tx,ty,tz))/amp;
      break;

      case TURBULANCE :
        out=m_noise->turbulance(scale,MPoint(tx,ty,tz))/amp;
      break;

    case COMPLEX :
      out=m_noise->complex(steps,persistence,scale,MPoint(tx,ty,tz))/amp;

    break;
    }


     outputData.set(out);
    // clean the output plug, ie unset it from dirty so that maya does not re-evaluate it
    _data.setClean( _plug );

    return MStatus::kSuccess;
	}


  return MStatus::kUnknownParameter;
}


//----------------------------------------------------------------------------------------------------------------------
NoiseNode::NoiseNode()
{
  MGlobal::displayInfo("constructing new noise");
  m_noise.reset(new  Noise());
  m_seedValue=1;
}


//----------------------------------------------------------------------------------------------------------------------
void* NoiseNode::creator()
{
  return new NoiseNode();
}
//----------------------------------------------------------------------------------------------------------------------


