#!/bin/sh

############################################################################
#                  Run the Capture Image application                       #
############################################################################
if [ "$1" = "-C" ]; then 
    echo "=== Camera will output a colored image ==="
    ./capture_image /usr/etc/settings/camera_settings_color.xml; ERR_CODE=$?
elif [ "$1" = "-M" ]; then
    echo "=== Camera will output a greyscale image ==="
    ./capture_image /usr/etc/settings/camera_settings_mono.xml; ERR_CODE=$?
else
    echo "Error: No command given"
    ERR_CODE=50
fi

if [ "$ERR_CODE" = 0 ]; then
    echo ""
elif [ "$ERR_CODE" = 253 ]; then
    echo ""
elif [ "$ERR_CODE" = 240 ]; then
    echo ">> Could not start Vimba [error code: -16]"
    echo ">> No transport layers were found - Need to initialise Vimba drivers..."
    echo ">> Exporting GENICAM_GENTL32_PATH"
    export GENICAM_GENTL32_PATH=:/usr/etc/config/
else
    echo "Error: $ERR_CODE"
fi


############################################################################
#                           Error still detected                           #
############################################################################
### Run the Capture Image program again if there was an issue previously ###
if [ "$ERR_CODE" -ne 0 ]; then
    if [ "$1" = "-C" ]; then 
        echo "=== Camera will output a colored image ==="
        ./capture_image /usr/etc/settings/camera_settings_color.xml; ERR_CODE=$?
    elif [ "$1" = "-M" ]; then
        echo "=== Camera will output a greyscale image ==="
        ./capture_image /usr/etc/settings/camera_settings_mono.xml; ERR_CODE=$?
    else
        echo "Error: No command given"
        ERR_CODE=50
    fi
fi

# Config and reboot the system if the Camera cannot start
if [ "$ERR_CODE" = 253 ]; then
    echo ""
elif [ "$ERR_CODE" = 240 ]; then
    echo ">> Could not start Vimba [error code: -16]"
    echo ">> No transport layers were found - Need to initialise Vimba drivers..."
    echo ">> Exporting GENICAM_GENTL32_PATH"
    sh /usr/etc/config/install.sh
    reboot
fi

echo "=================================================="
echo ">> [$ERR_CODE]"
echo "=================================================="
exit $ERR_CODE

