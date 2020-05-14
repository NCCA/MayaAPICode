####################################################################################
# This file is split into Three sections
# The first configures Qt and the source files for all platforms
# The second is the linux build
# The third the mac build
# (if your using windows you will need to add a fourth one!)
# first lets remove Qt core and gui not going to need it
####################################################################################
QT       -= core gui
####################################################################################
# This is the name of the plugin / final lib file
####################################################################################
TARGET = testNode
####################################################################################
# for for mac we need a bundle so change the name
####################################################################################
macx:TARGET=testNode.bundle
####################################################################################
# here we add the source files (and headers if required)
####################################################################################
SOURCES+=src/TestNode.cpp \
         src/Plugin.cpp
HEADERS+=include/TestNode.h


OBJECTS_DIR = obj
INCLUDEPATH+=include
DEPENDPATH+=include
# these are defines required by Maya to re-define some C++
# stuff, we will add some more later to tell what platform
# we are on as well
DEFINES+=REQUIRE_IOSTREAM \
         _BOOL
####################################################################################
# These are the maya libs we need to link to, this will change depending
# upon which maya framework we use, just add them to the end of
# this list as required and they will be added to the build
####################################################################################
MAYALIBS=-lOpenMaya \
        -lFoundation \
        -lOpenMayaUI
####################################################################################
# these are all the libs usually included by mayald in case you need
# them just add them to the list above and make sure you escape
####################################################################################
#-lOpenMayalib \
#-lOpenMaya \
#-lAnimSlice \
#-lDeformSlice \
#-lModifiers \
#-lDynSlice \
#-lKinSlice \
#-lModelSlice \
#-lNurbsSlice \
#-lPolySlice \
#-lProjectSlice \
#-lImage \
#-lShared \
#-lTranslators \
#-lDataModel \
#-lRenderModel \
#-lNurbsEngine \
#-lDependEngine \
#-lCommandEngine \
#-lFoundation \
#-lIMFbase \
#-lm -ldl
####################################################################################
# now tell linux we need to build a lib
####################################################################################
linux-g++-64:TEMPLATE = lib
####################################################################################
# this tells qmake where maya is
####################################################################################
linux-g++-64:MAYALOCATION=/usr/autodesk/maya2011-x64/
####################################################################################
# under linux we need to use the version of g++ used to build maya
# in this case g++412
####################################################################################
linux-g++-64:QMAKE_CXX = g++412
####################################################################################
# set the include path for linux
####################################################################################
linux-g++-64:INCLUDEPATH += $$MAYALOCATION/include \
                        /usr/X11R6/include
####################################################################################
# set which libs we need to include
####################################################################################
linux-g++-64:LIBS += -L$$MAYALOCATION/lib \
                   $$MAYALIBS
####################################################################################
# tell maya we're building for linux
####################################################################################
linux:DEFINES+=linux
####################################################################################
# tell maya we're building for Mac
####################################################################################
macx:DEFINES+=OSMac_
macx:MAYALOCATION=/Applications/Autodesk/maya2011
macx:CONFIG -= app_bundle
macx:INCLUDEPATH+=$$MAYALOCATION/devkit/include
macx:LIBS+= -framework OpenGL
####################################################################################
# under mac we need to build a bundle, to do this use
# the -bundle flag but we also need to not use -dynamic lib so
# remove this
####################################################################################
macx:LIBS +=-bundle
mac:LIBS -=-dynamiclib
####################################################################################

####################################################################################
macx:LIBS += -L$$MAYALOCATION/Maya.app/Contents/MacOS \
             $$MAYALIBS
####################################################################################
