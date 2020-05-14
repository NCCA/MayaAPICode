from __future__ import print_function,division
import pymel.core as pm

pm.sphere(radius=2, name='Sphere1')

# query the radius of the sphere named Sphere1
radius = pm.sphere('Sphere1', query=True, radius=True)
print ('The sphere radius =',radius)

# modify the radius
print ('Increment the sphere\'s radius by one unit')
pm.sphere('Sphere1', edit=True, radius=radius+1)
radius = pm.sphere('Sphere1', query=True, radius=True)
print ('The new sphere radius =', radius)
