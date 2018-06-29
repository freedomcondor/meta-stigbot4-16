require recipes-kernel/linux/linux-yocto.inc

COMPATIBLE_MACHINE = "duovero"

RDEPENDS_kernel-base += "kernel-devicetree"

#KERNEL_DEVICETREE ?= "omap4-duovero-parlor.dtb"
# device tree switched to omap4-duovero-stig.dtb

LINUX_VERSION = "4.16"
LINUX_VERSION_EXTENSION = "-jumpnow"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-stable-${LINUX_VERSION}:"

S = "${WORKDIR}/git"

PV = "4.16.5"
SRCREV = "e5ce9f6879d3fe20435f34dfd86fb76c36072916"
SRC_URI = " \
    git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
    file://0001-spidev-Add-spidev-compatible-id.patch \
    file://0002-mwifiex-Skip-unsupported-CHAN_REGION_CFG-cmd.patch \
    file://0003-mwifiex-Remove-unsupported-GTK_REKEY_OFFLOAD_CFG-cmd.patch \
    file://clk-ti-fix-flag-space-conflict-with-clkctrl-clocks.patch \
    file://reset-ti-rstctrl-use-the-reset-simple-driver.patch \
    file://defconfig \
    file://led-pca9632.cfg \
    file://sc16is7xx.cfg \
    file://pca954x.cfg \
    file://vcnl4000.cfg \
    file://0004-vcnl-match.patch \
    file://v4l2_omap4iss.cfg \
    file://devmem.cfg \
    file://0005-omap4iss_driver_dtsi.patch \
    file://pca953x.cfg \
    file://ov5640.cfg \
    file://0006-ov5640driver.patch \
"    

SRC_URI += "file://omap4-duovero-stig.dts;subdir=git/arch/${ARCH}/boot/dts"
KERNEL_DEVICETREE = "omap4-duovero-stig.dtb"
# for device tree
#	change device tree from omap4-duovero-parlor.dts to omap4-duovero-stig.dts
#	works together with u-boot_%.bbappend

#Harry's description:
#1. led-pca9632.cfg
#       #for leds under i2c2
#       #result: should see 12 leds under /sys/class/leds/
#2. sc16is7xx.cfg
#       #sc16is762 for uart 5,6 and 7,8
#       #result: should see /dev/ttySC[0-3]
#3. pca954x.cfg 
#   vcnl4000.cfg
#   0004-vcnl-match.patch
#       #pca9545 and vcnl4010for rangefinder
#	#the patch is to add a of_match_table for the probe function
#	#result: should see 12 iio:device under /sys/bus/iio/devices/    
#4. v4l2_omap4iss.cfg
#   devmem.cfg
#   0005-omap4iss_driver_dtsi.patch0005-omap4iss_driver_dtsi.patch 
#       #create /dev/mem for rwmem debug
#       #for omap4iss driver and omap4.dtsi
#5. pca953x.cfg
#   ov5640.cfg
#   0006-ov5640driver.patch
#       #ov5640.cfg depends on v4l2, so it has to be after v4l2_omap4iss.cfg
#       #for the driver of ov5640
#6. 
																																																																																																																																																																																															

