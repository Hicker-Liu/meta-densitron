DESCRIPTION = "Infineon Embedded Linux TPM Toolbox 2 for TPM 2.0"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${S}/License.txt;md5=1e492cfcb05c60002d4bee800bd9c296"

SRC_URI = "git://github.com/Infineon/eltt2;protocol=https;branch=master" 
SRCREV = "b677fc9ecfc5e896935d7b5e899472a18cadddae"

S = "${WORKDIR}/git"

do_compile() {
#	make
	 ${CC} eltt2.c ${LDFLAGS} -o eltt2
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/eltt2 ${D}${bindir}
}

#FILES_${PN} = "${systemd_unitdir}/system/*.service ${sysconfdir} ${bindir}"
