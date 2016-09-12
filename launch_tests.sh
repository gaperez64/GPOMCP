#!/bin/bash
TIGEREX="../Examples/toytigerex.POMDP"
LOG="log_tigerex.txt"
HORIZON=100
for i in `seq 0 50`;
do
  THRESH=$(echo "${i} * 0.5" | bc)
  echo "threshold = "${THRESH} >> ${LOG}
  ./test-sim ${TIGEREX} ${THRESH} ${HORIZON} 1000 >> ${LOG}
done
