DESCRIPTION = "The pwm-beep test tools"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

RDEPENDS_${PN} = "systemd"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI = " \
    file://beep.c \
    file://beep.service \
    "

S = "${WORKDIR}"


do_compile() {
	${CC} beep.c ${LDFLAGS} -o beep
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/beep ${D}${bindir}
	install -d ${D}${systemd_unitdir}/system/
	install -d ${D}${sysconfdir}/systemd/system/sysinit.target.wants/
	install -m 0644 ${WORKDIR}/beep.service ${D}${systemd_unitdir}/system/beep.service
	ln -sf ${systemd_unitdir}/system/beep.service \
                      ${D}${sysconfdir}/systemd/system/sysinit.target.wants/beep.service
}


SYSTEMD_SERVICE_${PN} = "beep.service"

FILES_${PN} = "${systemd_unitdir}/system/*.service ${sysconfdir} ${bindir}"

# As this package is tied to systemd, only build it when we're also building systemd.
python () {
    if not bb.utils.contains ('DISTRO_FEATURES', 'systemd', True, False, d):
        raise bb.parse.SkipPackage("'systemd' not in DISTRO_FEATURES")
}
