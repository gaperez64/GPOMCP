#!/bin/bash
TIGEREX="../Examples/toytigerex.POMDP"
LOG="log_tigerex.txt"
HORIZON=100
for i in `seq 0 50`;
do
  for j in `seq 1 100`;
  do
    THRESH=$(echo "${i} * 0.5" | bc)
    echo "threshold = "${THRESH} >> ${LOG}
    ./dspomdp-bwc ${TIGEREX} ${THRESH} ${HORIZON} >> ${LOG}
  done
done
