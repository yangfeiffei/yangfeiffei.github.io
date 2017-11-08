---
layout: post
title: pxeserver install in rhel6.8
date: 2017-11-08 12:00
author: felo
tags: redhat pxe
---


# env

[root@pxeserver ~]# cat /etc/redhat-release
Red Hat Enterprise Linux Server release 6.8 (Santiago)
[root@pxeserver ~]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6


192.168.88.21 pxeserver
192.168.88.22 cpt01
192.168.88.23 cpt02

# set iptables selinux

[root@pxeserver ~]# /etc/init.d/iptables stop
iptables: Setting chains to policy ACCEPT: filter          [  OK  ]
iptables: Flushing firewall rules:                         [  OK  ]
iptables: Unloading modules:                               [  OK  ]
[root@pxeserver ~]# chkconfig iptables off

[root@pxeserver ~]# sed -i "s#SELINUX=enforcing#SELINUX=disabled#g" /etc/selinux/config
[root@pxeserver ~]# getenforce
Enforcing
[root@pxeserver ~]# setenforce 0
[root@pxeserver ~]# getenforce
Permissive


# install dhcp tftp httpd 

[root@pxeserver ~]# mount -t iso9660 /dev/sr0 /mnt/
mount: no medium found on /dev/sr0

[root@pxeserver ~]# cat /etc/yum.repos.d/rhel-source.repo
...
[rhel-local]
name=Red Hat Enterprise Linux $releasever Local Media
baseurl=file:///mnt
enabled=1
gpgcheck=0
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-redhat-beta,file:///etc/pki/rpm-gpg/RPM-GPG-KEY-redhat-release

[root@pxeserver yum.repos.d]# yum install dhcp -y
[root@pxeserver yum.repos.d]# rpm -ql dhcp |grep dhcpd.conf
/etc/dhcp/dhcpd.conf
/usr/share/doc/dhcp-4.1.1/dhcpd-conf-to-ldap
/usr/share/doc/dhcp-4.1.1/dhcpd.conf.sample
/usr/share/man/man5/dhcpd.conf.5.gz
[root@pxeserver yum.repos.d]# vim /etc/dhcp/dhcpd.conf
[root@pxeserver ~]# cat /etc/dhcp/dhcpd.conf
#
# DHCP Server Configuration file.
#   see /usr/share/doc/dhcp*/dhcpd.conf.sample
#   see 'man 5 dhcpd.conf'
#
subnet 192.168.88.0 netmask 255.255.255.0 {
        range 192.168.88.100 192.168.88.200;
        option subnet-mask 255.255.255.0;
        #next-server 192.168.88.21;
        #filename "/pxelinux.0";
       }

group  {
        filename "/rhel6.8/pxelinux.0";
        next-server 192.168.88.21;

        host cpt01 { hardware ethernet 00:50:56:23:F9:14;fixed-address 192.168.88.22;}
       }

group  {
        filename "/centos7.0/pxelinux.0";
        next-server 192.168.88.21;

        host cpt02 { hardware ethernet 00:50:56:33:DC:57;fixed-address 192.168.88.23;}
       }

[root@pxeserver yum.repos.d]# /etc/init.d/dhcpd start
Starting dhcpd:                                            [  OK  ]
[root@pxeserver yum.repos.d]# netstat -nltup| grep dhcpd
udp        0      0 0.0.0.0:67                  0.0.0.0:*                               31437/dhcpd

[root@pxeserver yum.repos.d]# yum install tftp-server tftp
Loaded plugins: product-id, search-disabled-repos, security, subscription-manager
This system is not registered to Red Hat Subscription Management. You can use subscription-manager to register.
Setting up Install Process
Package tftp-server-0.49-8.el6.x86_64 already installed and latest version
Package tftp-0.49-8.el6.x86_64 already installed and latest version
Nothing to do
[root@pxeserver yum.repos.d]# rpm -ql tftp-server |grep tftp
/etc/xinetd.d/tftp
/usr/sbin/in.tftpd
/usr/share/doc/tftp-server-0.49
/usr/share/doc/tftp-server-0.49/CHANGES
/usr/share/doc/tftp-server-0.49/README
/usr/share/doc/tftp-server-0.49/README.security
/usr/share/doc/tftp-server-0.49/README.security.tftpboot
/usr/share/man/man8/in.tftpd.8.gz
/usr/share/man/man8/tftpd.8.gz
/var/lib/tftpboot
[root@pxeserver yum.repos.d]# vim /etc/xinetd.d/tftp
[root@pxeserver yum.repos.d]# cat /etc/xinetd.d/tftp |grep disable
        disable                 = no
