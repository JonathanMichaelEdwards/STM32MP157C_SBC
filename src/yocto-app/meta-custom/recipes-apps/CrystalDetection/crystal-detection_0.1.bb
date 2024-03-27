SUMMARY = "Crystal Detection recipe"
DESCRIPTION = "Custom recipe to build Crystal Detection application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Where to find source files (can be local, GitHub, etc.)
SRC_URI = "file://bin"

# Where to keep downloaded source files (in tmp/work/...)
S = "${WORKDIR}/bin"

# Pass arguments to linker
RDEPENDS:${PN}      += "libopencv-core libopencv-imgproc libopencv-imgcodecs"

# Create /usr/bin in rootfs and copy program to it
do_install() {
    install -d ${D}${bindir}
    install -m 0755 crystal_detection ${D}${bindir}
}

