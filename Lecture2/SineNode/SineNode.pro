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
TARGET = sineNode
####################################################################################
# for for mac we need a bundle so change the name
####################################################################################
macx:TARGET=sineNode.bundle
####################################################################################
# here we add the source files (and headers if required)
####################################################################################
SOURCES+=src/SineNode.cpp \
         src/Plugin.cpp
HEADERS+=include/SineNode.h
OTHER_FILES+=createSineBall.py
OBJECTS_DIR = obj
INCLUDEPATH+=include
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
linux-*:TEMPLATE = lib
####################################################################################
# this tells qmake where maya is
####################################################################################
linux-*:MAYALOCATION=/opt/autodesk/maya/
# and now the devkit is not part of maya where to find it
# in the Uni I have it in /public/devel/mayaDevkit
linux-*:DEVKITLOCATION=/public/devel/mayaDevkit
####################################################################################
# under linux we need to use the version of g++ used to build maya
# in this case g++412
####################################################################################
#linux-g++-64:QMAKE_CXX = g++412
####################################################################################
# set the include path for linux
####################################################################################
linux-*:INCLUDEPATH += $$DEVKITLOCATION/include \
                        /usr/X11R6/include
####################################################################################
# set which libs we need to include
####################################################################################
linux-*:LIBS += -L$$MAYALOCATION/lib \
                   $$MAYALIBS
####################################################################################
# tell maya we're building for linux
####################################################################################
linux:DEFINES+=linux
####################################################################################
# tell maya we're building for Mac
####################################################################################
macx:DEFINES+=OSMac_
macx:MAYALOCATION=/Applications/Autodesk/maya2016
macx:CONFIG -= app_bundle
macx:INCLUDEPATH+=$$MAYALOCATION/devkit/include
macx:LIBS+= -framework OpenGL
macx:INCLUDEPATH+=/opt/X11/include
macx:QMAKE_MAC_SDK = macosx10.12

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
