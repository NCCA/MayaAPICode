#include "TestNode.h"
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
MTypeId TestNode::m_id( 0x70003 );		// numeric Id of node
const MString TestNode::typeName( "TestNode" );
MObject TestNode::m_output;
MObject TestNode::m_tx;


//----------------------------------------------------------------------------------------------------------------------

TestNode::~TestNode()
{

  MGlobal::displayInfo("Node dtor called");

}

//----------------------------------------------------------------------------------------------------------------------
// this method creates the attributes for our node and sets some default values etc
//----------------------------------------------------------------------------------------------------------------------
MStatus	TestNode::initialize()
{

  MStatus status;
  MGlobal::displayInfo("Node initialize called");


  // now we are going to add several number attributes
  MFnNumericAttribute	numAttr;

  // now the position inputs
  m_tx = numAttr.create( "translateX", "tx", MFnNumericData::kDouble, 1.0, &status );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to create \"x pos \" attribute" );
  // Add  attribute to node
  status = addAttribute( m_tx );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"tx\" attribute to TestNode" );


  // create the output attribute
  m_output = numAttr.create( "output", "out",MFnNumericData::kDouble,0.0f);

  // make this attribute a read only output value
  numAttr.setReadable(true);
  numAttr.setKeyable(false);
  status = addAttribute( m_output );
  CHECK_STATUS_AND_RETURN_MSTATUS_IF_FAIL( status , "Unable to add \"output\" attribute to TestNode" );
  attributeAffects(m_tx,m_output);
  // report all was good
  return MStatus::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
// This method should be overridden in user defined nodes.
// Recompute the given output based on the nodes inputs.
// The plug represents the data value that needs to be recomputed, and the data block holds the storage
// for all of the node'_scale attributes.
//----------------------------------------------------------------------------------------------------------------------
MStatus TestNode::compute( const MPlug &_plug , MDataBlock &_data )
{
  MStatus status;
  MGlobal::displayInfo("Node compute called");

  // see if we get the output plug
  if( _plug == m_output)
	{

    return MStatus::kSuccess;
	}


  return MStatus::kUnknownParameter;
}


//----------------------------------------------------------------------------------------------------------------------
TestNode::TestNode()
{
  MGlobal::displayInfo("Node ctor called");

}


//----------------------------------------------------------------------------------------------------------------------
void* TestNode::creator()
{
  MGlobal::displayInfo("Node creator called");

  return new TestNode();
}
//----------------------------------------------------------------------------------------------------------------------


