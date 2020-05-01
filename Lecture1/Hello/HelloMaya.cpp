#include <maya/MSimple.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
// This is a macro to create a simple command
// the compiler expands it to a bunch of code

DeclareSimpleCommand( HelloMaya , "NCCA", "Maya 2020");

MStatus HelloMaya::doIt( const MArgList& )
{
  std::cout<<"This should come from the shell\n";
  MGlobal::displayInfo("Hello Maya in the maya command shell");
  MGlobal::displayWarning("This should be a warning");
  MGlobal::displayError("This should be an error");

  return MS::kSuccess;
}

