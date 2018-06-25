
# for menuconfig
#1. low level debug, for uart debug
#SRC_URI += "file://lowleveldebug.cfg"

#2. pca963x, for leds under i2c2
SRC_URI += "file://led-pca9632.cfg"
#	#result: should see 12 leds under /sys/class/leds/

#3. sc16is762, for uart 5,6 and 7,8
SRC_URI += "file://sc16is7xx.cfg"
#	#result: should see /dev/ttySC0-3

#4. pca9545 and vcnl4010, for rangefinder
SRC_URI += "file://pca954x.cfg"
SRC_URI += "file://vcnl4000.cfg"
SRC_URI += "file://0001-vcnl-match.patch"
#	#this patch is for adding a of_match_table for the probe function
#	#result: should see 12 iio:device under /sys/bus/iio/devices/

#5. omap4iss
SRC_URI += "file://v4l2_omap4iss.cfg"
SRC_URI += "file://devmem.cfg"
#	#create /dev/mem for rwmem debug
SRC_URI += "file://0001-omap4includechanged.patch"
SRC_URI += "file://0001-omap4issdriverchanged.patch"
SRC_URI += "file://0001-omap4iss_dtschanged.patch"

#6. camera
SRC_URI += "file://pca953x.cfg"
SRC_URI += "file://ov5640.cfg"
#	#ov5640.cfg depends on v4l2, so it has to be after v4l2_omap4iss.cfg
SRC_URI += "file://0001-ov5640driverchanged.patch"

# for device tree
#	change device tree from omap4-duovero-parlor.dts to omap4-duovero-stig.dts
#	works together with u-boot_%.bbappend
SRC_URI += "file://omap4-duovero-stig.dts;subdir=git/arch/${ARCH}/boot/dts"
KERNEL_DEVICETREE = "omap4-duovero-stig.dtb"

#FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

#PACKAGE_ARCH = "${MACHINE_ARCH}"




