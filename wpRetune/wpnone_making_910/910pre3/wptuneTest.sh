#!/bin/bash -f

MENU=/users/jiwonpark/WPtune/V14
GLOBALTAG=90X_upgrade2017_TSG_Hcal_V3
INPUT=root://eoscms.cern.ch//eos/cms/store/mc/PhaseIFall16DR/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130001/FE64C02E-A70E-E711-B368-FA163E1C071E.root
PROCESS=TEST
#HLTPATH=HLTriggerFirstPath,HLTriggerFinalPath,HLTAnalyzerEndpath

hltGetConfiguration $MENU \
    --globaltag $GLOBALTAG --input $INPUT \
    --mc --process $PROCESS --full --offline \
    --l1-emulator FullSimHcalTP  --l1Xml L1Menu_20170412.xml \
    --unprescale --max-events 1000 --output none > WPTest_DY.py
