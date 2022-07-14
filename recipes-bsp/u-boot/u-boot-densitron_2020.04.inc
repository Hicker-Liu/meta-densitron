# Copyright (C) 2017 DENSITRON TECHNOLOGIES Ltd.

DESCRIPTION = "U-Boot for Densitron Embedded i.MX based boards."

DEPENDS += "flex-native bison-native"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRCBRANCH = "ds_v2020.04_5.4.70_2.3.0"
SRCREV    = "${AUTOREV}"
SRC_URI   = "git://github.com/DensitronEmbedded/uboot-densitron.git;branch=${SRCBRANCH}"

S = "${WORKDIR}/git"

inherit fsl-u-boot-localversion

LOCALVERSION ?= ""

PACKAGE_ARCH = "${MACHINE_ARCH}"
