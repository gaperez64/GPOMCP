#!/bin/bash
TIGEREX="../Examples/Hallway/hallway5.POMDP"
LOG="log_hallway5.txt"
HORIZON=5000
for i in `seq 0 98`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 10 >> ${LOG}
done
