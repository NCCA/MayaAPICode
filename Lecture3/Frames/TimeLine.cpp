#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MIOStream.h>
#include <maya/MAnimControl.h>
#include <maya/MTime.h>
// This is a macro to create a simple command
// the compiler expands it to a bunch of code

DeclareSimpleCommand( TimeLine , "NCCA", "Any");

MStatus TimeLine::doIt( const MArgList& )
{

  MTime currframe;
  MAnimControl anim;
  // lets set the min time
  currframe.setValue(0);
  anim.setMinTime(currframe);
  // lets set the max time
  currframe.setValue(100);
  anim.setMaxTime(currframe);

  // now lets accesss this info
  char msg[100];
  snprintf(msg,100,"start frame =%f",anim.animationStartTime().value() );
  MGlobal::displayInfo(msg);
  snprintf(msg,100,"end frame =%f",anim.animationEndTime().value() );
  MGlobal::displayInfo(msg);
  int end=anim.animationEndTime().value();
  for(float i=0.0f; i<end; i+=0.1)
  {
    currframe.setValue(i);
    anim.setCurrentTime(currframe);
    snprintf(msg,100,"frame =%f",anim.currentTime().value() );
    MGlobal::displayInfo(msg);
  }

  return MS::kSuccess;
}

