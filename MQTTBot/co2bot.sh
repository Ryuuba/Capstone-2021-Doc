#!/bin/bash
topic=$1
host=$2
delay=$3
echo "topic: "$topic
echo "delay: "$delay
echo "host: "$host
while :
do
  measure=$(($RANDOM%10+420))
  mosquitto_pub -t $topic -m $measure -h $host -d
  sleep $delay
done
