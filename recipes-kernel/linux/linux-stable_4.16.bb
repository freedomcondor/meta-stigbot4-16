require recipes-kernel/linux/linux-yocto.inc

COMPATIBLE_MACHINE = "duovero"

RDEPENDS_kernel-base += "kernel-devicetree"

KERNEL_DEVICETREE ?= "omap4-duovero-parlor.dtb"

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
"    
#    file://0004-media-Use-GPIO-controls-in-OV5640.patch 
#    file://0005-serdev-Support-multi-uart-controllers.patch 
#    file://0006-mfd-Add-support-for-the-BuilderBot-AVRs.patch 
#    file://0001-Debugging.patch 
#    file://enable-serdev.cfg 
#    file://enable-bb-avr-core.cfg 

