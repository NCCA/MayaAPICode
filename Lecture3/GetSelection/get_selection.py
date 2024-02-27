import maya.api.OpenMaya as OM

node_fn = OM.MFnDagNode()
list = OM.MGlobal.getActiveSelectionList()
for i in range(0, list.length()):
    path = list.getDagPath(i)
    node_fn.setObject(path)
    print(f"Object {node_fn.name()} is selected")


dagIt = OM.MItDag(OM.MItDag.kBreadthFirst)
while not dagIt.isDone():
    node = dagIt.currentItem()
    node_fn.setObject(node)
    if node.hasFn(OM.MFn.kNamedObject):
        print(f"Object {node_fn.name()} type {node_fn.typeName}")
        m = node_fn.transformationMatrix()
        print(m)
    dagIt.next()
