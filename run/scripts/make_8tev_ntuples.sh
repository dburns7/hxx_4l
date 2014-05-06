#!/bin/sh

#source COMPILE
pushd .. ; make ; popd

# set to zero for all, non-zero to put cap on number output
#setenv MAXEVENTA 1000000 
#setenv MAXEVENTB 100000

#setenv MAXEVENTA 1000
#setenv MAXEVENTB 1000
export MAXEVENTA=1000
export MAXEVENTB=1000

rm ../data/Analysis/8TeV/*.root   ######  DELETING FILES HERE #####

# Using lumi in fb, xsec in fb^-1

#./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=8 --lumi=20 --xsec=2553000    data/analysis/8tev/wjjj.root \
#data/delphes/wjjj/run_*_8TeV/tag_*_delphes_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=1 --lumi=20 --xsec=1 ../data/Analysis/8TeV/ppTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=2 --lumi=20 --xsec=1 ../data/Analysis/8TeV/ppTOhTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOhTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=3 --lumi=20 --xsec=1 ../data/Analysis/8TeV/ppTOzh_zTO2nu_hTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzh_zTO2nu_hTOzzTO4l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=4 --lumi=20 --xsec=1 ../data/Analysis/8TeV/ppTOzh_zTO2l_hTOzzTO2nu2l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOzh_zTO2l_hTOzzTO2nu2l_8TeV_events.root

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=5 --lumi=20 --xsec=1 ../data/Analysis/8TeV/ppTOwh_wTOnul_hTOzzTO4l_8TeV_ntuple.root \
../data/Delphes/8TeV/ppTOwh_wTOnul_hTOzzTO4l_8TeV_events.root




pushd ../data/Analysis/8TeV
if [ -e "all.root" ] 
then 
  rm all.root
fi
hadd all.root *.root
popd
