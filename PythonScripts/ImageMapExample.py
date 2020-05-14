import maya.OpenMaya as om
import maya.cmds as cmds



basicFilter = "*.*"

imageFile=cmds.fileDialog2(caption="Please select imagefile",
													fileFilter=basicFilter, fm=1)


img=MayaImage(str(imageFile[0]))
print img.width()
print img.height()
xoffset=-img.width()/2
yoffset=-img.height()/2

for y in range (0,img.height()) :
	for x in range(0,img.width()) :
		r,g,b,a=img.getPixel(x,y)
		if r > 10 :
			cmds.polyCube(h=float(r/10))
			cmds.move(xoffset+x,float(r/10)/2,yoffset+y)
