import maya.api.OpenMaya as OpenMaya
import sys
import math

# Set this flag to show we are using api 2.0
maya_useNewAPI = True


class SineNode(OpenMaya.MPxNode):
    # class attributes are placed here maya will use these to create the node
    # and to determine the type of node which is setup during initialize method
    # Older plugins used to set these to OpenMaya.MObject() (see api1 demos)
    # however we now just set to None and use the FunctionSets to create the attributess
    id = OpenMaya.MTypeId(0x00105481)
    amplitude = None
    frequency = None
    time = None
    output = None
    function_type = None

    def __init__(self):
        OpenMaya.MPxNode.__init__(self)

    # factory to create the node
    @staticmethod
    def creator():
        return SineNode()

    @staticmethod
    def initialize():
        # Create the attributes
        numeric_attrib_fn = OpenMaya.MFnNumericAttribute()
        SineNode.amplitude = numeric_attrib_fn.create(
            "amplitude", "a", OpenMaya.MFnNumericData.kDouble, 1.0
        )
        numeric_attrib_fn.storable = True
        numeric_attrib_fn.keyable = True
        numeric_attrib_fn.readable = True
        numeric_attrib_fn.writable = True
        OpenMaya.MPxNode.addAttribute(SineNode.amplitude)
        # add frequency attribute
        SineNode.frequency = numeric_attrib_fn.create(
            "frequency", "f", OpenMaya.MFnNumericData.kDouble, 1.0
        )
        numeric_attrib_fn.storable = True
        numeric_attrib_fn.keyable = True
        numeric_attrib_fn.readable = True
        numeric_attrib_fn.writable = True
        OpenMaya.MPxNode.addAttribute(SineNode.frequency)
        # add enum attribute
        enum_attribute_fn = OpenMaya.MFnEnumAttribute()
        SineNode.function_type = enum_attribute_fn.create("functionType", "ft")
        enum_attribute_fn.addField("sine", 0)
        enum_attribute_fn.addField("cosine", 1)
        enum_attribute_fn.addField("complex", 2)

        enum_attribute_fn.storable = True
        enum_attribute_fn.keyable = True
        enum_attribute_fn.readable = True
        OpenMaya.MPxNode.addAttribute(SineNode.function_type)

        # Time attribute
        time_attribute_fn = OpenMaya.MFnUnitAttribute()
        SineNode.time = time_attribute_fn.create(
            "time", "t", OpenMaya.MFnUnitAttribute.kTime, 0.0
        )
        time_attribute_fn.storable = False
        time_attribute_fn.keyable = False
        OpenMaya.MPxNode.addAttribute(SineNode.time)
        # output set by the compute
        SineNode.output = numeric_attrib_fn.create(
            "output", "o", OpenMaya.MFnNumericData.kDouble, 0.0
        )
        numeric_attrib_fn.storable = False
        numeric_attrib_fn.keyable = False
        numeric_attrib_fn.readable = True
        numeric_attrib_fn.writable = False
        OpenMaya.MPxNode.addAttribute(SineNode.output)
        # Set the attribute dependencies
        OpenMaya.MPxNode.attributeAffects(SineNode.amplitude, SineNode.output)
        OpenMaya.MPxNode.attributeAffects(SineNode.frequency, SineNode.output)
        OpenMaya.MPxNode.attributeAffects(SineNode.time, SineNode.output)
        OpenMaya.MPxNode.attributeAffects(SineNode.function_type, SineNode.output)
        return True

    def compute(self, plug, data):
        """
        Description

           When input attributes are dirty this method will be called to
           recompute the output attributes.

        Arguments

           plug      - the attribute that triggered the compute
           data - the nodes data
        """
        # we only need to compute if the plug is the output node changing
        if plug == SineNode.output:
            # get the input data and convert to python types
            time_data = data.inputValue(SineNode.time)
            time = time_data.asTime()
            amplitude = data.inputValue(SineNode.amplitude).asDouble()
            frequency = data.inputValue(SineNode.frequency).asDouble()
            function_type = data.inputValue(SineNode.function_type).asShort()
            # compute the result
            if function_type == 0:
                result = amplitude * math.sin(
                    math.radians(frequency * math.pi * time.asUnits(OpenMaya.MTime.kSeconds))
                )
            elif function_type == 1:
                result = amplitude * math.cos(
                    math.radians(frequency * math.pi * time.asUnits(OpenMaya.MTime.kSeconds))
                )
            else:
                result = amplitude * math.sin(
                    math.radians(frequency * math.pi * time.asUnits(OpenMaya.MTime.kSeconds))
                ) + amplitude * math.sin(
                    math.radians(
                        2 * frequency * math.pi * time.asUnits(OpenMaya.MTime.kSeconds)
                    )
                )
            # set the output
            output_data = data.outputValue(SineNode.output)
            output_data.setDouble(result)
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
            "SineNodePy", SineNode.id, SineNode.creator, SineNode.initialize
        )
    except:
        sys.stderr.write("Failed to register node\n")
        raise


def uninitializePlugin(obj):
    plugin = OpenMaya.MFnPlugin(obj)

    try:
        plugin.deregisterNode(SineNode.id)
    except:
        sys.stderr.write("Failed to deregister node\n")
        raise
