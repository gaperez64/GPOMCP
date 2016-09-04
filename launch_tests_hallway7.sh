#!/bin/bash
TIGEREX="../Examples/Hallway/hallway7.POMDP"
LOG="log_hallway7.txt"
HORIZON=1000
for i in `seq 0 98`;
do
  for j in `seq 1 100`;
  do
    THRESH=${i}
    echo "threshold = "${THRESH} >> ${LOG}
    ./dspomdp-bwc ${TIGEREX} ${THRESH} ${HORIZON} >> ${LOG}
  done
done
