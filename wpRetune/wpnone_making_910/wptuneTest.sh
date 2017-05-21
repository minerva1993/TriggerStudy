#!/bin/bash -f

MENU=/users/jiwonpark/WPtune/V27
GLOBALTAG=90X_upgrade2017_TSG_Hcal_V3
INPUT=/store/mc/PhaseIFall16DR/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130000/002CCEBB-7A0E-E711-B0AC-0025901FB188.root
PROCESS=WPTEST

hltGetConfiguration $MENU \
    --globaltag $GLOBALTAG --input $INPUT \
    --mc --process $PROCESS --full --offline \
    --l1-emulator FullMC --l1Xml L1Menu_Collisions2017_dev_r3.xml \
    --unprescale --max-events -1 --output none > check_V27.py
