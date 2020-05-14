#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
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
  char msg[50];

  MGlobal::displayInfo("traversing DAG");
  for (dagIt.next(); !dagIt.isDone(); dagIt.next())
  {
    node = dagIt.item();
    nodeFn.setObject( node );
    if( node.hasFn(MFn::kNamedObject))
    {
      MGlobal::displayInfo("Object "+nodeFn.name()+ " type " + nodeFn.typeName());
      MGlobal::displayInfo("fullPathName "+nodeFn.fullPathName());
      MGlobal::displayInfo("partialPathName "+nodeFn.partialPathName());
      MGlobal::displayInfo("Parent path "+nodeFn.partialPathName());
      MGlobal::displayInfo("Parent namespace "+nodeFn.parentNamespace());
      sprintf(msg,"number of children= %d number of parents= %d ",nodeFn.childCount(),nodeFn.parentCount());
      MGlobal::displayInfo(msg);
      sprintf(msg,"attribute count = %d ",nodeFn.attributeCount());
      MGlobal::displayInfo(msg);
      MGlobal::displayInfo("Transformation Matrix");
      MMatrix m=nodeFn.transformationMatrix();
      sprintf(msg,"[%02.4lf,%02.4lf,%02.4lf,%02.4lf]",m(0,0),m(0,1),m(0,2),m(0,3));
      MGlobal::displayInfo( msg);
      sprintf(msg,"[%02.4lf,%02.4lf,%02.4lf,%02.4lf]",m(1,0),m(1,1),m(1,2),m(1,3));
      MGlobal::displayInfo( msg);
      sprintf(msg,"[%02.4lf,%02.4lf,%02.4lf,%02.4lf]",m(2,0),m(2,1),m(2,2),m(2,3));
      MGlobal::displayInfo( msg);
      sprintf(msg,"[%02.4lf,%02.4lf,%02.4lf,%02.4lf]",m(3,0),m(3,1),m(3,2),m(3,3));
      MGlobal::displayInfo( msg);

      MGlobal::displayInfo("*********************************************************************");
      ++count;
    }

  }
  sprintf(msg,"found %d objects",count);
  MGlobal::displayInfo(msg);

  return MS::kSuccess;
}

