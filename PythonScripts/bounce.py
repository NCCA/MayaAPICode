import maya.cmds as cmds

cmds.polySphere(name='ball', radius=2)

g = -0.04
yVelocity = 0.8
xVelocity = 0.4
initialYPos = 6
initialXPos = 0
cmds.move(initialXPos,initialYPos,0,"ball")
posy=initialYPos
timeMult=4
for bounces in range(0,6):
	time=0
	posy=initialYPos
	while posy>0 :
		posy = initialYPos + yVelocity*(time-1) + g*(time-1)*(time-1)/2
		posx = initialXPos + xVelocity*((time) + time-1)
		cmds.setKeyframe( 'ball', attribute='translateY', value=posy, t=time*timeMult )
		cmds.setKeyframe( 'ball', attribute='translateX', value=posx, t=time*timeMult )
		time+=1

	yVelocity-=0.1