#!/bin/bash -f

MENU=/users/sharper/2017/egamma/exampleMenus/egPMv1/V4
GLOBALTAG=90X_upgrade2017_TSG_Hcal_V2
INPUT=/store/mc/PhaseIFall16DR/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130000/F42F8B0A-6E08-E711-915D-02163E01771F.root,/store/mc/PhaseIFall16DR/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130003/428FF9A0-360B-E711-9597-0025904B11B0.root,/store/mc/PhaseIFall16DR/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130001/BED942D5-FA07-E711-A27D-FA163E5A9B94.root,/store/mc/PhaseIFall16DR/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/120000/348555EB-0508-E711-8A8A-FA163E737A07.root,/store/mc/PhaseIFall16DR/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130000/B89E4935-110D-E711-B174-FA163E7712BE.root
PROCESS=TEST
#HLTPATH=HLTriggerFirstPath,HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v6,HLTriggerFinalPath 

hltGetConfiguration $MENU --globaltag $GLOBALTAG --input $INPUT \
    --setup  /dev/CMSSW_9_0_1/HLT  \
    --mc --process $PROCESS --full --offline --l1-emulator FullMC \
    --unprescale --max-events 5000 --output full > hlt_cfg_TEST_QCD_Full.py
    #--path $HLTPATH > hlt_cfg.py

