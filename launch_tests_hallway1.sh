#!/bin/bash
TIGEREX="../Examples/Hallway/hallway1.POMDP"
LOG="log_hallway1.txt"
HORIZON=2500
for i in `seq 0 50 6300`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 10 >> ${LOG}
done
