import os
import shutil
import subprocess
import glob


#mettere lo script fuori dalla cartella "tar_files"
current_dir = os.getcwd() + '/'
tar_files_dir = current_dir + 'tar_files'
fort_files_dir = current_dir + 'neutron_20'

os.system("rm -r neutron_20")

#creazione delle cartelle se non ci sono
if not os.path.exists(tar_files_dir):
    print("Tar files folder does not exits")
    print("Looking for .zip file")
    if os.path.exists(tar_files_dir + ".zip"):
        print("Decompressing the .zip file")
        os.system("unzip -q tar_files.zip")
        os.chdir(tar_files_dir)
        print("Current wd: " + os.getcwd())
elif os.path.exists(tar_files_dir):
    print("Tar files folder exits cd into it")
    os.chdir(tar_files_dir)
    print("Current wd: " + os.getcwd())

print("Untarring the files...")

os.system("ls *.tar |xargs -n1 tar -xf")
os.system("rm -r *.tar")
os.system("rm -r *.lis")
os.rename(tar_files_dir, fort_files_dir)

print("Changing wd to: " + os.getcwd())

files_to_merge = glob.glob("*_fort.61")

tab_merge = "merge.sh"

print("Checking files...")

usxsuw_location = "$FLUPRO/flutil/usxsuw";

if not os.path.exists(usxsuw_location):
    print("Default location of usxsuw does not exist, insert manually the location of usxsuw")
    usxsuw_location = "/home/tony/fluka/flutil/usxsuw"   #insert here the location
    if os.path.exists(usxsuw_location):
        print("The file usxsuw exist!")
    else:
        print("The file usxsuw not found")
        quit()

print("Creating the script to merge the fort files...")
with open(tab_merge, 'w') as f:
    f.write("#!/bin/bash\n\n")
    f.write(usxsuw_location + " <<EOF \n")
    f.write("\n".join(files_to_merge))
    f.write("\n \n")
    f.write("Neutrons_0-20MeV\n")
    f.write("EOF")

os.system("chmod +x merge.sh")

print("Executing the script...")

os.system("./merge.sh > log.dat")

print("\nCleaning the folder...")

os.system("rm -r *_fort.*")
os.system("rm -r *.sh")
os.system("rm -r *.bnx")
