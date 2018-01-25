#!/bin/bash

# Save current working dir so img can be outputted there later
W_DIR=$(pwd);

# Set SCRAM architecture var
SCRAM_ARCH=slc6_amd64_gcc530; 
export SCRAM_ARCH;

cd $W_DIR;
source /afs/cern.ch/cms/cmsset_default.sh;
eval `scram run -sh`;

mkdir -p $W_DIR/results

elements=(X Y Z Alpha Beta Gamma)

for i in "${elements[@]}"
do
    echo "Processing: $i element"

    getPayloadData.py  \
	--plugin pluginTrackerAlignment_PayloadInspector \
	--plot plot_TrackerAlignmentCompare${i} \
	--tag TrackerAlignment_PCL_byRun_v2_express \
	--time_type Run \
	--iovs '{"start_iov": "303809", "end_iov": "303886"}' \
	--db Prod \
	--test;

    mv *.png $W_DIR/results/TrackerAlignmentCompare${i}.png
done

elements=(BPix FPix TIB TOB TID TEC)

for i in "${elements[@]}"
do
    echo "Processing: $i element"
    
    getPayloadData.py  \
 	--plugin pluginTrackerAlignment_PayloadInspector \
 	--plot plot_TrackerAlignmentSummary${i} \
 	--tag TrackerAlignment_PCL_byRun_v2_express \
 	--time_type Run \
	--iovs '{"start_iov": "292200", "end_iov": "303886"}' \
  	--db Prod \
 	--test;
    
    mv *.png $W_DIR/results/TrackerAlignmentSummary${i}.png
done