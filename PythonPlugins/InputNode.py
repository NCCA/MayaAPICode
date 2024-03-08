import maya.api.OpenMaya as OpenMaya
import sys
import math
import random

# Set this flag to show we are using api 2.0
maya_useNewAPI = True


class InputNode(OpenMaya.MPxNode):
    # class attributes are placed here maya will use these to create the node
    # and to determine the type of node which is setup during initialize method
    # Older plugins used to set these to OpenMaya.MObject() (see api1 demos)
    # however we now just set to None and use the FunctionSets to create the attributes
    id = OpenMaya.MTypeId(0x00105489)
    output = None
    time = None
    data_size = None

    def __init__(self):
        OpenMaya.MPxNode.__init__(self)

    # factory to create the node
    @staticmethod
    def creator():
        return InputNode()

    @staticmethod
    def initialize():
        # Create the attributes
        numeric_attrib_fn = OpenMaya.MFnNumericAttribute()
        # add enum attribute

        # Time attribute
        time_attribute_fn = OpenMaya.MFnUnitAttribute()
        InputNode.time = time_attribute_fn.create(
            "time", "t", OpenMaya.MFnUnitAttribute.kTime, 0.0
        )
        time_attribute_fn.storable = False
        time_attribute_fn.keyable = False
        OpenMaya.MPxNode.addAttribute(InputNode.time)

        # Data size attribute
        InputNode.data_size = numeric_attrib_fn.create(
            "dataSize", "ds", OpenMaya.MFnNumericData.kInt, 10
        )
        numeric_attrib_fn.storable = True
        numeric_attrib_fn.keyable = True
        numeric_attrib_fn.readable = True
        numeric_attrib_fn.writable = True
        OpenMaya.MPxNode.addAttribute(InputNode.data_size)

        # output set by the compute

        array_data_fn = OpenMaya.MFnNumericAttribute()
        InputNode.output = array_data_fn.create(
            "output", "o", OpenMaya.MFnNumericData.kFloat, 0.0
        )
        array_data_fn.array = True
        array_data_fn.usesArrayDataBuilder = True

        array_data_fn.storable = False
        array_data_fn.keyable = False
        array_data_fn.readable = True
        array_data_fn.writable = False
        OpenMaya.MPxNode.addAttribute(InputNode.output)

        # Set the attribute dependencies
        OpenMaya.MPxNode.attributeAffects(InputNode.time, InputNode.output)
        OpenMaya.MPxNode.attributeAffects(InputNode.data_size, InputNode.output)
        return True

    def compute(self, plug, data):
        """
        Description

         When input attributes are dirty this method will be called to
         recompute the output attributes.

        Arguments

            plug      - the attribute that triggered the compute
            data - the nodes data

        we only need to compute if the plug is the output node changing
        """
        if plug == InputNode.output:
            # get the input data and convert to python types
            time_data = data.inputValue(InputNode.time)
            time = time_data.asTime()
            array_data_handle = data.outputArrayValue(InputNode.output)
            data_size = data.inputValue(InputNode.data_size).asInt()

            data_block = self.forceCache()
            array_builder = OpenMaya.MArrayDataBuilder(data_block, self.output, data_size)

            output_array_builder = array_data_handle.builder()
            OpenMaya.MGlobal.displayInfo("Data size: " + str(data_size))
            for i in range(0, data_size):
                child = array_builder.addLast()
                child.setFloat(random.uniform(0, 10))
            array_data_handle.set(array_builder)
            array_data_handle.setAllClean()
            data.setClean(plug)

            return True
        return False

    def postConstructor(self):
        """
        Description

           When instances of this node are created internally, the MObject associated
           with the instance is not created until after the constructor of this class
           is called. This means that no member functions of Node can
           be called in the constructor.
           The postConstructor solves this problem. Maya will call this function
           after the internal object has been created.
           As a general rule do all of your initialization in the postConstructor.
           Not used here but for info
        """
        print("postConstructor called")


def initializePlugin(obj):
    plugin = OpenMaya.MFnPlugin(obj)
    try:
        plugin.registerNode(
            "InputNodePy", InputNode.id, InputNode.creator, InputNode.initialize
        )
    except:
        sys.stderr.write("Failed to register node\n")
        raise


def uninitializePlugin(obj):
    plugin = OpenMaya.MFnPlugin(obj)

    try:
        plugin.deregisterNode(InputNode.id)
    except:
        sys.stderr.write("Failed to deregister node\n")
        raise
