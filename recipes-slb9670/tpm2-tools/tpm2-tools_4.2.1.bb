FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SUMMARY = "Tools for TPM2."
DESCRIPTION = "tpm2-tools"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://doc/LICENSE;md5=a846608d090aa64494c45fc147cc12e3"
SECTION = "tpm"

DEPENDS = "tpm2-abrmd tpm2-tss openssl curl autoconf-archive"

SRC_URI = " \
	https://github.com/tpm2-software/${BPN}/releases/download/${PV}/${BPN}-${PV}.tar.gz \
"

SRC_URI[md5sum] = "a8fbfc4a2a584d4a6ada8b51a738953a"
SRC_URI[sha256sum] = "1ba32a5824e40bee18637c0b323943deff2fb085b07a4edffd0e045a520809c0"

inherit autotools pkgconfig bash-completion
