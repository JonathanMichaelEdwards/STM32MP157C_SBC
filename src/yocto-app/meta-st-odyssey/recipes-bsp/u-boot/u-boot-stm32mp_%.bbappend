FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001-stm32mp1.h-flashing-boot-loader.patch \
            file://0002-modify-_deconfig-file-CONFIG_ENV_SIZE-0x4000-so-that.patch \
            file://0003-odyssey-u-boot-flash-from-usb.patch \
            file://0004-Add-pll1.patch \
            file://0006-fix-file-DFU.patch \
            file://0007-Modify-exlinux.conf-odyssey.patch \
            file://0008-Fix-eth-rst-issue.patch \
            file://0008-stm32mp157c-odyssey-dts.patch \
            file://0009-stm32mp157c-odyssey-som-dtsi.patch \
            file://0012-Comment-out-usb-power.patch \
            file://0014-add-ethclk-pinctrl.patch \
            file://0015-add-ethclk-ctrl.patch \
            "

