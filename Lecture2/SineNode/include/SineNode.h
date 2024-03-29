#ifndef SINE_NODE_H_
#define SINE_NODE_H_
#ifndef WIN32
  #define LINUX
#endif
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MFnDependencyNode.h>
//----------------------------------------------------------------------------------------------------------------------
/// @brief a simple node that takes an input value (time) and creates a sin output value based on two parameters
//----------------------------------------------------------------------------------------------------------------------

class SineNode : public MPxNode
{

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the creator node is called when the plugin is created
  //----------------------------------------------------------------------------------------------------------------------
  static void		*creator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief initialize called when plug is created
  //----------------------------------------------------------------------------------------------------------------------
  static MStatus		initialize();
  ///----------------------------------------------------------------------------------------------------------------------
  // @brief compute is called every time the attributes of the class change
  /// @param [in] 	_plug 	plug representing the attribute that needs to be recomputed
  /// @param [in] 	_block 	data block containing storage for the node's attributes
  //----------------------------------------------------------------------------------------------------------------------
  virtual MStatus		compute(const MPlug &_plug , MDataBlock &_block );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the id of this plugin must be public so we can set outside of class
  //----------------------------------------------------------------------------------------------------------------------
  static MTypeId 		m_id;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the unique type name of our custom node. Mainly for mel purposes.
  /// must be public so maya can access
  //----------------------------------------------------------------------------------------------------------------------
  static const MString m_typeName;

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the amplitude of our node
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_amplitude;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the frequency of our node
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_frequency;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the input to our node
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_time;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the output to our node
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_output;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the use sine cose etc to our node
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_functionType;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief enum of different function types
  //----------------------------------------------------------------------------------------------------------------------
  enum FunctionTypes{SINE,COS,COMPLEX};

};

#endif
//----------------------------------------------------------------------------------------------------------------------


