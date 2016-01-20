#! /bin/bash

# Quick script to run the plot-making macro for the HBHE commissioning channel timing plots

# Directory where your input root files are stored
# INPUTDIR=/afs/cern.ch/user/s/sabrandt/work/public/For_Miao/HcalTiming/CMSSW_7_6_3/src/
INPUTDIR=../..

# function inputs: drawPlots(inputFileName, outputFolderName, datasetDescriptionForPlots, getTextOutput of timing)
root -l -q drawTimingMaps.C+\(\"${INPUTDIR}/run251721.root\",\"run251721\",\"run251721\"\)

rm *.so *.d *.pcm