index = 0
y = 0.0
for z in range(-5, 5):
    for x in range(-5, 5):
        cmds.polyCube(name=f"cube{index}", w=0.4, h=0.4, d=0.4)
        cmds.move(x, y, z, f"cube{index}")
        index += 1

cmds.createNode("InputNodePy", name="ipNode1")
cmds.setAttr("ipNode1.data_size", 100)
cmds.connectAttr("time1.outTime", "ipNode1.time")

for i in range(0, 100):
    cmds.connectAttr(f"ipNode1.output[{i}]", f"cube{i}.ty")
