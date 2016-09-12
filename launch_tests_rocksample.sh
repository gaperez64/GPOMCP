#!/bin/bash
TIGEREX="../Examples/rockSample/rsD2M2G1_S52_O29_U25.POMDP"
LOG="log_rocksample.txt"
HORIZON=500
for i in `seq 0 1 14`;
do
  THRESH=$(echo "${i} * 0.5" | bc)
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 100 >> ${LOG}
done
