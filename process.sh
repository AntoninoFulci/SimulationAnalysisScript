#!/bin/bash

if test -f "Analisi_ane&ne.root"
then
  echo "> Not processing the file Analisi_ane&ne.root exist"
else
  echo "> Processing the simulation file for neutrinos..."
  root -l <<EOF
  .L /home/tony/FlukaWork/Script_LastVersion/neutrini.C
  neutrini o
  o.Loop()
EOF
fi

if test -f "Analisi_neutron.root"
then
  echo "> Not processing the file Analisi_neutron.root exist"
else
  echo "> Processing the simulation file for neutron..."
  root -l <<EOF
  .L /home/tony/FlukaWork/Script_LastVersion/neutroni.C
  neutroni o
  o.Loop()
EOF
fi

if test -f "Analisi_neutron.root" && test -f "Analisi_ane&ne.root"
then
  echo "> Creating graphs..."
#   root -b -l /home/tony/FlukaWork/Script_LastVersion/GraphPlot.C <<EOF
#   .q
# EOF
root -b -l /home/tony/FlukaWork/Script_LastVersion/distanze.C <<EOF
.q
EOF
else
  echo "> Analys file not found, skipping creating graphs."
fi

if [ -f "tar_files.zip" ] || [ -d "tar_files" ]; then
  echo "Merging the results from the tar_files..."
  python /home/tony/FlukaWork/Script_LastVersion/fort_merge.py
fi

if [ -d "neutron_20" ]; then
  cd neutron_20
  root -b -l /home/tony/FlukaWork/Script_LastVersion/neutron_fluka.C <<EOF
  .q
EOF
fi

if [ -d "Grafici" ] && [ -d "neutron_20" ]
then
  python /home/tony/FlukaWork/Script_LastVersion/SpectraTable/SpectraTable.py
fi
