#!/bin/bash
TIGEREX="../Examples/Hallway/hallway1-traps.POMDP"
LOG="log_hallway1_traps.txt"
HORIZON=300
for i in `seq 3000 50 5000`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 80 >> ${LOG}
done
