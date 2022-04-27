import os
import shutil
import subprocess

#cartella all'interno della quale si trovano le cartelle job
#basta mettere il file dove ci sono le cartelle job e fa tutto da solo
# print(os.getcwd() + '/')
job_dir = os.getcwd() + '/'

root_files_dir = job_dir + 'root_files'
tar_files_dir = job_dir + 'tar_files'

#creazione delle cartelle se non ci sono
if not os.path.exists(root_files_dir):
    print("Root files folder does not exist, creating it.")
    os.mkdir(root_files_dir)
else:
    print(root_files_dir + " already exits! Skipping it.")

if not os.path.exists(tar_files_dir):
    print("Tar files folder does not exist, creating it.")
    os.mkdir(tar_files_dir)
else:
    print(tar_files_dir + " already exits!  Skipping it.")

##############

#Sposta i file tutti nella stessa cartella
buffer = os.listdir(job_dir)

jobs_list = []

for x in buffer:
    if "job" in x:
        jobs_list.append(x)

print(jobs_list)

for x in jobs_list:
    jobs = job_dir + x
    # print(jobs)
    job_files = os.listdir(jobs)
    for file in job_files:
        if file.endswith('.root'):
            shutil.move(os.path.join(jobs,file), os.path.join(root_files_dir,file))
        if file.endswith('.tar'):
            shutil.move(os.path.join(jobs,file), os.path.join(tar_files_dir,file))
    #shutil.rmtree(jobs)

#Cambia la directory e unisce i file .root nella cartella dei job
os.chdir(root_files_dir)
os.system('$ROOTSYS/bin/hadd -f ' + job_dir + 'simulazione.root *.root')
