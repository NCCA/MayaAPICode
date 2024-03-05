import maya.api.OpenMaya as om
import sys
import math
import random

# Set this flag to show we are using api 2.0
maya_useNewAPI = True


class InputNode(om.MPxNode):
    id = om.MTypeId(0x00105489)
    output = None
    time = None
    data_size = None

    def __init__(self):
        om.MPxNode.__init__(self)

    # factory to create the node
    @staticmethod
    def creator():
        return InputNode()

    @staticmethod
    def initialize():
        # Create the attributes
        numeric_attrib_fn = om.MFnNumericAttribute()
        # add enum attribute

        # Time attribute
        time_attribute_fn = om.MFnUnitAttribute()
        InputNode.time = time_attribute_fn.create(
            "time", "t", om.MFnUnitAttribute.kTime, 0.0
        )
        time_attribute_fn.storable = False
        time_attribute_fn.keyable = False
        om.MPxNode.addAttribute(InputNode.time)
        
        # Data size attribute
        InputNode.data_size = numeric_attrib_fn.create(
            "dataSize", "ds", om.MFnNumericData.kInt, 10
        )
        numeric_attrib_fn.storable = True
        numeric_attrib_fn.keyable = True
        numeric_attrib_fn.readable = True
        numeric_attrib_fn.writable = True
        om.MPxNode.addAttribute(InputNode.data_size)
        
        # output set by the compute
        
        array_data_fn = om.MFnNumericAttribute()
        InputNode.output = array_data_fn.create(
            "output", "o", om.MFnNumericData.kFloat, 0.0
        )
        array_data_fn.array = True
        array_data_fn.usesArrayDataBuilder = True

        array_data_fn.storable = False
        array_data_fn.keyable = False
        array_data_fn.readable = True
        array_data_fn.writable = False
        om.MPxNode.addAttribute(InputNode.output)

        # Set the attribute dependencies
        om.MPxNode.attributeAffects(InputNode.time, InputNode.output)
        om.MPxNode.attributeAffects(InputNode.data_size, InputNode.output)
        return True

    def compute(self, plug, data):
        ##
		## Description
		##
		##    When input attributes are dirty this method will be called to
		##    recompute the output attributes.
		##
		## Arguments
		##
		##    plug      - the attribute that triggered the compute
		##    datablock - the nodes data
		##
        # we only need to compute if the plug is the output node changing
        if plug == InputNode.output:
            # get the input data and convert to python types
            time_data = data.inputValue(InputNode.time)
            time = time_data.asTime()
            array_data_handle = data.outputArrayValue(InputNode.output)
            data_size = data.inputValue(InputNode.data_size).asInt()

            data_block = self.forceCache()
            array_builder = om.MArrayDataBuilder(data_block, self.output, data_size)

            output_array_builder = array_data_handle.builder()
            om.MGlobal.displayInfo("Data size: " + str(data_size))
            for i in range(0,data_size) :
                #output_handle = output_array_builder.addLast()
                #output_handle.setFloat(random.uniform(0,10))
                child = array_builder.addLast()
                child.setFloat(random.uniform(0,10))
            array_data_handle.set(array_builder)
            array_data_handle.setAllClean()
            data.setClean(plug)
            
            return True
        return False
    
    def postConstructor(self):
        ##
        ## Description
        ##
        ##    When instances of this node are created internally, the MObject associated
        ##    with the instance is not created until after the constructor of this class
        ##    is called. This means that no member functions of Node can
        ##    be called in the constructor.
        ##    The postConstructor solves this problem. Maya will call this function
        ##    after the internal object has been created.
        ##    As a general rule do all of your initialization in the postConstructor.
        ##    Not used here but for info
        print("postConstructor called")

    

def initializePlugin(obj):
    plugin = om.MFnPlugin(obj)
    try:
        plugin.registerNode(
            "InputNodePy", InputNode.id, InputNode.creator, InputNode.initialize
        )
    except:
        sys.stderr.write("Failed to register node\n")
        raise


def uninitializePlugin(obj):
    plugin = om.MFnPlugin(obj)

    try:
        plugin.deregisterNode(InputNode.id)
    except:
        sys.stderr.write("Failed to deregister node\n")
        raise
