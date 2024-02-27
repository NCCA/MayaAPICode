import maya.OpenMayaAnim as OMA
time_line=OMA.MAnimControl()

print(f" min time {time_line.minTime().value()} max time {time_line.maxTime().value()}")

