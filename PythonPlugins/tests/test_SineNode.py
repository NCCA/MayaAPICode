import pytest

import sys, os
import maya.standalone
import SineNode

node_name = "TestSineNode"


def setup_module(module):
    maya.standalone.initialize(name="python")
    import maya.cmds as cmds

    print("Loading plugin for testing")
    cmds.loadPlugin("SineNode.py")
    cmds.createNode("SineNodePy", name=node_name)


def teardown_module(module):
    print("closing down maya-standalone")
    maya.standalone.uninitialize()


def test_SineNode():
    # Create a sphere
    import maya.cmds as cmds

    results = cmds.ls(node_name)
    assert len(results) == 1


def test_setAttrib():
    import maya.cmds as cmds

    # Set the amplitude attribute
    cmds.setAttr(f"{node_name}.amplitude", 2.0)
    results = cmds.getAttr(f"{node_name}.amplitude")
    assert results == pytest.approx(2.0)

    cmds.setAttr(f"{node_name}.frequency", 200.0)
    results = cmds.getAttr(f"{node_name}.frequency")
    assert results == pytest.approx(200.0)
