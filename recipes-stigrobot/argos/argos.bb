DESCRIPTION = "argos installation"

LICENSE = "CLOSED"
BB_STRICT_CHECKSUM = "0"

DEPENDS += "lua"
#DEPENDS += "asciidoc"

inherit cmake

SRC_URI = "git://github.com/ilpincy/argos3;protocol=http \
file://0001-Cflags.patch \
"

#file://0001-positionofcore.patch 

SRCREV = "${AUTOREV}"
S = "${WORKDIR}/git/src"

EXTRA_OECMAKE = "-DARGOS_BUILD_FOR=stigbot -DARGOS_DOCUMENTATION=OFF"

FILES_${PN} += "${libdir}/*"	
#/usr/lib
FILES_${PN} += "${prefix}/doc/*"  	
#/usr/doc
FILES_${PN} += "${datadir}/*"		
#/usr/shared
TARGET_CC_ARCH += "${LDFLAGS}" 





