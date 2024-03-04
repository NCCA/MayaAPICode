import maya.api.OpenMaya as om
import sys

# Set this flag to show we are using api 2.0
maya_useNewAPI = True


class SineNode(om.MPxNode):
    id = om.MTypeId(0x00105481)

    def __init__(self):
        om.MPxNode.__init__(self)

    # factory to create the node
    @staticmethod
    def creator():
        return SineNode()

    @staticmethod
    def initialize(): ...

    # COMPUTE METHOD'S DEFINITION:
    def compute(self, plug, data):
        assert isinstance(data.context(), om.MDGContext)
        assert data.setContext(data.context()) == data


def initializePlugin(obj):
    plugin = om.MFnPlugin(obj)
    try:
        plugin.registerNode(
            "SineNodePy", SineNode.id, SineNode.creator, SineNode.initialize
        )
    except:
        sys.stderr.write("Failed to register node\n")
        raise


def uninitializePlugin(obj):
    plugin = om.MFnPlugin(obj)

    try:
        plugin.deregisterNode(SineNode.id)
    except:
        sys.stderr.write("Failed to deregister node\n")
        raise
