#!/bin/bash
kill $(cat /tmp/server.pid 2>/dev/null) 2>/dev/null
rm -f /tmp/server.pid