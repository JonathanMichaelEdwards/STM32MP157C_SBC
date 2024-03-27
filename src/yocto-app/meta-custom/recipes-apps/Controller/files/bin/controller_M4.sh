#!/bin/sh

# ============================================================================================
#
#  FILE: controller.sh
#  Author:       Jonathan Edwards
#  Date:         24/11/2022, 28/07/2023
# 
#  Description:  
# 				Running Cortex M4 application demo 
#                   - M4 running
#                   - A7 sleeps while the M4 is running 
#
#  ============================================================================================

if [ "$1" = "stop" ]; then
    echo stop > /sys/class/remoteproc/remoteproc0/state
    exit 0
else
    FILE_NAME=$1
fi


# Create a M4 firmware directory it one doesn't exist
if [ ! -d "/lib/firmware/" ]; then
    mkdir /lib/firmware/
fi

cd /lib/firmware/
if [ -f "/usr/bin/$FILE_NAME" ]; then
    mv /usr/bin/$FILE_NAME /lib/firmware/
fi

# Enable Wakeup Sources
echo enabled > /sys/devices/platform/soc/40010000.serial/tty/ttySTM0/power/wakeup
echo enabled > /sys/devices/platform/soc/40010000.serial/power/wakeup
echo enabled > /sys/devices/platform/soc/40013000.i2c/power/wakeup

# Stop, load, and run M4 application
echo stop > /sys/class/remoteproc/remoteproc0/state
echo $FILE_NAME > /sys/class/remoteproc/remoteproc0/firmware
echo start > /sys/class/remoteproc/remoteproc0/state

# # A7 Active blocking
# sleep 5

# # Set the A7 to sleep for 10 seconds and wakeup
# echo "A7 DOWN..."
# echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +10 > /sys/class/rtc/rtc0/wakealarm; echo mem > /sys/power/state
# echo "A7 UP..."

