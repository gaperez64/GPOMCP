#!/bin/bash
TIGEREX="../Examples/rockSample/rsD3M4G3_S1432_O168_U150.POMDP"
LOG="log_rocksample.txt"
HORIZON=100
for i in `seq 0 7 109`;
do
  THRESH=${i}
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 50 >> ${LOG}
done
