SUMMARY = "Capture Image recipe"
DESCRIPTION = "Custom recipe to build Capture Image application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# Where to find source files (can be local, GitHub, etc.)
SRC_URI = "file://bin file://lib file://etc/"

# Where to keep downloaded source files (in tmp/work/...)
S = "${WORKDIR}"

# Pass arguments to linker
FILES:${PN}         += "/usr/etc"
INSANE_SKIP:${PN}   += "already-stripped file-rdeps"
SOLIBS               = ".so"
FILES_SOLIBSDEV      = ""
RDEPENDS:${PN}      += "libopencv-core libopencv-imgproc libopencv-imgcodecs"

# Create /usr/bin in rootfs and copy program to it
do_install() {
    install -d ${D}/usr/etc
    cp -r etc/images/ ${D}/usr/etc/
    cp -r etc/config/ ${D}/usr/etc/
    cp -r etc/settings/ ${D}/usr/etc/

    install -d ${D}${libdir}
    install -m 0644 lib/libvimba_c.so ${D}${libdir}

    install -d ${D}${bindir}
    install -m 0755 bin/capture_image ${D}${bindir}
    install -m 0755 bin/capture_image.sh ${D}${bindir}
}

