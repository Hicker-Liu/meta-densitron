FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
	file://weston.ini \
"

INI_UNCOMMENT_ASSIGNMENTS_remove_imx8mq-ds-163ap = " \
    size=1920x1080 \
    repaint-window=16 \
"
#INI_UNCOMMENT_ASSIGNMENTS_append_imx8mq-ds-163ap = " \
#    size=1920x287 \
#    \\[output\\] \
#    name=DSI-1 \
#    name=HDMI-A-1 \
#    mode=192x287 \
#    mode=1920x1080@60 \
#"

FILES_${PN} += " \
	${sysconfdir}/xdg/weston/weston.ini \
	${sysconfdir}/udev/rules/71-weston-drm.rules \
"
