#!/bin/sh



#
# Update sample exchanger and autofocus
#
./autofocus 10

#
# Lighting, capture images and clear pagecache, dentries, and inodes
#
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -M; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -C; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches

#
# Detect for crystals
#
if [ "$ERR_CODE" -ne 0 ]; then
    echo "Error: $ERR_CODE"
else
    ./crystal_detection; ERR_CODE=$?

    STAMP=1
    mv /usr/etc/images/tmp/result_mono.bmp /usr/etc/images/tmp/result_mono_$STAMP.bmp
    mv /usr/etc/images/tmp/result_color.bmp /usr/etc/images/tmp/result_color_$STAMP.bmp
    mv /usr/etc/images/tmp/result_edge.bmp /usr/etc/images/tmp/result_edge_$STAMP.bmp
fi



#
# Update sample exchanger and autofocus
#
./autofocus 200

#
# Lighting, capture images and clear pagecache, dentries, and inodes
#
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -M; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches
sh capture_image.sh -C; ERR_CODE=$?
echo 3 > /proc/sys/vm/drop_caches

#
# Detect for crystals
#
if [ "$ERR_CODE" -ne 0 ]; then
    echo "Error: $ERR_CODE"
else
    ./crystal_detection; ERR_CODE=$?

    STAMP=2
    mv /usr/etc/images/tmp/result_mono.bmp /usr/etc/images/tmp/result_mono_$STAMP.bmp
    mv /usr/etc/images/tmp/result_color.bmp /usr/etc/images/tmp/result_color_$STAMP.bmp
    mv /usr/etc/images/tmp/result_edge.bmp /usr/etc/images/tmp/result_edge_$STAMP.bmp
fi


echo ">> $ERR_CODE"
exit $ERR_CODE

