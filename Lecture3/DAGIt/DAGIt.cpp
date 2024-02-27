#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>
#include <maya/MItDag.h>
#include <maya/MMatrix.h>

// This is a macro to create a simple command
// the compiler expands it to a bunch of code

DeclareSimpleCommand( DAGit , "NCCA", "Any");

MStatus DAGit::doIt( const MArgList& )
{

  MItDag dagIt (MItDag::kBreadthFirst);
  MFnDagNode nodeFn;
  MObject node;
  uint count=0;
  MString message;
  // numeric arguments are harder to use, we can format using the following
  // MStringArray and set the arguments we need in the message string
  // using format. We have 4 arguments max with a matrix but we can use less if we want
  MStringArray args(4,"");
  MGlobal::displayInfo("traversing DAG");
  for (dagIt.next(); !dagIt.isDone(); dagIt.next())
  {
    // note item() is now deprecated so we use currentItem()
    node = dagIt.currentItem();
    nodeFn.setObject( node );
    if( node.hasFn(MFn::kNamedObject))
    {
      // for simple string objects formatting can be done with + 
      MGlobal::displayInfo("Object "+nodeFn.name()+ " type " + nodeFn.typeName());
      MGlobal::displayInfo("fullPathName "+nodeFn.fullPathName());
      MGlobal::displayInfo("partialPathName "+nodeFn.partialPathName());
      MGlobal::displayInfo("Parent path "+nodeFn.partialPathName());
      MGlobal::displayInfo("Parent namespace "+nodeFn.parentNamespace());
      // numeric arguments need conversion so we use the args string array.
      args[0].set(nodeFn.childCount());
      args[1].set(nodeFn.parentCount());
      // followed by the format with the ^1s string marker for the first argument 
      // and the ^2s string marker for the second argument etc.
      message.format("number of children= ^1s number of parents= ^2s ",args);     
      MGlobal::displayInfo(message);

      args[0].set(nodeFn.attributeCount());
      message.format("attribute count = ^1s ",args);     
      MGlobal::displayInfo(message);
      MGlobal::displayInfo("Transformation Matrix");
      MMatrix m=nodeFn.transformationMatrix();
      // set row 1 args
      args[0].set(m(0,0));
      args[1].set(m(0,1));
      args[2].set(m(0,2));
      args[3].set(m(0,3));
      message.format("[^1s,^2s,^3s,^4s]",args);
      MGlobal::displayInfo(message);
      // set row 2 args
      args[0].set(m(1,0));
      args[1].set(m(1,1));
      args[2].set(m(1,2));
      args[3].set(m(1,3));
      message.format("[^1s,^2s,^3s,^4s]",args);
      MGlobal::displayInfo(message);
      // set row 3 args
      args[0].set(m(2,0));
      args[1].set(m(2,1));
      args[2].set(m(2,2));
      args[3].set(m(2,3));
      message.format("[^1s,^2s,^3s,^4s]",args);
      MGlobal::displayInfo(message);
      // set row 4 args
      args[0].set(m(3,0));
      args[1].set(m(3,1));
      args[2].set(m(3,2));
      args[3].set(m(3,3));
      message.format("[^1s,^2s,^3s,^4s]",args);
      MGlobal::displayInfo(message);

      MGlobal::displayInfo("*********************************************************************");
      ++count;
    }

  }
  args[0].set(count);
  message.format("found ^1s objects",args);
  MGlobal::displayInfo(message);

  return MS::kSuccess;
}

