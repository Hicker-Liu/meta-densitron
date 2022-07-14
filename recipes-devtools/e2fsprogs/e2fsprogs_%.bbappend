FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI += "file://e2fsck.conf"
PR = "r1"

do_install_append() {
	install -m 0644 ${WORKDIR}/e2fsck.conf ${D}${sysconfdir}/e2fsck.conf
}

# Add resize2fs tool

EXTRA_OECONF += "\
  --enable-resizer \
"


FILES_e2fsprogs-e2fsck += "${sysconfdir}/e2fsck.conf"

