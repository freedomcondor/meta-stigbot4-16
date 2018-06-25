Weixu ZHU (Harry)
	zhuweixu_harry@126.com
-------------------------------------------------------------------------------------------
current state:
	nearly all the robot sensors are functional except the camera.
	the camera can send data in 2 lane mode, and omap4iss can successfully receive
	images from 1 lane. So we can have some blur pictures.
	if we switch the camera into 1 lane mode, an L3 error would occur, still working on that
-------------------------------------------------------------------------------------------

This layer depends on:

    URI: git://git.yoctoproject.org/poky.git
    branch: rocko
    commit: fdeecc9

    URI: git://git.openembedded.org/meta-openembedded
    branch: rocko
    commit: dacfa2b

If using mender

    URI: git://github.com/mendersoftware/meta-mender
    branch: rocko
    commit: cb4feea


meta-duovero layer maintainer: Scott Ellis <scott@jumpnowtek.com>

Instructions for using this layer can be found at [jumpnowtek.com][duovero-yocto-build]

[duovero-yocto-build]: http://www.jumpnowtek.com/yocto/Duovero-Systems-with-Yocto.html


-----------------------------------------------------------------------------------------
what has been changed based on meta-duovero:
	recipe-stigboard
	recipe-robot
		recipe-stigboard contains drivers related to the board, like device tree, drivers, etc.
		recipe-robot contains softwares, like argos, lua, speed controls
	
	recipes-connectivity/wpa-supplicant/files/wap_supplicant.conf-sane
		wifi support, the wifi ssid and password is defined here

--------------------------------------------------------------------------------------------

The tutorial in that link is not using a standard folder structure. To make it simple, you can:
1. clone poky-rocko

	~# git clone -b rocko git://git.yoctoproject.org/poky.git poky-rocko

2. in folder poky-rocko, clone layer openembedded
	
	~/poky-rocko$ git clone -b rocko git://git.openembedded.org/meta-openembedded

3. clone/copy/move this layer under poky-rocko, and change the folder name into meta-stigbot

	~/poky-rocko$ git clone https://github.com/freedomcondor/meta-stigbot4-16
	change folder name (remove 4-16)

	or simply:

	~/poky-rocko$ git clone https://github.com/freedomcondor/meta-stigbot4-16 meta-stigbot

4. prepare build by setup poky-rocko/build/conf:
	
	~/poky-rocko$ source poky-rocko/oe-init-build-env

5. copy sample files (bblayers.conf and local.conf) from meta-stigbot/conf to poky-rocko/build/conf

	you may need to check these two files to make sure all the paths written inside comply to your own

6. bitbake console-image-stig

7. copy image:

	(you may need to create a /media/card folder for the first time)

	goto meta-stig/scripts
	
	export OETMP=/xxx/build/tmp

	sudo umount /dev/sdb1

	sudo umount /dev/sdb2

	./copy-boot.sh sdb

	./copy-roots.sh sdb

