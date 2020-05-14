import maya.cmds as cmds

cmds.createNode('sineNode')
cmds.connectAttr('time1.outTime','sineNode1.time')
cmds.polySphere()
cmds.connectAttr('sineNode1.o','pSphere1.tx')
cmds.connectAttr('sineNode1.o','pSphere1.ty')
cmds.select('sineNode1')