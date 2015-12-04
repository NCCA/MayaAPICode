#ifndef CUSTOMSPHERE_H__
#define CUSTOMSPHERE_H__

#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

//----------------------------------------------------------------------------------------------------------------------
// @brief a simple maya command class
//----------------------------------------------------------------------------------------------------------------------

class CustomSphere : public MPxCommand
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  // virtual dtor
  //----------------------------------------------------------------------------------------------------------------------
  virtual		~CustomSphere();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the doIt command is called everytime the command is executed in the maya shell
  /// @param _args the command arguments passed when command is run
  /// @note from the maya docs
  /// The doIt method should collect whatever information is
  /// required to do the task, and store it in local class data.  It
  /// should finally call redoIt to make the command happen.  The
  /// redoIt method should do the actual work, using only the local
  ///  class data.  The undoIt method should undo the actual work
  /// again using only the local class data.
  //----------------------------------------------------------------------------------------------------------------------
  MStatus		doIt( const MArgList& _args );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief does the actual commands work
  //----------------------------------------------------------------------------------------------------------------------
  MStatus		redoIt();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief undo what was done by the class using local data only
  //----------------------------------------------------------------------------------------------------------------------
  MStatus		undoIt();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief tell of the class is undoable (in this case true)
  //----------------------------------------------------------------------------------------------------------------------
  bool		isUndoable() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief our creator function called when the class is created
  /// the returns a new instance of this class
  //----------------------------------------------------------------------------------------------------------------------
  static void*	creator();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of spheres created
  //----------------------------------------------------------------------------------------------------------------------
  int		m_count;

};

//----------------------------------------------------------------------------------------------------------------------

#endif

