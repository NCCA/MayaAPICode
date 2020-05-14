from __future__ import print_function,division
import maya.cmds as cmds

# let's create a sphere
cmds.sphere(radius=2, name='Sphere1')

# query the radius of the sphere named Sphere1
radius = cmds.sphere('Sphere1', query=True, radius=True)
print('The sphere radius =',radius)

# modify the radius
print('Increment the sphere\'s radius by one unit')
cmds.sphere('Sphere1', edit=True, radius=radius+1)
radius = cmds.sphere('Sphere1', query=True, radius=True)
print ('The new sphere radius =', radius)
