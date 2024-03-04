import maya.api.OpenMaya as om
import maya.cmds as cmds
import random

'''
def maya_useNewAPI():
    """
    Can either use this function (which works on earlier versions)
    or we can set maya_useNewAPI = True
    """
    pass
'''

maya_useNewAPI = True


class CustomSphere(om.MPxCommand):

    CMD_NAME = "CustomSpherePy"

    def __init__(self):
        super(CustomSphere, self).__init__()

    def doIt(self, args):
        """
        Called when the command is executed in script
        args is an MArgList object
        """
        print(len(args))
        if len(args) == 0:
            om.MGlobal.displayError("No radius specified")
            return False
        try :
            self.count = args.asInt(0)
        except :
            om.MGlobal.displayError("Invalid radius")
            return False
        if self.count < 0:
            om.MGlobal.displayError("argument must be greater than zero")
            return False
        return self.redoIt()

    def redoIt(self):
        """
        Called when the command is executed in script
        """
        for i in range(0,self.count) :
            x = random.uniform(-10, 10)
            y = random.uniform(-10, 10)
            z = random.uniform(-10, 10)
            radius = random.uniform(0.8,4.5)
            cmds.sphere(r=radius, n=f"sphere{i}")
            cmds.move(x, y, z)
        return True

    def undoIt(self):
        """
        Called when the command is undone
        """
        for i in range(0,self.count) :
            cmds.delete(f"sphere{i}")


    def isUndoable(self):
        return True


    @classmethod
    def creator(cls):
        """
        Think of this as a factory
        """
        return CustomSphere()


def initializePlugin(plugin):
    """
    Load our plugin
    """
    vendor = "NCCA"
    version = "1.0.0"

    plugin_fn = om.MFnPlugin(plugin, vendor, version)

    try:
        plugin_fn.registerCommand(CustomSphere.CMD_NAME, CustomSphere.creator)
    except:
        om.MGlobal.displayError(
            "Failed to register command: {0}".format(CustomSphere.CMD_NAME)
        )


def uninitializePlugin(plugin):
    """
    Exit point for a plugin
    """
    plugin_fn = om.MFnPlugin(plugin)
    try:
        plugin_fn.deregisterCommand(CustomSphere.CMD_NAME)
    except:
        om.MGlobal.displayError(
            "Failed to deregister command: {0}".format(CustomSphere.CMD_NAME)
        )


if __name__ == "__main__":
    """
    So if we execute this in the script editor it will be a __main__ so we can put testing code etc here
    Loading the plugin will not run this
    As we are loading the plugin it needs to be in the plugin path.
    """

    plugin_name = "CustomSphere.py"

    cmds.evalDeferred(
        'if cmds.pluginInfo("f{plugin_name}", q=True, loaded=True): cmds.unloadPlugin(f"{plugin_name}")')

    cmds.evalDeferred(
        'if not cmds.pluginInfo(f"{plugin_name}", q=True, loaded=True): cmds.loadPlugin(f"{plugin_name}")')

    cmds.CustromSpherePy(100)