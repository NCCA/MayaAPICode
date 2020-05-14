from __future__ import print_function,division
import maya.cmds as cmds

# lets delete all objects
cmds.select(all=True)
cmds.delete()
#lets create a nurb's cube
cmds.nurbsCube(w=3, name='Cube1')
cmds.getAttr('Cube1.tx')
cmds.getAttr('Cube1.ty')
cmds.getAttr('Cube1.tz')

# let's make sure 'Cube1' is selected
cmds.select('Cube1', replace=True)
#let's change its translation attributes
cmds.setAttr('Cube1.tx', 1)
cmds.setAttr('Cube1.ty', 2)
cmds.setAttr('Cube1.tz', 3)

