---
layout: post
title: ISCSI����ʾ��
date: 2019-01-28 12:00
tags: iscsi lustre 
typora-root-url: ../../yangfeiffei.github.io

---




![](/images/iscsi-config-example/top.jpg)


## ���������

׼��һ��200M�Ĵ�����ΪMGT��

```bash
[root@iscsi ~]# lsblk /dev/sdb 
NAME MAJ:MIN RM SIZE RO TYPE MOUNTPOINT
sdb 8:16 0 200M 0 disk 
```

iscsi������������£�

```bash
[root@iscsi ~]# yum install targetcli
[root@iscsi ~]# systemctl enable targetd
[root@iscsi ~]# targetcli 
# ����һ�����豸�Ĵ��̣�������ļ�img�Ļ���Ҫʹ��/backstores/fileio
/> /backstores/block create mgt /dev/sdb 
# ����һ���յ�iqn
/> /iscsi create iqn.2019-01.f.com:mgt
# �����洴�����Է��ʡ�mgt����iqn��������io1��io2����������
/> /iscsi/iqn.2019-01.f.com:mgt/tpg1/acls create iqn.2019-01.f.cm:io1
/> /iscsi/iqn.2019-01.f.com:mgt/tpg1/acls create iqn.2019-01.f.cm:io2
# Ϊ��mgt������lun
/> /iscsi/iqn.2019-01.f.com:mgt/tpg1/luns create /backstores/block/mgt 
# �˳����Զ�����
/> exit
Global pref auto_save_on_exit=true
Last 10 configs saved in /etc/target/backup/.
Configuration saved to /etc/target/saveconfig.json
```

����������£�


![](/images/iscsi-config-example/iscsi-config.png)


## �ͻ�������

- io1���ã�

```bash
[root@io1 ~]# yum install iscsi-initiator-utils
# �޸ı���iqn��������ע����iscsi�������ϵ�iqn��ȫһ��
[root@io1 ~]# echo "InitiatorName=iqn.2019-01.f.com:io1" > /etc/iscsi/initiatorname.iscsi 
[root@io1 ~]# systemctl start iscsi
[root@io1 ~]# systemctl enable iscsi
# ����Ŀ��˵�iscsi����
[root@io1 ~]# iscsiadm -m discovery -t sendtargets -p 192.168.56.130
# ��¼������������ǵ�¼���ֵ����д���
[root@io1 ~]# iscsiadm -m node -L all
# Ҳ����ָ����¼
# iscsiadm -m node -T iqn.2019-01.f.com:mgt -l 
# ȡ����¼ʹ�ã�
# iscsiadm -m node -T iqn.2019-01.f.com:mgt -u
[root@io1 ~]# ls -l /dev/disk/by-id/ |grep sdc
lrwxrwxrwx 1 root root 9 Jan 28 15:05 scsi-36001405a6e718219a59410a82aa3069f -> ../../sdc
lrwxrwxrwx 1 root root 9 Jan 28 15:05 wwn-0x6001405a6e718219a59410a82aa3069f -> ../../sdc
```

- io2���ã�

```bash
[root@io2 ~]# yum install iscsi-initiator-utils
[root@io2 ~]# echo "InitiatorName=iqn.2019-01.f.com:io2" > /etc/iscsi/initiatorname.iscsi 
[root@io2 ~]# systemctl start iscsi
[root@io2 ~]# systemctl enable iscsi
[root@io2 ~]# iscsiadm -m discovery -t sendtargets -p 192.168.56.130
[root@io2 ~]# iscsiadm -m node -L all
[root@io2 ~]# ls -l /dev/disk/by-id/ |grep sdc
lrwxrwxrwx 1 root root 9 Jan 28 15:05 scsi-36001405a6e718219a59410a82aa3069f -> ../../sdc
lrwxrwxrwx 1 root root 9 Jan 28 15:05 wwn-0x6001405a6e718219a59410a82aa3069f -> ../../sdc
```

## �ο�
- https://blog.csdn.net/zhongbeida_xue/article/details/70921167
- http://blog.51cto.com/12227558/2096433
- https://blog.csdn.net/qq_40385970/article/details/78786023



