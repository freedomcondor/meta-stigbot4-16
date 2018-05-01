DESCRIPTION = "Overwrite /etc/network/interfaces file to switch on wifi auto connection"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://interfaces"

S = "${WORKDIR}"

do_install() {
        install -d ${D}/etc/network
	install -m 0755 interfaces ${D}/etc/network
}
