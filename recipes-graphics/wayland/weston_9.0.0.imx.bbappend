FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append += " \
	file://0010-weston-terminal-Add-cwd-and-command-options.patch \
"
