import sys

import maya.api.OpenMaya as OpenMaya
import maya.api.OpenMayaUI as OpenMayaUI
import maya.cmds as cmds
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaUI as omui
from PySide2 import QtCore, QtWidgets
from shiboken2 import wrapInstance


def maya_useNewAPI():
    """
    Can either use this function (which works on earlier versions)
    or we can set maya_useNewAPI = True
    """
    pass


maya_useNewAPI = True


def get_main_window():
    """this returns the maya main window for parenting"""
    window = omui.MQtUtil.mainWindow()
    return wrapInstance(int(window), QtWidgets.QDialog)


class SimpleDialog(QtWidgets.QDialog):
    def __init__(self, parent=get_main_window()):
        """init the class and setup dialog"""
        # Python 3 does inheritance differently to 2 so support both
        # as Maya 2020 is still Python 2
        if sys.version_info.major == 3:
            super().__init__(parent)
        # python 2
        else:
            super(SimpleDialog, self).__init__(parent)
        # Set the GUI components and layout
        self.setWindowTitle("SimpleDialog")
        self.resize(200, 200)


class QtUIMaya(OpenMaya.MPxCommand):

    CMD_NAME = "QtUIMaya"

    def __init__(self):
        super(QtUIMaya, self).__init__()
        self.ui = None

    def doIt(self, args):
        """
        Called when the command is executed in script
        """
        if self.ui is None:
            self.ui = SimpleDialog()
            self.ui.showNormal()
        else:
            self.ui.showNormal()
        pass

    @classmethod
    def creator(cls):
        """
        Think of this as a factory
        """
        return QtUIMaya()


def initializePlugin(plugin):
    """
    Load our plugin
    """
    vendor = "NCCA"
    version = "1.0.0"

    plugin_fn = OpenMaya.MFnPlugin(plugin, vendor, version)

    try:
        plugin_fn.registerCommand(QtUIMaya.CMD_NAME, QtUIMaya.creator)
    except:
        OpenMaya.MGlobal.displayError(
            "Failed to register command: {0}".format(QtUIMaya.CMD_NAME)
        )


def uninitializePlugin(plugin):
    """
    Exit point for a plugin
    """
    plugin_fn = OpenMaya.MFnPlugin(plugin)
    try:
        plugin_fn.deregisterCommand(QtUIMaya.CMD_NAME)
    except:
        OpenMaya.MGlobal.displayError(
            "Failed to deregister command: {0}".format(QtUIMaya.CMD_NAME)
        )


if __name__ == "__main__":
    """
    So if we execute this in the script editor it will be a __main__ so we can put testing code etc here
    Loading the plugin will not run this
    As we are loading the plugin it needs to be in the plugin path.
    """

    plugin_name = "QtUIMaya.py"

    cmds.evalDeferred(
        'if cmds.pluginInfo("{0}", q=True, loaded=True): cmds.unloadPlugin("{0}")'.format(
            plugin_name
        )
    )
    cmds.evalDeferred(
        'if not cmds.pluginInfo("{0}", q=True, loaded=True): cmds.loadPlugin("{0}")'.format(
            plugin_name
        )
    )
