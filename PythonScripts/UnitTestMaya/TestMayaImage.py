#!/Applications/Autodesk/maya2019/Maya.app/Contents/bin/mayapy

import maya.standalone
import unittest
import MayaImage as mi

class TestMayaImage(unittest.TestCase):
  @classmethod
  def setUpClass(self):
    print('doing setup')

  def testConstructFromWidthHeight(self) :
    image=mi.MayaImage(width=100,height=200)
    self.assertEqual(image.width,100)
    self.assertEqual(image.height,200)
    
  def testThrowFromBadFilename(self) :
    with self.assertRaises(ValueError) :
      _=mi.MayaImage(200)
  def testThrowFromIndexError(self) :
    with self.assertRaises(IndexError) :
      img=mi.MayaImage(width=10,height=10)
      img.setPixel(200,100,255,255,0)
  def testSaveImage(self) :
    image=mi.MayaImage(width=100,height=100)
    for h in range(0,100) :
      for w in range(0,100) :
        image.setPixel(w,h,255,0,0)
    image.save("test","png")

  def testOpenImage(self) :
    self.testSaveImage()
    image=mi.MayaImage("test.png")
    r,g,b,_=image.getPixel(2,2)
    self.assertEqual(r,255)
    self.assertEqual(g,0)
    self.assertEqual(b,0)
    

if __name__ == '__main__' :
  maya.standalone.initialize(name='python')
  unittest.main()
  print('closing down maya-standalone')
  maya.standalone.uninitialize()