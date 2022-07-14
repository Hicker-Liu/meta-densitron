SUMMARY = "Install TPM SLB9670 service"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI = "file://slb9670.service"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -d ${D}${sysconfdir}/systemd/system/multi-user.target.wants
    install -m 0644 ${WORKDIR}/slb9670.service ${D}${systemd_system_unitdir}
    ln -sf  ${systemd_system_unitdir}/slb9670.service ${D}${sysconfdir}/systemd/system/multi-user.target.wants
}

FILES_${PN} += "${systemd_system_unitdir}/slb9670.service"

REQUIRED_DISTRO_FEATURES = "systemd"

