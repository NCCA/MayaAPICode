import os
import sys

import maya.api.OpenMaya as OpenMaya
import maya.api.OpenMayaUI as OpenMayaUI
import maya.cmds as cmds
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaUI as omui
from PySide2 import QtCore, QtWidgets
from PySide2.QtCore import QFile
from PySide2.QtGui import QColor, QFont
from PySide2.QtUiTools import QUiLoader
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
    return wrapInstance(int(window), QtWidgets.QWidget)


class SimpleDialog(QtWidgets.QWidget):
    def __init__(self, parent=get_main_window()):
        """init the class and setup dialog"""
        # Python 3 does inheritance differently to 2 so support both
        # as Maya 2020 is still Python 2
        if sys.version_info.major == 3:
            super().__init__(parent)
        # python 2
        else:
            super(SimpleDialog, self).__init__(parent)

        app_root = os.environ.get("QTUIPLUGIN_ROOT")
        # uic.loadUi(app_root + "ui/form.ui", self)  # Load the .ui file
        loader = QUiLoader()
        file = QFile(app_root + "/ui/form.ui")
        print(app_root + "/ui/form.ui")
        file.open(QFile.ReadOnly)
        self.ui = loader.load(file, parentWidget=self)
        file.close()
        # This should make the window stay on top
        self.ui.setWindowFlags(QtCore.Qt.Tool)

        self.ui.show()
        self.ui.show_colour.clicked.connect(self.set_colour)
        self.ui.confirm_button.clicked.connect(self.confirm)
        self.ui.progress_window.clicked.connect(self.do_progress)
        self.ui.prompt_dialog.clicked.connect(self.prompt_dialog)
        self.ui.select_font.clicked.connect(self.select_font)

    def set_colour(self):
        cmds.colorEditor()
        if cmds.colorEditor(query=True, result=True):
            values = cmds.colorEditor(query=True, rgb=True)
            print(values)
            colour = QColor()
            colour.setRedF(values[0])
            colour.setGreenF(values[1])
            colour.setBlueF(values[2])
            self.ui.colour_label.setStyleSheet(
                "QLabel {{ background-color : rgb({},{},{} ) ; border :5px; }}".format(
                    colour.red(), colour.green(), colour.blue()
                )
            )

    def confirm(self):
        result = cmds.confirmDialog(
            title="Confirm",
            message="Are you sure?",
            button=["Yes", "No"],
            defaultButton="Yes",
            cancelButton="No",
            dismissString="No",
        )
        self.ui.confirm_result.setText(result)

    def do_progress(self):
        amount = 0
        cmds.progressWindow(
            title="Doing Nothing",
            progress=amount,
            status="Sleeping: 0%",
            isInterruptable=True,
        )
        while True:
            # Check if the dialog has been cancelled
            if cmds.progressWindow(query=True, isCancelled=True):
                break

            # Check if end condition has been reached
            if cmds.progressWindow(query=True, progress=True) >= 100:
                break
            amount += 25
            # can use f string in new maya
            message = "Sleeping {} %".format(amount)
            cmds.progressWindow(edit=True, progress=amount, status=message)
            cmds.pause(seconds=1)
        cmds.progressWindow(endProgress=1)

    def prompt_dialog(self):
        result = cmds.promptDialog(
            title="Prompt dialog",
            message="Enter some Text:",
            button=["OK", "Cancel"],
            defaultButton="OK",
            cancelButton="Cancel",
            dismissString="Cancel",
        )

        if result == "OK":
            text = cmds.promptDialog(query=True, text=True)
            self.ui.prompt_text.setText(text)

    def select_font(self):
        font = cmds.fontDialog()
        # font elements are seperated by |
        values = font.split("|")
        # first value is the font name
        print(values)
        # sz is value[2] need to split
        size = values[2].split(":")
        font = QFont(values[0], int(size[1]))
        self.ui.font_text.setFont(font)


class QtUIMaya(OpenMaya.MPxCommand):

    CMD_NAME = "QtUIMaya"
    ui = None

    def __init__(self):
        super(QtUIMaya, self).__init__()
        ui = None

    @classmethod
    def doIt(cls, args):
        """
        Called when the command is executed in script
        """
        if QtUIMaya.ui is None:
            QtUIMaya.ui = SimpleDialog()
            QtUIMaya.ui.showNormal()
        else:
            QtUIMaya.ui.showNormal()

    @classmethod
    def creator(cls):
        """
        Think of this as a factory
        """
        return QtUIMaya()

    @classmethod
    def cleanup(cls):
        # cleanup the UI and call the destructors
        QtUIMaya.ui.deleteLater()


def initializePlugin(plugin):
    """
    Load our plugin
    """
    vendor = "NCCA"
    version = "1.0.0"
    if os.environ.get("QTUIPLUGIN_ROOT") is None:
        OpenMaya.MGlobal.displayError(
            "Module Environment not set ensure QTUIPLUGIN_ROOT is set in module file"
        )
        # throw exception and let maya deal with it
        raise

    plugin_fn = OpenMaya.MFnPlugin(plugin, vendor, version)
    try:
        plugin_fn.registerCommand(QtUIMaya.CMD_NAME, QtUIMaya.creator)
        cmds.evalDeferred("cmds.QtUIMaya()")
    except:
        OpenMaya.MGlobal.displayError(
            "Failed to register command: {0}".format(QtUIMaya.CMD_NAME)
        )


def uninitializePlugin(plugin):
    """
    Exit point for a plugin
    """
    # cleanup the dialog first
    QtUIMaya.cleanup()
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
