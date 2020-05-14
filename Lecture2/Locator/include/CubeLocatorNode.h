#ifndef CUBENODE_H_
#define CUBENODE_H_

#define LINUX

#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/M3dView.h>
#include <maya/MGlobal.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>





//----------------------------------------------------------------------------------------------------------------------
/// @brief a simple maya locator node using OpenGL to draw a cube with width / height / depth
/// paramaters. Also the volume of the cube is created each time an attribute is changed
//----------------------------------------------------------------------------------------------------------------------

class CubeLocatorNode : public MPxLocatorNode
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
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief compute is called every time the attributes of the class change
  /// @param [in] 	_plug 	plug representing the attribute that needs to be recomputed
  /// @param [in] 	_block 	data block containing storage for the node's attributes
  //----------------------------------------------------------------------------------------------------------------------
  virtual MStatus		compute(
                             const MPlug &_plug ,
                              MDataBlock &_block
                            );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called when the display is updated
  /// @param [in] 	_view 	3D view that is being drawn into
  /// @param [in] 	_path 	to this locator in the DAG
  /// @param [in] 	_style 	style to draw object in
  /// @param [in] 	_status 	selection status of object
  //----------------------------------------------------------------------------------------------------------------------
  virtual void 		draw(
                        M3dView & _view ,
                        const MDagPath & _path ,
                        M3dView::DisplayStyle _style,
                        M3dView::DisplayStatus _status
                        );


  MFloatPoint getDimensions() const ;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the id of this plugin must be public so we can set outside of class
  //----------------------------------------------------------------------------------------------------------------------
  static MTypeId 		s_id;
  static MString		s_drawDbClassification;
  static MString		s_drawRegistrantId;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the unique type name of our custom node. Mainly for mel purposes.
  /// must be public so maya can access
  //----------------------------------------------------------------------------------------------------------------------
  static const MString m_typeName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the width of our locator cube this can be changed by the user
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_width;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the height of our locator cube this can be changed by the user
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_height;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the depth of our locator cube this can be changed by the user
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_depth;

private :

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the volume of our locator cube this is calculated each time the attributes
  /// above are changed
  //----------------------------------------------------------------------------------------------------------------------
  static MObject m_volume;

};







#endif
//----------------------------------------------------------------------------------------------------------------------


