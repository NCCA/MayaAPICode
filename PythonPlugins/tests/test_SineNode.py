import pytest

import sys,os
import maya.standalone
import SineNode

def setup_module(module):
    maya.standalone.initialize(name='python')
    import maya.cmds as cmds
    # add the current directory to the MAYA_PLUG_IN_PATH
    print('initializing maya-standalone')

def teardown_module(module):
  print('closing down maya-standalone')
  maya.standalone.uninitialize()


def test_SineNode():
  # Create a sphere
  #
  import maya.cmds as cmds
  print("loading plugin")
  cmds.loadPlugin("SineNode.py")
  cmds.createNode("SineNodePy",name="TestSineNode")
  results = cmds.ls("TestSineNode")
  assert len(results) == 1