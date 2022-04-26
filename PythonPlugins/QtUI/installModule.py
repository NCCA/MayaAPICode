#!/usr/bin/env python
import os
import platform
import sys
from pathlib import Path

maya_locations = {
    "Linux": "/maya",
    "Darwin": "/Library/Preferences/Autodesk/maya",
    "Windows": "\\Documents\\maya\\version",
}


def install_module(location):
    print(f"installing to {location}")
    # first write the module file
    current_dir = Path.cwd()
    if not Path(location + "modules/QtUIMaya.mod").is_file():
        print("writing module file")
        with open(location + "modules/QtUIMaya.mod", "w") as file:
            file.write(f"+ QtUiMaya 1.0 {current_dir}\n")
            file.write("MAYA_PLUG_IN_PATH +:= plug-ins\n")
            file.write(f"QTUIPLUGIN_ROOT={current_dir}\n\n")
    # if we are using Maya 2022 and above this will be ok
    # however if using 2020 and below we need to set Maya.env
    folders = os.listdir(location)
    for folder in folders:
        # nobody use maya begining with 19 do they? :-)
        if folder.startswith("20") and int(folder) < 2022:
            with open(f"{location}/{folder}/Maya.env", "a+") as env_file:
                if "QTUIPLUGIN_ROOT" not in env_file.read():
                    env_file.write(f"QTUIPLUGIN_ROOT={current_dir}\n\n")


def check_maya_installed(op_sys):
    mloc = f"{Path.home()}{maya_locations.get(op_sys)}/"
    if not os.path.isdir(mloc):
        raise
    return mloc


if __name__ == "__main__":
    op_sys = platform.system()
    try:
        m_loc = check_maya_installed(op_sys)
    except:
        print("Error can't find maya install")
        sys.exit(os.EX_CONFIG)

    install_module(m_loc)
