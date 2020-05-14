import maya.OpenMaya as OM

# as these can take time to process we have an interupter to allow for the process to be
# stopped
interupter=OM.MComputation()
# set the start of the heavy computation
interupter.beginComputation()

while  True :
  if interupter.isInterruptRequested()  :
    print "intrupted by escape"
    interupter.endComputation();
    quit=True
    break
  for i in range(0,999):
    print '.',

