import os
import shutil

orig = os.getcwd()
path = orig + "/Tables"
src = "/home/tony/FlukaWork/Script_LastVersion/SpectraTable"

if not os.path.exists(path):
    print("Creating Tables folder...")
    os.mkdir(path)
    os.chdir(path)
else:
    print("Tables folder exits cd into it...")
    os.chdir(path)

print("Creting pdf files...")

shutil.copyfile(src + "/SpectraTable.tex", path + "/SpectraTable.tex")
shutil.copyfile(src + "/DistancesTable.tex", path + "/DistancesTable.tex")

a_file = open("SpectraTable.tex", "r")
list_of_lines = a_file.readlines()
list_of_lines[37] = "\def \FigPath {" + orig + "}\n"
a_file = open("SpectraTable.tex", "w")
a_file.writelines(list_of_lines)
a_file.close()

a_file = open("DistancesTable.tex", "r")
list_of_lines = a_file.readlines()
list_of_lines[37] = "\def \FigPath {" + orig + "}\n"
a_file = open("DistancesTable.tex", "w")
a_file.writelines(list_of_lines)
a_file.close()

os.system("latexmk -pdf -synctex=1 -silent SpectraTable.tex")
os.system("latexmk -pdf -synctex=1 -silent DistancesTable.tex")

print("Cleaning up the folder...")
os.system("rm -r *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz *.tex")
