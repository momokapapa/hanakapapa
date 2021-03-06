#!/bin/bash

###############################################################
# -p <producing nodes count>
# -n <total nodes>
# -s <topology>
# -d <delay between nodes startup>
# -m # message storm test
# message storm test: "-m -p 21 -n 21 -d 5"
###############################################################

pnodes=10
topo=mesh
delay=0
debug="false"

message_storm="false"

args=`getopt p:n:s:d:lm $*`
if [ $? == 0 ]; then

    set -- $args
    for i; do
        case "$i"
        in
            -p) pnodes=$2;
                shift; shift;;
            -n) total_nodes=$2;
                shift; shift;;
            -d) delay=$2;
                shift; shift;;
            -s) topo="$2";
                shift; shift;;
            -l) debug="true"
                shift;;
            -m) message_storm="true"
                shift;;
            --) shift;
                break;;
        esac
    done
else
    echo "huh we got err $?"
    if [ -n "$1" ]; then
        pnodes=$1
        if [ -n "$2" ]; then
            topo=$2
            if [ -n "$3" ]; then
		total_nodes=$3
            fi
        fi
    fi
fi

total_nodes="${total_nodes:-`echo $pnodes`}"
launcherPath="programs/enumivo-launcher/enumivo-launcher"
clientPath="programs/enucli/enucli"

rm -rf etc/enumivo/node_* var/lib/* stage
ls -l etc/enumivo var/lib

debugArg=""
if [ "$debug" == true ]; then
   debugArg="--enunode \"--log-level-net-plugin debug\""
fi

cmd="$launcherPath -p $pnodes -n $total_nodes -s $topo -d $delay $debugArg"
if [ "$delay" == 0 ]; then
    cmd="$launcherPath -p $pnodes -n $total_nodes -s $topo"
fi
echo cmd: $cmd
eval $cmd

sleep 25
echo "start" > test.out
port=8888
endport=`expr $port + $total_nodes`
echo endport = $endport
while [ $port  -ne $endport ]; do
    cmd="$clientPath --port $port get block 20 >> test.out 2>&1;"
    echo cmd: $cmd
    eval $cmd
    port=`expr $port + 1`
done

grep 'producer"' test.out | tee summary | sort -u -k2 | tee unique
prodsfound=`wc -l < unique`
lines=`wc -l < summary`
if [ $lines -eq $total_nodes -a $prodsfound -eq 1 ]; then
    echo all synced
    if [ "$message_storm" == false ]; then
        cmd="$launcherPath -k 15"
        echo cmd: $cmd
        eval $cmd
        exit
    fi
fi

echo $lines reports out of $total_nodes and prods = $prodsfound
sleep 18
cmd="$clientPath --port 8888 get block 25"
echo cmd: $cmd
eval $cmd
sleep 15
cmd="$clientPath --port 8888 get block 30"
echo cmd: $cmd
eval $cmd
sleep 15
cmd="$clientPath --port 8888 get block 35"
echo cmd: $cmd
eval $cmd
sleep 15
cmd="$clientPath --port 8888 get block 40"
echo cmd: $cmd
eval $cmd
sleep 15
echo "pass 2" > test.out
port=8888
while [ $port  -ne $endport ]; do
    cmd="$clientPath --port $port get block 1 >> test.out 2>&1;"
    echo cmd: $cmd
    eval $cmd
    port=`expr $port + 1`
done


grep 'producer"' test.out | tee summary | sort -u -k2 | tee unique
prodsfound=`wc -l < unique`
lines=`wc -l < summary`
if [ $lines -eq $total_nodes -a $prodsfound -eq 1 ]; then
    echo all synced
    cmd="$launcherPath -k 15"
    echo cmd: $cmd
    eval $cmd
    exit
fi
echo ERROR: $lines reports out of $total_nodes and prods = $prodsfound
cmd="$launcherPath -k 15"
echo cmd: $cmd
eval $cmd

count=0
while [ $count -lt $total_nodes ]; do
    num=$(printf "%02s" $count)
    echo =================================================================
    echo Contents of etc/enumivo/node_${num}/config.ini:
    cat etc/enumivo/node_${num}/config.ini
    echo =================================================================
    echo Contents of var/lib/node_${num}/stderr.txt:
    cat var/lib/node_${num}/stderr.txt
    count=`expr $count + 1`
done

exit 1
