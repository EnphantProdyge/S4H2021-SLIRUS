import os
import sys


def restart_program():
    python = sys.executable
    os.execl(python, * sys.argv) 

#Creating a new virtual environment
Name_input = input("Write the name of your virtual environment : ")
directory_input = input("Write the directory of your virtual environment. Ex: /home/pi : ")


os.system("python3 -m venv " + directory_input + "/" + Name_input)
#print("source " + directory_input + "/" + Name_input + "/bin/activate")
#os.system("source " + directory_input + "/" + Name_input + "/bin/activate")


#install_input = input("What module do you want to install?:- ")
'''
pip_install = "python3 -m pip install "
print("Installing sounddevice")
os.system(pip_install + "sounddevice")
print("Done sounddevice")

print("Installing google.cloud.storage")
os.system(pip_install + "google-cloud-storage")
print("Done google.cloud.storage")
print("Installing google.cloud.speech")
os.system(pip_install + "google-cloud-speech")
print("Done google.cloud.speech")
print("Installing soundfile")
os.system(pip_install + "soundfile")
print("Done soundfile")
'''
restart_program()

#code plus bas est pour executer un script dans notre env
'''

'''
