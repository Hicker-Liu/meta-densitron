# This appends to
# meta-imx/meta-sdk/dynamic-layers/qt5-layer/recipes-qt/qt5/qtbase_%.bbappend
# to add specific Qt5 environment variables to /etc/profile.d/qt5.sh

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# Enable GIF support
PACKAGECONFIG_append_pn-qtbase = " gif"

IMX_BACKEND = \
    "${@bb.utils.contains('DISTRO_FEATURES','x11',     'x11', \
        bb.utils.contains('DISTRO_FEATURES','wayland', 'wayland', \
                                                       'fb', d), d)}"
# Qt looks for fonts in /usr/lib/fonts
# OE installs ttf fonts to /usr/share/fonts/truetype
# as workaround we create a symlink

#do_install_append() {
#	ln -s /usr/share/fonts/truetype ${D}/usr/lib/fonts
#}

#FILES_${PN}_append = "/usr/lib/fonts"
