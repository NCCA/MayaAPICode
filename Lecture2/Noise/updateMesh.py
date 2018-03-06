import maya.cmds as cmds

cmds.createNode('noiseNode')
cmds.polyPlane(w=42,h=42,sx=80,sy=80)


def updateMesh() :
	nVerts=cmds.polyEvaluate('pPlane1',v=True)
	for i in range(0,nVerts) :
		a=cmds.pointPosition('pPlane1.vtx[%d]'%i )
		cmds.setAttr('noiseNode1.tx',a[0])
		cmds.setAttr('noiseNode1.tz',a[2])
		o=cmds.getAttr('noiseNode1.output')
		cmds.xform( 'pPlane1.vtx[%d]' %i ,a=True,ws=True,t=( a[0], o, a[2]) )



