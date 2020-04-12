#/bin/bash

./basePhyLayer -c Test1 > out.tmp
./basePhyLayer -c Test2 >> out.tmp

diff -I '^Assigned runID=' -I '^Loading NED files from' -w exp-output out.tmp

rm -f out.tmp


# Test configurations with TestBaseDecider as Decider

./basePhyLayer -c TestBaseDecider1 > out.tmp

diff -I '^Assigned runID=' -I '^Loading NED files from' -w exp-output_tBD out.tmp

rm -f out.tmp


