#!/bin/bash
pkill -f server 2>/dev/null
./server &
echo $! > /tmp/server.pid
sleep 1