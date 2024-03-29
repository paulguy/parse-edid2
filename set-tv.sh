#!/bin/sh

MONITOR_MODE=2560x1440
MONITOR_RATE=59.95
TV_MODE=720x480
TV_RATE=59.94
SCRIPT_PATH=/home/paul/projects/parse-edid2/parse-edid2/parse-edid2/match-display.py

MONITOR_CONNECTOR=$(${SCRIPT_PATH} name="lg ultragear+")
TV_CONNECTOR=$(${SCRIPT_PATH} name=macrosilicon)

function error {
    zenity --error --text="${1}"
    exit
}

if [ x${MONITOR_CONNECTOR} = x ]; then
    error "Couldn't find monitor."
fi

if [ x${TV_CONNECTOR} = x ]; then
    error "Couldn't find TV."
fi

#Set main display to 60 hz so they can coexist.
XRANDR=$(xrandr --output ${MONITOR_CONNECTOR} --mode ${MONITOR_MODE} --rate ${MONITOR_RATE})
if [ x${XRANDR} != x ]; then
    error ${XRANDR}
fi

#Enable TV
XRANDR=$(xrandr --output ${TV_CONNECTOR} --mode ${TV_MODE} --rate ${TV_RATE} --right-of ${MONITOR_CONNECTOR})
if [ x${XRANDR} != x ]; then
    error ${XRANDR}
fi
