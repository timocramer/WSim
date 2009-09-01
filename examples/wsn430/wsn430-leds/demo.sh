#! /bin/sh

## set -x
source ../config.soft

## ==================================
export SETUI=true
TIME=$(( 30 * $FACTOR ))

WS1="`run_wsim $DS1 wsn430-leds.elf $TIME`" 

echo "${WS1}"
xterm -T wsim-1 -e "$WS1" &

read

## ==================================
## ==================================

kill_demo