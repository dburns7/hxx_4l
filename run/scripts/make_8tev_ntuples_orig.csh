source COMPILE

# set to zero for all, non-zero to put cap on number output
#setenv MAXEVENTA 1000000 
#setenv MAXEVENTB 100000

setenv MAXEVENTA 1000
setenv MAXEVENTB 1000

rm data/analysis/8tev/*.root   ######  DELETING FILES HERE #####

# Using lumi in fb, xsec in fb^-1

./bin/MakeHxxTree --maxevent=$MAXEVENTB --sample=8 --lumi=20 --xsec=2553000    data/analysis/8tev/wjjj.root \
data/delphes/wjjj/run_*_8TeV/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTA --sample=1 --lumi=20 --xsec=206176    data/analysis/8tev/zjj.root \
data/delphes/pass2_zjj/run_8tev/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=2 --lumi=20 --xsec=741.8    data/analysis/8tev/zz_lljj.root \
data/delphes/zz_lljj/run_100K_8TeV/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=2 --lumi=20 --xsec=1055     data/analysis/8tev/zw_lljj.root \
data/delphes/zw_lljj/run_100K_8TeV/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=3 --lumi=20 --xsec=744.5     data/analysis/8tev/wwjj.root \
data/delphes/wwjj_lvlvjj/run_100K_8TeV/tag_*_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTA --sample=4 --lumi=20 --xsec=6298      data/analysis/8tev/tt.root \
data/delphes/tt/run_100K_8TeV/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=5 --lumi=20 --xsec=31.31    data/analysis/8tev/h_zz_lljj.root \
data/delphes/h_lljj/run_8tev/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=6 --lumi=20 --xsec=0.02996  data/analysis/8tev/zh_lljjvv.root \
data/delphes/zh_lljjvv/run_8tev/tag_1_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=7 --lumi=20 --xsec=0.2897   data/analysis/8tev/wh_vllljj.root \
data/delphes/wh_jjllvv/run_8tev/tag_*_delphes_events.root




# eft model:

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=20 --lumi=20 --xsec=149.8   data/analysis/8tev/hxx_lljjxx_mchi_1.root \
data/delphes/pass2_hxx_lljjxx/run_8tev_mchi1/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=21 --lumi=20 --xsec=149.8   data/analysis/8tev/hxx_lljjxx_mchi_10.root \
data/delphes/pass2_hxx_lljjxx/run_8tev_mchi10/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=22 --lumi=20 --xsec=149.8   data/analysis/8tev/hxx_lljjxx_mchi_100.root \
data/delphes/pass2_hxx_lljjxx/run_8tev_mchi100/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=23 --lumi=20 --xsec=149.8   data/analysis/8tev/hxx_lljjxx_mchi_500.root \
data/delphes/pass2_hxx_lljjxx/run_8tev_mchi500/tag_*_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=24 --lumi=20 --xsec=149.8   data/analysis/8tev/hxx_lljjxx_mchi_1000.root \
data/delphes/pass2_hxx_lljjxx/run_8tev_mchi1000/tag_*_delphes_events.root


# hzp model:

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=30 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp100_mchi1.root \
data/delphes/hzp/run_8tev_mzp100_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=31 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp100_mchi10.root \
data/delphes/hzp/run_8tev_mzp100_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=32 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp100_mchi100.root \
data/delphes/hzp/run_8tev_mzp100_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=33 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp100_mchi500.root \
data/delphes/hzp/run_8tev_mzp100_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=34 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp100_mchi1000.root \
data/delphes/hzp/run_8tev_mzp100_mchi1000/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=35 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp1000_mchi1.root \
data/delphes/hzp/run_8tev_mzp1000_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=36 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp1000_mchi10.root \
data/delphes/hzp/run_8tev_mzp1000_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=37 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp1000_mchi100.root \
data/delphes/hzp/run_8tev_mzp1000_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=38 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp1000_mchi500.root \
data/delphes/hzp/run_8tev_mzp1000_mchi500/tag_1_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=39 --lumi=20 --xsec=149.8   data/analysis/8tev/hzp_mzp1000_mchi1000.root \
data/delphes/hzp/run_8tev_mzp1000_mchi1000/tag_1_delphes_events.root

# hzpzp model:

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=40 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp100_mchi1.root \
data/delphes/hzpzp/run_8tev_mzp100_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=41 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp100_mchi10.root \
data/delphes/hzpzp/run_8tev_mzp100_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=42 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp100_mchi100.root \
data/delphes/hzpzp/run_8tev_mzp100_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=43 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp100_mchi500.root \
data/delphes/hzpzp/run_8tev_mzp100_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=44 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp100_mchi1000.root \
data/delphes/hzpzp/run_8tev_mzp100_mchi1000/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=45 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp1000_mchi1.root \
data/delphes/hzpzp/run_8tev_mzp1000_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=46 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp1000_mchi10.root \
data/delphes/hzpzp/run_8tev_mzp1000_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=47 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp1000_mchi100.root \
data/delphes/hzpzp/run_8tev_mzp1000_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=48 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp1000_mchi500.root \
data/delphes/hzpzp/run_8tev_mzp1000_mchi500/tag_1_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=49 --lumi=20 --xsec=149.8   data/analysis/8tev/hzpzp_mzp1000_mchi1000.root \
data/delphes/hzpzp/run_8tev_mzp1000_mchi1000/tag_1_delphes_events.root


# hsc model:


./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=50 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp100_mchi1.root \
data/delphes/hsc/run_8tev_mzp100_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=51 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp100_mchi10.root \
data/delphes/hsc/run_8tev_mzp100_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=52 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp100_mchi100.root \
data/delphes/hsc/run_8tev_mzp100_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=53 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp100_mchi500.root \
data/delphes/hsc/run_8tev_mzp100_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=54 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp100_mchi1000.root \
data/delphes/hsc/run_8tev_mzp100_mchi1000/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=55 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp1000_mchi1.root \
data/delphes/hsc/run_8tev_mzp1000_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=56 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp1000_mchi10.root \
data/delphes/hsc/run_8tev_mzp1000_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=57 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp1000_mchi100.root \
data/delphes/hsc/run_8tev_mzp1000_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=58 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp1000_mchi500.root \
data/delphes/hsc/run_8tev_mzp1000_mchi500/tag_1_delphes_events.root

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=59 --lumi=20 --xsec=149.8   data/analysis/8tev/hsc_mzp1000_mchi1000.root \
data/delphes/hsc/run_8tev_mzp1000_mchi1000/tag_1_delphes_events.root


# hhxx_combined

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=60 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_combined_mchi1.root \
data/delphes/hhxx_combined/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=61 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_combined_mchi10.root \
data/delphes/hhxx_combined/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=62 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_combined_mchi100.root \
data/delphes/hhxx_combined/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=63 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_combined_mchi500.root \
data/delphes/hhxx_combined/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=64 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_combined_mchi1000.root \
data/delphes/hhxx_combined/run_8tev_mchi1000/tag_1_delphes_events.root 

# hhxx_scalar

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=65 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi1.root \
data/delphes/hhxx_scalar/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=66 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi10.root \
data/delphes/hhxx_scalar/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=67 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi100.root \
data/delphes/hhxx_scalar/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=68 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi500.root \
data/delphes/hhxx_scalar/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=69 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi1000.root \
data/delphes/hhxx_scalar/run_8tev_mchi1000/tag_1_delphes_events.root 


# hhxx_scalar

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=65 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi1.root \
data/delphes/hhxx_scalar/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=66 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi10.root \
data/delphes/hhxx_scalar/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=67 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi100.root \
data/delphes/hhxx_scalar/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=68 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi500.root \
data/delphes/hhxx_scalar/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=69 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxx_scalar_mchi1000.root \
data/delphes/hhxx_scalar/run_8tev_mchi1000/tag_1_delphes_events.root 


# hhxg5x

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=70 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxg5x_mchi1.root \
data/delphes/hhxg5x/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=71 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxg5x_mchi10.root \
data/delphes/hhxg5x/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=72 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxg5x_mchi100.root \
data/delphes/hhxg5x/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=73 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxg5x_mchi500.root \
data/delphes/hhxg5x/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=74 --lumi=20 --xsec=149.8   data/analysis/8tev/hhxg5x_mchi1000.root \
data/delphes/hhxg5x/run_8tev_mchi1000/tag_1_delphes_events.root 


# xdx

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=75 --lumi=20 --xsec=149.8   data/analysis/8tev/xdx_mchi1.root \
data/delphes/xdx/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=76 --lumi=20 --xsec=149.8   data/analysis/8tev/xdx_mchi10.root \
data/delphes/xdx/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=77 --lumi=20 --xsec=149.8   data/analysis/8tev/xdx_mchi100.root \
data/delphes/xdx/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=78 --lumi=20 --xsec=149.8   data/analysis/8tev/xdx_mchi500.root \
data/delphes/xdx/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=79 --lumi=20 --xsec=149.8   data/analysis/8tev/xdx_mchi1000.root \
data/delphes/xdx/run_8tev_mchi1000/tag_1_delphes_events.root 


# xgx

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=80 --lumi=20 --xsec=149.8   data/analysis/8tev/xgx_mchi1.root \
data/delphes/xgx/run_8tev_mchi1/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=81 --lumi=20 --xsec=149.8   data/analysis/8tev/xgx_mchi10.root \
data/delphes/xgx/run_8tev_mchi10/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=82 --lumi=20 --xsec=149.8   data/analysis/8tev/xgx_mchi100.root \
data/delphes/xgx/run_8tev_mchi100/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=83 --lumi=20 --xsec=149.8   data/analysis/8tev/xgx_mchi500.root \
data/delphes/xgx/run_8tev_mchi500/tag_1_delphes_events.root 

./bin/MakeHxxTree --mllcut --maxevent=$MAXEVENTB --sample=84 --lumi=20 --xsec=149.8   data/analysis/8tev/xgx_mchi1000.root \
data/delphes/xgx/run_8tev_mchi1000/tag_1_delphes_events.root 

pushd data/analysis/8tev
if (-e all.root) rm all.root
hadd all.root *.root
popd
