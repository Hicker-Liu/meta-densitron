FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

# Enable ALSA and gstreamer support
PACKAGECONFIG_append_pn-qtmultimedia = " alsa gstreamer"
