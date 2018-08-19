#!/usr/bin/env bash

kill -9 `cat program.pid`
rm program.pid
/home/benjamin/Apps/expandr/expandr & echo $! > program.pid
