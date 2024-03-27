#!/bin/sh

# TTY_PORT=ttyUSB0
TTY_PORT=ttyACM0

rm -f -r scripts/config.sh scripts/log/
mkdir scripts/log/
echo "! echo sleep 2" >> scripts/config.sh
echo "! echo cd /usr/bin/" >> scripts/config.sh
# echo "! echo sh stm32_usbotg_eth_config.sh restart" >> scripts/config.sh
gnome-terminal -- bash -c "minicom -D /dev/$TTY_PORT -b 115200 -S scripts/config.sh -C scripts/log/monitor.log; exec bash"
