#!/bin/bash -f

MENU=/users/sharper/2017/egamma/exampleMenus/egPMv1/V4
GLOBALTAG=90X_upgrade2017_TSG_Hcal_V2
INPUT=/store/mc/PhaseIFall16DR/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130003/428FF9A0-360B-E711-9597-0025904B11B0.root
PROCESS=TEST
#HLTPATH=HLTriggerFirstPath,HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v6,HLTriggerFinalPath 

hltGetConfiguration $MENU --globaltag $GLOBALTAG --input $INPUT \
    --setup  /dev/CMSSW_9_0_1/HLT  \
    --mc --process $PROCESS --full --offline --l1-emulator FullMC \
    --unprescale --max-events 1000 --output full > hlt_cfg_TEST_QCD_30to50.py
    #--path $HLTPATH > hlt_cfg.py

