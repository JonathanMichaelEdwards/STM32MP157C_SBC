#!/bin/sh

# ============================================================================================
#
#  FILE: controller_A7.sh
#  Author:       Jonathan Edwards
#  Date:         24/11/2022, 28/07/2023
# 
#  Description:  
# 				Cortex A7 two image capturing and processing cycles demo.  
#                   - Capture - Changing Autofocus length and capturing Mono plus colored images
#                   - Crystal detection - Demo use the Canny edge detection algorithm to
#                      determine the light and dark regions of the image.
#                   - Storage - `/usr/etc/images/`
#                   - Error detecting
#
# 					Process:
#                           1. Enable the Camera, Liquid Lens, and startup wait time for 
#                               the Camera.
#                           2. Capture images and clear pagecache, dentries, and inodes
#                           3. Disable the Camera and Liquid Lens
#                           4. Detect for crystals (image processing)
#                           5. Cycle back to task 1 
#
#  ============================================================================================


echo "#######################################"
echo "# Stating cycle 1"

# Create a tmp image directory if it doesn't exist
if [ ! -d "/usr/etc/images/tmp/" ]; then
    mkdir /usr/etc/images/tmp/
fi

# Enable the Camera, Liquid Lens, and startup wait time for the Camera
gpioset gpiochip3 4=1; sleep 6
./autofocus 10

#
# Capture images and clear pagecache, dentries, and inodes
#
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -M; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -C; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches

# Disable the Camera and Liquid Lens
gpioset gpiochip3 4=0;

#
# Detect for crystals
#
if [ "$ERR_CODE" -ne 0 ]; then
    echo "Error: $ERR_CODE"
else
    echo "Starting Image Processing on Image 1..."
    ./crystal_detection; ERR_CODE=$?
    echo "Completed Image Processing on Image 1..."

    STAMP=1
    mv /usr/etc/images/tmp/result_mono.bmp /usr/etc/images/result_mono_$STAMP.bmp
    mv /usr/etc/images/tmp/result_color.bmp /usr/etc/images/result_color_$STAMP.bmp
    mv /usr/etc/images/tmp/result_edge.bmp /usr/etc/images/result_edge_$STAMP.bmp
fi

echo "# Finished cycle 1"
echo "#######################################"


echo ">> $ERR_CODE"


echo "#######################################"
echo "# Starting cycle 2"

# Enable the Camera, Liquid Lens, and startup wait time for the Camera
gpioset gpiochip3 4=1; sleep 6
./autofocus 200

#
# Lighting, capture images and clear pagecache, dentries, and inodes
#
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -M; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -C; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches

# Disable the Camera and Liquid Lens
gpioset gpiochip3 4=0;

#
# Detect for crystals
#
if [ "$ERR_CODE" -ne 0 ]; then
    echo "Error: $ERR_CODE"
else
    echo "Starting Image Processing on Image 2..."
    ./crystal_detection; ERR_CODE=$?
    echo "Completed Image Processing on Image 2..."

    STAMP=2
    mv /usr/etc/images/tmp/result_mono.bmp /usr/etc/images/result_mono_$STAMP.bmp
    mv /usr/etc/images/tmp/result_color.bmp /usr/etc/images/result_color_$STAMP.bmp
    mv /usr/etc/images/tmp/result_edge.bmp /usr/etc/images/result_edge_$STAMP.bmp
fi
rm -r /usr/etc/images/tmp/

echo "# Finished cycle 2"
echo "#######################################"

echo ">> $ERR_CODE"
exit $ERR_CODE

