import os
import sys
import subprocess

def restart_program():
    python = sys.executable
    os.execl(python, * sys.argv) 

#
Venv_dir = input("directory of the venv")
script_file = input("Script directory")

python_bin = Venv_dir  + "/bin/python3"
subprocess.Popen([python_bin, script_file])

restart_program()

