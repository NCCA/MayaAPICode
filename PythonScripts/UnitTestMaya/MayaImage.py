import maya.api.OpenMaya as OpenMaya
import ctypes


class MayaImage :
  def __init__(self,filename=None,width=None,height=None,channels=4,imagetype=OpenMaya.MImage.kByte) :
    """ constructor pass in the name of the file to load (absolute file name with path)
    or width and height to create and image
    """
    if filename != None and isinstance(filename,str) :
      self.filename=filename
    elif (width !=None and height !=None) :
      try :
        self.width=int(width)
        self.height=int(height)
      except ValueError :
        print('error converting values to int')
        exit()
      print('creating empty image {}x{}'.format(width,height))
    else :
      raise ValueError('ImageError trouble converting arguments')
    # create an MImage object
    self.image=OpenMaya.MImage()
    self.channels=channels
    self.type=imagetype
    if filename !=None :
      self.readImage(filename)
    else :
      self.image.create(self.width,self.height,self.channels,self.type)
      self.charPixelPtr = ctypes.cast(self.image.pixels(), ctypes.POINTER(ctypes.c_char) )
      self.width,self.height=self.image.getSize()
      

  def readImage(self,filename) :
    # read from file MImage should handle errors for us so no need to check
    self.image.readFromFile(filename)
    self.width,self.height=self.image.getSize()
    # get the pixel data
    self.charPixelPtr = ctypes.cast(self.image.pixels(), ctypes.POINTER(ctypes.c_char) )
    # query to see if it's an RGB or RGBA image, this will be True or False
    self.hasAlpha=self.image.isRGBA()

  def setPixel(self,x,y,r,g,b,a=255) :
    # check the bounds to make sure we are in the correct area
    if x<0 or x>self.width :
      raise IndexError('x value out of range')
    if y<0 or y>self.height :
      raise IndexError('y value out of range')
    # now calculate the index into the 1D array of data
    index=(y*self.width*self.channels)+x*self.channels
    # grab the pixels we need to convert from char to integer ordinal type so use ord
    self.charPixelPtr[index]=ctypes.c_char(chr(r))
    self.charPixelPtr[index+1]=ctypes.c_char(chr(g))
    self.charPixelPtr[index+2]=ctypes.c_char(chr(b))
    self.charPixelPtr[index+3]=ctypes.c_char(chr(a))

  def save(self,filename,outputFormat='png') :
    self.image.writeToFile(filename+'.'+outputFormat,outputFormat=outputFormat)
    
  def getPixel(self,x,y) :
    """ get the pixel data at x,y and return a 3/4 tuple depending upon type """
    # check the bounds to make sure we are in the correct area
    if x<0 or x>self.width :
      print "error x out of bounds\n"
      return
    if y<0 or y>self.height :
      print "error y our of bounds\n"
      return
    # now calculate the index into the 1D array of data
    index=(y*self.width*self.channels)+x*self.channels
    # grab the pixels we need to convert from char to integer ordinal type so use ord
    red   = ord(self.charPixelPtr[index])
    green = ord(self.charPixelPtr[index+1])
    blue  = ord(self.charPixelPtr[index+2])
    alpha = ord(self.charPixelPtr[index+3])
    #print type(red),green,blue,alpha
    return (red,green,blue,alpha)

  def getRGB(self,x,y) :
    r,g,b,_=self.getPixel(x,y)
    return (r,g,b)





