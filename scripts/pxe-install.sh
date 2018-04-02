#!/bin/bash

IP="192.168.88.21"
ISO_ALI="vsip"
ISO_NAME="VSIP-4.2-AVIC-x86_64.iso"
WORK_DIR="/root"

# 0.pre-install

## change ip address
cat>/etc/sysconfig/network-scripts/ifcfg-eth0 <<EOF
DEVICE=eth0
TYPE=Ethernet
ONBOOT=yes
BOOTPROTO=static
IPADDR=$IP
PREFIX=24
NAME=eth0
EOF

/etc/init.d/network restart

## create local repo
mount -t iso9660 /dev/sr0 /mnt
rm -f /etc/yum.repo.d/*
cat>/etc/yum.repos.d/cdrom.repo <<EOF
[rhel-local]
name=local
baseurl=file:///mnt/
enabled=1
gpgcheck=0
EOF
yum clean all && yum makecache

## disable selinux
sed -i 's/^SELINUX=.*/SELINUX=disabled/' /etc/selinux/config
setenforce 0

## disable firewalld
service iptables stop
chkconfig iptables off

# 1.install httpd service
yum -y install httpd tftp-server dhcp
chkconfig httpd on
chkconfig xinetd on
chkconfig dhcpd on

## for dhcp config file
IP_SUBNET=`echo $IP|cut -d '.' -f 1-3`
cat>/etc/dhcp/dhcpd.conf <<EOF
subnet $IP_SUBNET.0 netmask 255.255.255.0 {
        range $IP_SUBNET.100 $IP_SUBNET.200;
        option subnet-mask 255.255.255.0;
        next-server $IP;
        filename "/$ISO_ALI/pxelinux.0";
       }
#group  {
#        filename "/vsip/pxelinux.0";
#        next-server 192.168.88.21;
#        host test3 { hardware ethernet 52:54:00:DF:58:60;fixed-address 192.168.88.99;}
#       }
EOF

echo "DHCPDARGS=eth0" >/etc/sysconfig/dhcpd

/etc/init.d/dhcpd restart

## for tftp-server config

sed -i '/disable/s/yes/no/' /etc/xinetd.d/tftp 
/etc/init.d/xinetd restart


# 2.pxe config
## mount iso
mkdir -p /var/www/html/$ISO_ALI
echo "mount -o loop $WORK_DIR/$ISO_NAME /var/www/html/$ISO_ALI" >> /etc/rc.local
mount -o loop $WORK_DIR/$ISO_NAME /var/www/html/$ISO_ALI

## copy isolinux file
mkdir -p /var/lib/tftpboot/$ISO_ALI
cp -r /var/www/html/$ISO_ALI/isolinux/* /var/lib/tftpboot/$ISO_ALI
mkdir /var/lib/tftpboot/$ISO_ALI/pxelinux.cfg
 #cp /var/lib/tftpboot/$ISO_ALI/isolinux.cfg /var/lib/tftpboot/$ISO_ALI/pxelinux.cfg/default
cp $WORK_DIR/pxelinux.0 /var/lib/tftpboot/vsip/

## change ks file
mkdir -p /var/www/html/ksconfig/
cp /var/lib/tftpboot/$ISO_ALI/all.ks /var/www/html/ksconfig/$ISO_ALI.ks
sed -i 's/cdrom/#cdrom/' /var/www/html/ksconfig/$ISO_ALI.ks
URL="url --url http://$IP/$ISO_ALI"
sed -i '/cdrom/a$URL' /var/www/html/ksconfig/$ISO_ALI.ks


## change grub cfg
cat>/var/lib/tftpboot/$ISO_ALI/pxelinux.cfg/default << EOF
label node
  menu label Install VSIP 4.2 -auto
  kernel vmlinuz
  append initrd=initrd.img inst.ks=http://$IP/ksconfig/$ISO_ALI.ks quiet
EOF







