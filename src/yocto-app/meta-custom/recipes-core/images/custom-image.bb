SUMMARY = "My custom Linux image."

IMAGE_INSTALL = "packagegroup-core-boot ${CORE_IMAGE_EXTRA_INSTALL}"

IMAGE_LINGUAS = " "

LICENSE = "MIT"

inherit core-image
inherit extrausers

# Set rootfs to 1000 MiB
IMAGE_OVERHEAD_FACTOR ?= "1.0"
#IMAGE_ROOTFS_SIZE = "200000"
IMAGE_ROOTFS_SIZE = "1024000"
IMAGE_ROOTFS_MAXSIZE = "2048000"

# Change root password (note the capital -P) - Uncomment to enable password (disable 'debug-tweaks')
#EXTRA_USERS_PARAMS = "\
#  usermod -P 'toor'  root \
#  "

IMAGE_INSTALL += "controller autofocus capture-image crystal-detection mcu-firmware"