[root@pxeserver yum.repos.d]# /etc/init.d/xinetd restart
Stopping xinetd:                                           [FAILED]
Starting xinetd:                                           [  OK  ]
[root@pxeserver yum.repos.d]# netstat -nltup |grep tftp
[root@pxeserver yum.repos.d]# netstat -nltup |grep xinetd
udp        0      0 0.0.0.0:69                  0.0.0.0:*                               31485/xinetd

[root@pxeserver yum.repos.d]# yum install httpd -y
Loaded plugins: product-id, search-disabled-repos, security, subscription-manager
This system is not registered to Red Hat Subscription Management. You can use subscription-manager to register.
Setting up Install Process
Package httpd-2.2.15-53.el6.x86_64 already installed and latest version
Nothing to do
[root@pxeserver yum.repos.d]# rpm -ql httpd |grep httpd.conf
/etc/httpd/conf
/etc/httpd/conf.d
/etc/httpd/conf.d/README
/etc/httpd/conf.d/welcome.conf
/etc/httpd/conf/httpd.conf
/etc/httpd/conf/magic
[root@pxeserver yum.repos.d]# vim /etc/httpd/conf/httpd.conf
[root@pxeserver yum.repos.d]# cat /etc/httpd/conf/httpd.conf |grep "^ServerName"
ServerName 127.0.0.1:80

[root@pxeserver yum.repos.d]# /etc/init.d/httpd restart
Stopping httpd:                                            [  OK  ]
Starting httpd:                                            [  OK  ]

[root@pxeserver html]# yum -y install syslinux
[root@pxeserver html]# cp /usr/share/syslinux/pxelinux.0 /var/lib/tftpboot/
[root@pxeserver html]# cp /var/www/html/repo/isolinux/* /var/lib/tftpboot/
[root@pxeserver html]# mkdir -p /var/lib/tftpboot/pxelinux.cfg
[root@pxeserver html]# cp /var/www/html/repo/isolinux/isolinux.cfg /var/lib/tftpboot/pxelinux.cfg/default
[root@pxeserver ~]# cd /var/lib/tftpboot/
[root@pxeserver tftpboot]# tree
.
├── centos7.0
│   ├── boot.msg
│   ├── initrd.img
│   ├── pxelinux.0
│   ├── pxelinux.cfg
│   │   ├── default
│   │   └── default.bak
│   ├── vesamenu.c32
│   └── vmlinuz
└── rhel6.8
    ├── initrd.img
    ├── pxelinux.0
    ├── pxelinux.cfg
    │   ├── 01-00-50-56-23-f9-14
    │   └── default.bak
    └── vmlinuz

[root@pxeserver tftpboot]# cat rhel6.8/pxelinux.cfg/01-00-50-56-23-f9-14
default kslinux
prompt 0

label kslinux
  menu label ^Install system with kickstart
  kernel vmlinuz
  append initrd=initrd.img ks=http://192.168.88.21/ksconfig/rhel6.8-ks.cfg ksdevice=eth0

[root@pxeserver html]# cd /var/www/html
[root@pxeserver html]# tree
.
├── centos7.0
	...
├── ksconfig
│   ├── anaconda-ks.cfg
│   ├── centos7.0-ks.cfg
│   └── rhel6.8-ks.cfg
└── rhel6.8
	...


# cankao 
http://www.boobooke.com/m/dl.php?vid=5358
这个确实做得人比较少.前几天boobooke看赶星的一个xen/kvm的视频,里面讲Pxe的时候,提到用grub4dos的pxe引导代码替换pexlinux.0那个文件,引导封装好的Pe的img,你可以找找看看

