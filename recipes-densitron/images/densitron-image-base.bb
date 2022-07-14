SUMMARY = "A basic image for Densitron SOM boards"
LICENSE = "MIT"

inherit core-image

IMAGE_FEATURES += " \
	splash \
	ssh-server-openssh \
	ssh-server-dropbear \
	debug-tweaks \
	tools-profile \
	tools-sdk \
	tools-debug \
	package-management \
	nfs-server \
	hwcodecs \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', '', \
           bb.utils.contains('DISTRO_FEATURES',     'x11', 'x11-base x11-sato', \
                                                       '', d), d)} \
"
# Specifies the list of GLIBC locales to generate should you not wish generate all LIBC locals, which can be time consuming.
GLIBC_GENERATE_LOCALES = " \
	en_GB.UTF-8 \
	en_US.UTF-8 \
	zh_CN.UTF-8 \
	zh_TW.UTF-8 \
"

# Specifies the list of locales to install into the image during the root filesystem construction process.
IMAGE_LINGUAS += " \
	en-us \
	en-gb \
	zh-cn \
	zh-tw \
"

IMAGE_INSTALL += " \
	${CORE_IMAGE_BASE_INSTALL} \
	packagegroup-core-full-cmdline \
	packagegroup-fsl-tools-audio \
	packagegroup-fsl-tools-gpu \
	packagegroup-fsl-tools-gpu-external \
	packagegroup-fsl-tools-benchmark \
	packagegroup-fsl-tools-testapps \
	packagegroup-imx-security \
	packagegroup-fsl-gstreamer1.0 \
	packagegroup-fsl-gstreamer1.0-full \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'weston-init', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'weston-examples', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11 wayland', 'weston-xwayland xterm', '', d)} \
"
#       packagegroup-tools-bluetooth 
#       packagegroup-fsl-tools-testapps

IMAGE_INSTALL += " \
	u-boot-fw-utils \
	i2c-tools-misc \
	i2c-tools \
	can-utils \
	iproute2 \
	alsa-utils \
	alsa-tools \
	ethtool \
	iperf3 \
	usbutils \
	evtest \
	fb-test \
	fbida \
	v4l-utils \
	mmc-utils \
	cifs-utils \
	usbutils \
	lshw \
	mesa-demos \
"

IMAGE_INSTALL += " \
	coreutils \
	rng-tools \
	lsb-release \
	bash-completion \
	bash-completion-extra \
	htop \
	tmux \
	tree \
	nano \
	tzdata \
	rsync \
	dosfstools \
	net-tools \
	udev-extraconf \
	e2fsprogs-mke2fs \
	e2fsprogs-tune2fs \
	e2fsprogs-resize2fs \
	openssh-sftp-server \
	dhcp-client \
"

IMAGE_INSTALL += " \
	beep \
	slb9670 \
	eltt2 \
	tpm2-abrmd \
	tpm2-tools \
	tpm2-tss \
"

inherit populate_sdk_qt5

IMAGE_INSTALL += " \
    packagegroup-qt5-imx \    
"

IMAGE_INSTALL += " \
	chromium-ozone-wayland \
"

IMAGE_INSTALL += " \
	fio \
	smartmontools \
	hdparm \
	iperf3 \
	lmbench \
	memtester \
	stress \
	stressapptest \	
"
