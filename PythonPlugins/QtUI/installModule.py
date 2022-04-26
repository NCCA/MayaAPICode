#!/usr/bin/env python
import os
import sys
import platform
from pathlib import Path

maya_locations={
  "Linux" : "/maya",
  "Darwin" : "/Library/Preferences/Autodesk/maya",
  "Windows" :  "\\Documents\\maya\\version"
}

def install_module_linux(location) :
  print(f"installing for linux {location}")

def install_module_mac(location) :
  pass

def install_module_windows(location) :
  pass


def check_maya_installed(op_sys) :
  
  mloc=f"{Path.home()}{maya_locations.get(op_sys)}/"
  if  not os.path.isdir(mloc) :
    raise 
  return mloc


if __name__ == "__main__" :
  op_sys=platform.system()
  try :
    m_loc=check_maya_installed(op_sys)
  except :
    print("Error can't find maya install")
    sys.exit(os.EX_CONFIG)

  if op_sys == "Linux" :
    install_module_linux(m_loc)
  elif op_sys == "Darwin" :
    install_module_mac(m_loc)
  elif op_sys == "Windows" :
    install_module_windows(m_loc)