#!/usr/bin/env python
import os
import sys
import platform
from pathlib import Path

def install_module_linux() :
  print("installing for linux")

def install_module_mac() :
  pass

def install_module_windows() :
  pass


def check_maya_installed(op_sys) :
  """
  ensure maya is installed by checking the followind locations
  linux ~/maya
  mac /Users/username/Library/Preferences/Autodesk/maya
  windows is drive:\\Users\\username\\Documents\\maya\\version
  """
  home_dir=Path.home()
  raise 

if __name__ == "__main__" :
  op_sys=platform.system()
  try :
    check_maya_installed(op_sys)
  except :
    print("Error can't find maya install")
    sys.exit(os.EX_CONFIG)

  if op_sys == "Linux" :
    install_module_linux()
  elif op_sys == "Darwin" :
    install_module_mac()
  elif op_sys == "Windows" :
    install_module_windows()