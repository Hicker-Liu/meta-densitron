FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append_imx8mq-ds-163ap = " \
                           file://0001-fix-libdrm-related-configure-test-failure.patch \
"
