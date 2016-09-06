#!/bin/bash
TIGEREX="../Examples/Hallway/hallway1-traps.POMDP"
LOG="log_hallway1_traps.txt"
HORIZON=200
for i in `seq 0 50 5400`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 10 >> ${LOG}
done
