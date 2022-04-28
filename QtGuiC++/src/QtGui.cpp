#include "QtGui.h"
#include <maya/MString.h>
//----------------------------------------------------------------------------------------------------------------------
/// @brief simple macro to check status and return if error
/// originally written by Sola Aina
//----------------------------------------------------------------------------------------------------------------------
#define CHECK_STATUS_AND_RETURN_IF_FAIL( status , message )				\
  if( !status )									\
  {										\
    MString errorString = status.errorString() +  MString( message);	\
    MGlobal::displayError( errorString );					\
    return MStatus::kFailure;						\
  }										\

//----------------------------------------------------------------------------------------------------------------------


SimpleDialog::SimpleDialog(QWidget* parent)
{
  
}


QPointer<SimpleDialog>	QtGui::s_widget;


QtGui::~QtGui()
{
  // as we have no dynamic allocation we don't need to do anything here
}

//----------------------------------------------------------------------------------------------------------------------

MStatus QtGui::doIt( const MArgList& _args )
{
		return MStatus::kFailure;
}



void* QtGui::creator()
{
  return new QtGui();
}

//----------------------------------------------------------------------------------------------------------------------

