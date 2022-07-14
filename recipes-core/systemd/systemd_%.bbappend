FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
	file://10-eth0.dhcp \
	file://10-eth0.static \
	file://10-eth1.dhcp \
	file://10-eth1.static \
	file://10-can0.network \
"
PACKAGECONFIG_append = " networkd"

def ipaddr_to_cidr(iface, d):
    ipaddr = d.getVar('%s_STATIC_IP' % iface.upper(), True)
    netmask = d.getVar('%s_STATIC_NETMASK' % iface.upper(), True)
    binary_str = ''
    for byte in netmask.split('.'):
        binary_str += bin(int(byte))[2:].zfill(8)
    return ipaddr + '/' + str(len(binary_str.rstrip('0')))


ETH0_MODE ?= "dhcp"
ETH1_MODE ?= "dhcp"
ETH0_STATIC_CIDR = "${@ipaddr_to_cidr('eth0', d)}"
ETH1_STATIC_CIDR = "${@ipaddr_to_cidr('eth1', d)}"   

do_install_append() {
	
	# The network files need to be in /usr/lib/systemd, not ${systemd_unitdir}...
	install -d ${D}${prefix}/lib/systemd/network/
	# eth0
	install -m 0644 ${WORKDIR}/10-eth0.${ETH0_MODE} ${D}${prefix}/lib/systemd/network/10-eth0.network 

	sed -i -e "s,##ETH0_STATIC_CIDR##,${ETH0_STATIC_CIDR},g" \
		-e "s,##ETH0_STATIC_GATEWAY##,${ETH0_STATIC_GATEWAY},g" \
                -e "s,##ETH0_STATIC_DNS##,${ETH0_STATIC_DNS},g" \
                ${D}${prefix}/lib/systemd/network/10-eth0.network 

	# eth1
	install -m 0644 ${WORKDIR}/10-eth1.${ETH1_MODE} ${D}${prefix}/lib/systemd/network/10-eth1.network 

	sed -i -e "s,##ETH1_STATIC_CIDR##,${ETH1_STATIC_CIDR},g" \
		-e "s,##ETH1_STATIC_GATEWAY##,${ETH1_STATIC_GATEWAY},g" \
		-e "s,##ETH1_STATIC_DNS##,${ETH1_STATIC_DNS},g" \
		${D}${prefix}/lib/systemd/network/10-eth1.network 
	# can
	install -m 0644 ${WORKDIR}/10-can0.network ${D}${prefix}/lib/systemd/network/10-can0.network
}

FILES_${PN} += " \
	${nonarch_base_libdir}/systemd/network \
"
