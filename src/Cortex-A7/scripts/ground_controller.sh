#!/bin/sh

#
#   * Capturing image example script for testing communication between Linux server and stm32mp1 client machines.
#   * The server has complete control over the client 
#       (i.e., The host can wake up the stm32mp1 over the UART channel 4 - which is being used).
#           1. Reset Devices
#           2. Capture Image
#               - Success = 0 
#           3. Process Image
#               - Success = 0 
#           4. Config Ethernet
#               - Success = 0 
#           5. Request Image from client machine
#               - Success = 0 
#           6. Disable Ethernet
#               - Success = 0 
#           7. If Success - loop back to (1)
#


RUN=SerialCommunication/build/serial_communication


# Reset & Disable
printf "Resetting devices...\n"
fuser -k /dev/ttyACM0
echo "New log..." > scripts/log/monitor.log

# Capture Image
echo 
echo "--- Capturing Image ---"
echo "========================================================================================================="
${RUN} -L -t 10000; ERR_CODE=$?

if [ $ERR_CODE = 0 ]; then
    printf "\n\n>> Image captured successfully"
elif [ $ERR_CODE = 16 ]; then
    printf "\n\n>> No transport layers were found"
else
    printf "\n\nError code: %i\n" "$ERR_CODE"
fi
printf "\n_________________________________________________________________________________________\n"

# Config Ethernet & Request Image
echo "--- Requesting Image ---"
echo "========================================================================================================="
if [ $ERR_CODE = 0 ]; then
    printf "\nError code status: %d\n" $ERR_CODE
    scp -r root@"$1":/usr/etc/images/ ./
    printf "\n_________________________________________________________________________________________\n"
    printf "\n>> Successfully Sent Image...\n"
else
    printf "\n\n>> Failed to Capture Image...\n"
fi

