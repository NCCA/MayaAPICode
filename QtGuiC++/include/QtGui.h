#ifndef CUSTOMSPHERE_H_
#define CUSTOMSPHERE_H_
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MPxCommand.h>

class SimpleDialog : public QWidget
{
  Q_OBJECT

  public :
    SimpleDialog()=default;
    SimpleDialog(QWidget* parent);
    virtual ~SimpleDialog()=default;
  private :
};


//----------------------------------------------------------------------------------------------------------------------
// @brief a simple maya command class
//----------------------------------------------------------------------------------------------------------------------

class QtGui : public MPxCommand
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  // virtual dtor
  //----------------------------------------------------------------------------------------------------------------------
  virtual		~QtGui();
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
  /// @brief our creator function called when the class is created
  /// the returns a new instance of this class
  //----------------------------------------------------------------------------------------------------------------------
  static void*	creator();

private:
	static QPointer<SimpleDialog>	s_widget;

};

//----------------------------------------------------------------------------------------------------------------------

#endif

