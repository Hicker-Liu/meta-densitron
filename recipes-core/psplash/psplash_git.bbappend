FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
	file://psplash-colors.h \
	file://0001-psplash-config-enable-fullscreen-image.patch \
	file://psplash-shutdown.service \
"

SPLASH_IMAGES = "file://psplash-densitron-img.png;outsuffix=default"

do_configure_append () {
	cd ${S}
	cp ../psplash-colors.h ./
}

do_install_append() {
	install -d ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/psplash-shutdown.service ${D}${systemd_unitdir}/system/psplash-shutdown.service
}

SYSTEMD_SERVICE_${PN} += "psplash-shutdown.service"

FILES_${PN} += " \
	${systemd_unitdir}/system/psplash-shutdown.service \
"

python () {
    if not bb.utils.contains ('DISTRO_FEATURES', 'systemd', True, False, d):
        raise bb.parse.SkipPackage("'systemd' not in DISTRO_FEATURES")
}
