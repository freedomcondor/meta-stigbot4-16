
# for menuconfig
#1. low level debug, for uart debug
SRC_URI += "file://stig.config"

#2. pca963x, for leds under i2c2
SRC_URI += "file://led-pca9632.cfg"
#SRC_URI += "file://driver_debug/0001-pca963x-debug.patch"
#	#this patch is for drive debug, add a print mark in probe function
#	#result: should see 12 leds under /sys/class/leds/

#3. sc16is762, for uart 5,6 and 7,8
SRC_URI += "file://sc16is7xx.cfg"
#	#result: should see /dev/ttySC0-3

#4. pca9545 and vcnl4010, for rangefinder
#	#to do

#5. camera
#	#to do

# for device tree
#	change device tree from omap4-duovero-parlor.dts to omap4-duovero-stig.dts
#	works together with u-boot_%.bbappend
SRC_URI += "file://omap4-duovero-stig.dts;subdir=git/arch/${ARCH}/boot/dts"
KERNEL_DEVICETREE = "omap4-duovero-stig.dtb"

#FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

#PACKAGE_ARCH = "${MACHINE_ARCH}"




