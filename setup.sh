#!/bin/bash

#cp ~/../mulhearn/Delphes-3.0.10.tar .
cp /home/dburns/WorkingArea/Analysis/Delphes-3.0.10.tar .
tar -xf Delphes-3.0.10.tar
rm Delphes-3.0.10.tar
cd Delphes-3.0.10/
make
cd ..
cd include/
ln -s ../Delphes-3.0.10/classes/ classes
ln -s ../Delphes-3.0.10/external/ExRootAnalysis/ ExRootAnalysis
cd ../lib/
ln -s ../Delphes-3.0.10/libDelphes.so libDelphes.so
cd ..
cd run
ln -s ../lib/ .
source COMPILE
cd ..
echo 'hxx setup complete!'
