

if [ -z "$1" ]
then
  echo "Error: No arguments provided.
        Usage: ./run_vanilla_exp NoOfServers"
  exit
fi

gcc main_epoll_vanilla.c -o helloworld_epoll_vanilla

sleep 1

noOfServers=$1

isLogOn=0 #0=off, 1=on
portNo=9001

sudo pkill hello
sleep 2

for i in `seq 1 $noOfServers`;
do
  echo "./epoll_vanilla $portNo $isLogOn"
   
  taskset -c $i ./epoll_vanilla $portNo $isLogOn & 
  portNo=$((portNo+1))
done
