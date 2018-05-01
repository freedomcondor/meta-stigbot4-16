
SRC_URI += "file://stig.config"
# for menuconfig
#	1. low level debug, for uart debug

SRC_URI += "file://omap4-duovero-stig.dts;subdir=git/arch/${ARCH}/boot/dts"
KERNEL_DEVICETREE = "omap4-duovero-stig.dtb"
# for device tree
#	change device tree from omap4-duovero-parlor.dts to omap4-duovero-stig.dts
#	works together with u-boot_%.bbappend

#FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

#PACKAGE_ARCH = "${MACHINE_ARCH}"




