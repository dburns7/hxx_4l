import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(


#'file:CrabFiles/GluGluToZZTo4L/outfile_1_1_WEX.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_2_1_PYi.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_3_1_nGO.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_4_1_aUG.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_5_1_Hl2.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_6_1_y32.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_7_1_U6c.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_8_1_eBx.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_9_1_woN.root',
#'file:CrabFiles/GluGluToZZTo4L/outfile_10_1_rak.root'


#'file:CrabFiles/ZZTo2e2mu/outfile_1_1_koQ.root',
#'file:CrabFiles/ZZTo2e2mu/outfile_2_1_cnq.root',
#'file:CrabFiles/ZZTo2e2mu/outfile_3_1_dpG.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_4_1_h13.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_5_1_oG6.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_6_1_zcg.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_7_1_IgQ.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_8_1_G9i.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_9_1_d0i.root ',
#'file:CrabFiles/ZZTo2e2mu/outfile_10_1_H6k.root'


#CrabFiles/GluGluToHToZZTo4L:
#outfile_10_1_DvK.root  outfile_13_1_Gya.root  outfile_16_1_4sE.root  outfile_19_1_NRx.root  outfile_2_1_KXZ.root  outfile_5_1_YQZ.root  outfile_8_1_Iei.root
#outfile_11_1_9yh.root  outfile_14_1_RAa.root  outfile_17_1_Rmz.root  outfile_1_1_QDW.root   outfile_3_1_PhS.root  outfile_6_1_BgA.root  outfile_9_1_lm8.root
#outfile_12_1_ite.root  outfile_15_1_9Eb.root  outfile_18_1_3YN.root  outfile_20_1_cMc.root  outfile_4_1_snJ.root  outfile_7_1_4yd.root
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_1_1_QDW.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_2_1_KXZ.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_3_1_PhS.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_4_1_snJ.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_5_1_YQZ.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_6_1_BgA.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_7_1_4yd.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_8_1_Iei.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_9_1_lm8.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_10_1_DvK.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_11_1_9yh.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_12_1_ite.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_13_1_Gya.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_14_1_RAa.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_15_1_9Eb.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_16_1_4sE.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_17_1_Rmz.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_18_1_3YN.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_19_1_NRx.root',
#'file:CrabFiles/GluGluToHToZZTo4L/outfile_20_1_cMc.root'
#
#
#CrabFiles/VBF_HToZZTo4L:
#outfile_10_1_Fck.root  outfile_13_1_FKM.root  outfile_16_1_SGC.root  outfile_19_1_VcP.root  outfile_2_1_zPj.root  outfile_5_1_IJR.root  outfile_8_1_oaz.root
#outfile_11_1_Szv.root  outfile_14_1_6OH.root  outfile_17_1_1JX.root  outfile_1_1_O6D.root   outfile_3_1_A18.root  outfile_6_1_37J.root  outfile_9_1_Tm8.root
#outfile_12_1_uuM.root  outfile_15_1_Iow.root  outfile_18_1_GMl.root  outfile_20_1_xlD.root  outfile_4_1_gGy.root  outfile_7_1_HrD.root
#
#CrabFiles/WH_HToZZTo4L:
#outfile_10_1_PP0.root  outfile_13_1_9fp.root  outfile_16_1_4fN.root  outfile_19_1_78m.root  outfile_2_1_3ac.root  outfile_5_1_ibU.root  outfile_8_1_Twx.root
#outfile_11_1_Vlq.root  outfile_14_1_kqq.root  outfile_17_1_a8s.root  outfile_1_1_kGQ.root   outfile_3_1_kNI.root  outfile_6_1_90R.root  outfile_9_1_oNO.root
#outfile_12_1_gbA.root  outfile_15_1_ZKA.root  outfile_18_1_N8D.root  outfile_20_1_gOv.root  outfile_4_1_rFL.root  outfile_7_1_bKG.root
#'file:CrabFiles/WH_HToZZTo4L/outfile_1_1_kGQ.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_2_1_3ac.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_3_1_kNI.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_4_1_rFL.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_5_1_ibU.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_6_1_90R.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_7_1_bKG.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_8_1_Twx.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_9_1_oNO.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_10_1_PP0.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_11_1_Vlq.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_12_1_gbA.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_13_1_9fp.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_14_1_kqq.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_15_1_ZKA.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_16_1_4fN.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_17_1_a8s.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_18_1_N8D.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_19_1_78m.root',
#'file:CrabFiles/WH_HToZZTo4L/outfile_20_1_gOv.root'

#CrabFiles/ZH_HToZZTo4L:
#CrabFiles/ZH_HToZZTo4L:
#outfile_10_1_oCM.root  outfile_13_1_6CU.root  outfile_16_1_4SG.root  outfile_19_1_j2K.root  outfile_2_1_OEq.root  outfile_5_1_9os.root  outfile_8_1_gEk.root
#outfile_11_1_gWE.root  outfile_14_1_5TI.root  outfile_17_1_IEf.root  outfile_1_1_Uww.root   outfile_3_1_AsH.root  outfile_6_1_tIb.root  outfile_9_1_GxY.root
#outfile_12_1_20h.root  outfile_15_1_7FF.root  outfile_18_1_7cu.root  outfile_20_1_gPw.root  outfile_4_1_cEK.root  outfile_7_1_Vpn.root
#'file:CrabFiles/ZH_HToZZTo4L/outfile_1_1_Uww.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_2_1_OEq.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_3_1_AsH.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_4_1_cEK.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_5_1_9os.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_6_1_tIb.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_7_1_Vpn.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_8_1_gEk.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_9_1_GxY.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_10_1_oCM.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_11_1_gWE.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_12_1_20h.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_13_1_6CU.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_14_1_5TI.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_15_1_7FF.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_16_1_4SG.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_17_1_IEf.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_18_1_7cu.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_19_1_j2K.root',
#'file:CrabFiles/ZH_HToZZTo4L/outfile_20_1_gPw.root'

#
#
#CrabFiles/ZZTo4e:
#outfile_10_1_zQP.root  outfile_2_1_o0e.root  outfile_4_1_sTt.root  outfile_6_1_QvD.root  outfile_8_1_HZ9.root
#outfile_1_1_64x.root   outfile_3_1_G02.root  outfile_5_1_CM7.root  outfile_7_1_Anw.root  outfile_9_1_Jp6.root
#'file:CrabFiles/ZZTo4e/outfile_1_1_64x.root',
#'file:CrabFiles/ZZTo4e/outfile_2_1_o0e.root',
#'file:CrabFiles/ZZTo4e/outfile_3_1_G02.root',
#'file:CrabFiles/ZZTo4e/outfile_4_1_sTt.root',
#'file:CrabFiles/ZZTo4e/outfile_5_1_CM7.root',
#'file:CrabFiles/ZZTo4e/outfile_6_1_QvD.root',
#'file:CrabFiles/ZZTo4e/outfile_7_1_Anw.root',
#'file:CrabFiles/ZZTo4e/outfile_8_1_HZ9.root',
#'file:CrabFiles/ZZTo4e/outfile_9_1_Jp6.root',
#'file:CrabFiles/ZZTo4e/outfile_10_1_zQP.root'
#
#CrabFiles/ZZTo4mu:
#outfile_10_1_P9b.root  outfile_2_1_e3L.root  outfile_4_1_M9i.root  outfile_6_1_Xns.root  outfile_8_1_sNm.root
#outfile_1_1_JfK.root   outfile_3_1_cZQ.root  outfile_5_1_LJU.root  outfile_7_1_ZfF.root  outfile_9_1_pPF.root
#'file:CrabFiles/ZZTo4mu/outfile_1_1_JfK.root',
#'file:CrabFiles/ZZTo4mu/outfile_2_1_e3L.root',
#'file:CrabFiles/ZZTo4mu/outfile_3_1_cZQ.root',
#'file:CrabFiles/ZZTo4mu/outfile_4_1_M9i.root',
#'file:CrabFiles/ZZTo4mu/outfile_5_1_LJU.root',
#'file:CrabFiles/ZZTo4mu/outfile_6_1_Xns.root',
#'file:CrabFiles/ZZTo4mu/outfile_7_1_ZfF.root',
#'file:CrabFiles/ZZTo4mu/outfile_8_1_sNm.root',
#'file:CrabFiles/ZZTo4mu/outfile_9_1_pPF.root',
#'file:CrabFiles/ZZTo4mu/outfile_10_1_P9b.root'

#CrabFiles/GluGluToHToWWTo2LAndTau2Nu:
#outfile_10_1_yDR.root  outfile_13_1_LtR.root  outfile_16_1_14w.root  outfile_19_1_PWS.root  outfile_2_1_YsZ.root  outfile_5_1_pG3.root  outfile_8_1_Ymu.root
#outfile_11_1_RVD.root  outfile_14_1_8KJ.root  outfile_17_1_RuJ.root  outfile_1_1_IGC.root   outfile_3_1_Qde.root  outfile_6_1_llK.root  outfile_9_1_pQz.root
#outfile_12_1_4d0.root  outfile_15_1_iOT.root  outfile_18_1_W9j.root  outfile_20_1_LFM.root  outfile_4_1_IIR.root  outfile_7_1_ChL.root
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_1_1_IGC.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_2_1_YsZ.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_3_1_Qde.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_4_1_IIR.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_5_1_pG3.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_6_1_llK.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_7_1_ChL.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_8_1_Ymu.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_9_1_pQz.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_10_1_yDR.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_11_1_RVD.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_12_1_4d0.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_13_1_LtR.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_14_1_8KJ.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_15_1_iOT.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_16_1_14w.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_17_1_RuJ.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_18_1_W9j.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_19_1_PWS.root',
#'file:CrabFiles/GluGluToHToWWTo2LAndTau2Nu/outfile_20_1_LFM.root'

#outfile_10_1_hbT.root  outfile_15_1_dbn.root  outfile_21_1_Ige.root  outfile_7_1_frj.root
#outfile_11_1_EsG.root  outfile_16_1_i0j.root  outfile_2_1_bVc.root   outfile_8_1_qtj.root
#outfile_1_1_YB8.root   outfile_17_1_JzM.root  outfile_3_1_hDQ.root   outfile_9_1_hil.root
#outfile_12_1_pg0.root  outfile_18_1_mf1.root  outfile_4_1_SLK.root
#outfile_13_1_T3r.root  outfile_19_1_OqY.root  outfile_5_1_rRw.root
#outfile_14_1_BL2.root  outfile_20_1_tsL.root  outfile_6_1_8YP.root

#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_1_1_YB8.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_2_1_bVc.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_3_1_hDQ.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_4_1_SLK.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_5_1_rRw.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_6_1_8YP.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_7_1_frj.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_8_1_qtj.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_9_1_hil.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_10_1_hbT.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_11_1_EsG.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_12_1_pg0.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_13_1_T3r.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_14_1_BL2.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_15_1_dbn.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_16_1_i0j.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_17_1_JzM.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_18_1_mf1.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_19_1_OqY.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_20_1_tsL.root',
#'file:CrabFiles/WH_ZH_TTH_HToWW/outfile_21_1_Ige.root'

'file:data/CMSSW/8TeV/WH_ZH_TTH_HToWW/outfile_21_1_Ige.root'

  )
)

process.MessageLogger = cms.Service("MessageLogger")

process.analyzeBasicPat = cms.EDAnalyzer("MyZPeakAnalyzer",

  electronSrc = cms.untracked.InputTag("cleanPatElectrons"),
  muonSrc     = cms.untracked.InputTag("cleanPatMuons"),
  metSrc      = cms.untracked.InputTag("patMETs")                                             
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('WH_ZH_TTH_HToWW.root')
                                   )

process.p = cms.Path(process.analyzeBasicPat)


