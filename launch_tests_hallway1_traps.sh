#!/bin/bash
TIGEREX="../Examples/Hallway/hallway1-traps.POMDP"
LOG="log_hallway1_traps.txt"
HORIZON=300
for i in `seq 3500 50 4550`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 50 >> ${LOG}
done
