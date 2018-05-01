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
in this layer, folder recipe-stigboard and recipe-robot are added on the bases of meta-duovero
recipe-stigboard contains drivers related to the board, like device tree, drivers, etc.
recipe-robot contains softwares, like argos, lua, speed controls

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

	goto meta-stig/scripts
	
	export OETMP=/xxx/build/tmp

	sudo umount /dev/sdb1

	sudo umount /dev/sdb2

	./copy-boot.sh sdb

	./copy-roots.sh sdb

