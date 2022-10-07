#!/bin/bash


for i in {0..2};
do
	port=$(( 9001+i ))
	echo "./wrk -t5 -c500 -d10s  -L http://192.168.232.254:${port}"
	./wrk -t5 -c1000 -d10s  -L http://192.168.232.254:9001 &
done
