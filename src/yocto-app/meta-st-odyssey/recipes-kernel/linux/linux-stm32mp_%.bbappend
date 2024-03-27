FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0001-stm32mp157c-odyssey-dts.patch \
            file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0002-stm32mp157c-odyssey-som-dtsi.patch \            
            file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0006-add-ethclk.patch \
            file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0007-add-ethclk-ctrl.patch \
            file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0008-usb-otg-disable-INT.patch \
           "

