# The imx-mkimage makefile requires a dtb file named (PLAT)-evk.dtb
# where PLAT is the platform name, such as imx8mq.
# Copying and renaming the EA dtb file.
do_compile_prepend_imx8mq-ds-163ap() {
	echo "Copying DTB"
	echo ${DEPLOY_DIR_IMAGE}
	echo ${BOOT_TOOLS}
	if [ -f ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/imx8mq-ds-163ap.dtb ]; then
	  cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/imx8mq-ds-163ap.dtb ${S}/iMX8M/imx8mq-evk.dtb
	fi
}
