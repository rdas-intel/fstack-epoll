#!/bin/bash


#all servers are launched at 9001 and interface defined by epoll.ini

./echo_epoll --conf epoll.ini --proc-type=primary --proc-id=0 9001 &
for i in {1..3}; do port=$(( 9001+i )); ./echo_epoll --conf epoll.ini --proc-type=secondary --proc-id=${i}  9001 & done
