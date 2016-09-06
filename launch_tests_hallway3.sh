#!/bin/bash
TIGEREX="../Examples/Hallway/hallway3.POMDP"
LOG="log_hallway3.txt"
HORIZON=7000
for i in `seq 0 50 3750`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 10 >> ${LOG}
done
