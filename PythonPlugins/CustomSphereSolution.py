import maya.api.OpenMaya as OpenMaya
import maya.cmds as cmds
import random


maya_useNewAPI = True


class CustomSphereSolution(OpenMaya.MPxCommand):

    CMD_NAME = "CustomSphereSolutionPy"

    def __init__(self):
        super(CustomSphereSolution, self).__init__()
        self.count=100
        self.xExtent=20
        self.yExtent=20
        self.zExtent=20
        self.minRadius=0.2
        self.maxRadius=2.5


    def postConstructor(self):
        """
        This method is called after the object has been constructed
        """
        self.count=100
        self.xExtent=20
        self.yExtent=20
        self.zExtent=20
        self.minRadius=0.2
        self.maxRadius=2.5


    @staticmethod
    def newSyntax() :
        """
        Create a new syntax object registered as part of the

        """
        syntax = OpenMaya.MSyntax()
        syntax.addFlag("-h", "-help")
        syntax.addFlag("-x","-xExtents", OpenMaya.MSyntax.kDouble)
        syntax.addFlag("-y","-yExtents", OpenMaya.MSyntax.kDouble)
        syntax.addFlag("-z","-zExtents", OpenMaya.MSyntax.kDouble)
        syntax.addFlag("-n","-number", OpenMaya.MSyntax.kUnsigned)
        syntax.addFlag("-mr","-minRadius", OpenMaya.MSyntax.kDouble)
        syntax.addFlag("-mm","-maxRadius", OpenMaya.MSyntax.kDouble)
        return syntax

    def doIt(self, args):
        """
        Called when the command is executed in script
        :param args:
        :return:
        """

        # now create a parser object
        parser=OpenMaya.MArgParser(self.syntax(),args)
        if parser.isFlagSet("-h"):
            OpenMaya.MGlobal.displayInfo("This command creates a number of spheres in the scene")
            OpenMaya.MGlobal.displayInfo("Flags:")
            OpenMaya.MGlobal.displayInfo("-x -xExtents: The x extents of the sphere")
            OpenMaya.MGlobal.displayInfo("-y -yExtents: The y extents of the sphere")
            OpenMaya.MGlobal.displayInfo("-z -zExtents: The z extents of the sphere")
            OpenMaya.MGlobal.displayInfo("-n -number: The number of spheres to create")
            OpenMaya.MGlobal.displayInfo("-mr -minRadius: The minimum radius of the sphere")
            OpenMaya.MGlobal.displayInfo("-mm -maxRadius: The maximum radius of the sphere")
            return True
        if parser.isFlagSet("-n"):
            self.count=parser.flagArgumentInt("-n",0)
        if parser.isFlagSet("-x"):
            self.xExtent=parser.flagArgumentDouble("-x",0)
        if parser.isFlagSet("-y"):
            self.yExtent=parser.flagArgumentDouble("-y",0)
        if parser.isFlagSet("-z"):
            self.zExtent=parser.flagArgumentDouble("-z",0)
        if parser.isFlagSet("-mr"):
            self.minRadius=parser.flagArgumentDouble("-mr",0)
        if parser.isFlagSet("-mm"):
            self.maxRadius=parser.flagArgumentDouble("-mm",0)
        return self.redoIt()

    def redoIt(self):
        """
        Called when the command is executed in script
        """
        for i in range(0, self.count):
            x = random.uniform(-self.xExtent, self.xExtent)
            y = random.uniform(-self.yExtent, self.yExtent)
            z = random.uniform(-self.zExtent, self.zExtent)
            radius = random.uniform(self.minRadius, self.maxRadius)
            cmds.sphere(r=radius, n=f"sphere{i}")
            cmds.move(x, y, z)
        return True

    def undoIt(self):
        """
        Called when the command is undone
        """

        for i in range(0, self.count):
            cmds.delete(f"sphere{i}")

    def isUndoable(self):
        return True

    @classmethod
    def creator(cls):
        """
        Think of this as a factory
        """
        return CustomSphereSolution()


def initializePlugin(plugin):
    """
    Load our plugin
    """
    vendor = "NCCA"
    version = "1.0.0"

    plugin_fn = OpenMaya.MFnPlugin(plugin, vendor, version, "Any")

    try:
        plugin_fn.registerCommand(CustomSphereSolution.CMD_NAME, CustomSphereSolution.creator, CustomSphereSolution.newSyntax)
    except:
        OpenMaya.MGlobal.displayError(f"Failed to register command: {CustomSphereSolution.CMD_NAME}")


def uninitializePlugin(plugin):
    """
    Exit point for a plugin
    """
    plugin_fn = OpenMaya.MFnPlugin(plugin)
    try:
        plugin_fn.deregisterCommand(CustomSphereSolution.CMD_NAME)
    except:
        OpenMaya.MGlobal.displayError(f"Failed to deregister command: {CustomSphereSolution.CMD_NAME}")

