#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

// This is a macro to create a simple command
// the compiler expands it to a bunch of code

DeclareSimpleCommand( GetSelection , "NCCA", "Any");

MStatus GetSelection::doIt( const MArgList& )
{

  MDagPath node;
  MObject component;
  MSelectionList list;
  MFnDagNode nodeFn;
  // get the currently selected objects
  MGlobal::getActiveSelectionList( list );
  // get the size of the list
  uint size=list.length();
  for (uint  i=0; i< size; ++i )
  {
    // get the DAG path for this object and place the node and component values
    list.getDagPath( i, node, component );
    // attach the found node to the DagNode function set
    nodeFn.setObject( node );
    // now use this to get the object name and display
    MGlobal::displayInfo("Object "+nodeFn.name()+ " is selected");
  }
  return MS::kSuccess;
}












