#!/bin/sh

source COMPILE
#pushd .. ; make ; popd

# set to zero for all, non-zero to put cap on number output
export MAXEVENTA=1000
export MAXEVENTB=1000

rm ../data/Analysis/8TeV/*.root   ######  DELETING FILES HERE #####

# Using lumi in fb, xsec in fb^-1
# H -> llll ................................. 2.813706
# ZZ -> llll ................................ 71.637508096
# ZH, Z -> vv, H -> llll .................... 0.011097
# ZH, Z -> ll, H -> llvv .................... 0.3139155
# WH, W -> lv, H -> llll .................... 0.020300112

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=1 --lumi=20 --xsec=2.814 ../data/Analysis/8TeV/ppTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOhTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=2 --lumi=20 --xsec=71.64 ../data/Analysis/8TeV/ppTOhTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=3 --lumi=20 --xsec=0.01110 ../data/Analysis/8TeV/ppTOzh_zTO2nu_hTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzh_zTO2nu_hTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=4 --lumi=20 --xsec=0.3139 ../data/Analysis/8TeV/ppTOzh_zTO2l_hTOzzTO2nu2l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzh_zTO2l_hTOzzTO2nu2l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=5 --lumi=20 --xsec=0.02030 ../data/Analysis/8TeV/ppTOwh_wTOnul_hTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOwh_wTOnul_hTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=20 --lumi=20 --xsec=0.1 ../data/Analysis/8TeV/ppTOwh_wTOnul_hTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOhxx_hTOzzTO4l_8TeV_1GeV_events.root



pushd ../data/Analysis/8TeV
if [ -e "all.root" ] 
then 
  rm all.root
fi
hadd all.root *.root
popd
