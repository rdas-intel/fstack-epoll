#!/bin/bash
for i in {1..51}; 
	do 
		port=$(( 9000+i )) 
		echo "/original_pnp/f-stack/example/epoll_vanila ${port} 0 \"192.168.232.251\" &"
		/original_pnp/f-stack/example/epoll_vanila ${port} 0 "192.168.232.251" &

	done
