# Copyright (C) 2017 DENSITRON TECHNOLOGIES Ltd.
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Linux Kernel provided by Densitron Embedded but based on NXP's kernel"
DESCRIPTION = "Linux Kernel for Densitron Embedded i.MX based SOM boards. The kernel is based on the kernel provided by NXP."

require recipes-kernel/linux/linux-imx.inc

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

SRCBRANCH = "ds_5.4.70_2.3.0"
SRCREV    = "${AUTOREV}"
SRC_URI   = "git://github.com/DensitronEmbedded/linux-densitron.git;protocol=git;branch=${SRCBRANCH}"

LOCALVERSION ?= ""

DEPENDS += "lzop-native bc-native firmware-imx"

DEFAULT_PREFERENCE = "1"

KERNEL_DEFCONFIG_imx8mq-ds-163ap = "${S}/arch/arm64/configs/imx_v8_ds_defconfig"

S = "${WORKDIR}/git"

addtask copy_defconfig after do_unpack before do_preconfigure

do_copy_defconfig() {
	install -d ${B}

#       cp ${KERNEL_DEFCONFIG} ${WORKDIR}/defconfig

	install -d ${B}
	mkdir -p ${B}
	cp ${KERNEL_DEFCONFIG} ${B}/.config
        cp ${KERNEL_DEFCONFIG} ${B}/../defconfig

        ## fetch the firmware blob
#        cp -arv ${COMPONENTS_DIR}/all/firmware-imx/lib/firmware/imx ${S}/firmware/ || die "no sdma-firmware found"
}

#do_configure_append () {
#	mkdir ${S}/firmware/imx
##	cp ${WORKDIR}/recipe-sysroot/${base_libdir}/firmware/imx ${S}/firmware/
#	oe_runmake ${MACHINE}_defconfi
#}

COMPATIBLE_MACHINE = "(mx8)"

EXTRA_OEMAKE_append_imx8mq-ds-163ap = "ARCH=arm64"
