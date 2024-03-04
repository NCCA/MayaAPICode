import pytest

import sys,os
import maya.standalone
import CustomSphere

def setup_module(module):
    maya.standalone.initialize(name='python')
    import maya.cmds as cmds
    # add the current directory to the MAYA_PLUG_IN_PATH
    print('initializing maya-standalone')

def teardown_module(module):
  print('closing down maya-standalone')
  maya.standalone.uninitialize()


def test_CustomSphere():
    # Create a sphere
    #
    import maya.cmds as cmds
    print("loading plugin")
    cmds.loadPlugin("CustomSphere.py")
    cmds.CustomSpherePy(100) 
    results = cmds.ls("sphere*",type="shape")
    assert len(results) == 100
    cmds.undo()
    results = cmds.ls("sphere*",type="shape")
    assert len(results) == 0
    