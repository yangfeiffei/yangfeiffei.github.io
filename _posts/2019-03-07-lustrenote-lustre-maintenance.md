---
layout: post
title: Lustreѧϰ�ʼǣ�6.1��- ά��Lustre
date: 2019-03-07 12:00
tags: lustre 
typora-root-url: ..
---



## 1. �������OSTs

����ʱ�ų������OSTs��

```bash
# �ڿͻ��˹��� ���߹���MDT��ʱ��ִ�У����OST��ʱ��ʹ��ð�ŷָ�
client# mount -o exclude=testfs-OST0000 -t lustre uml1:/testfs /mnt/testfs
# ������OST����client��ִ��
lctl --device 7 activate  # ò�Ʋ��У���ȷ��
```

## 2. �ҵ�lustre�ļ�ϵͳ�еĽڵ�

��ʱ����Ҫ�ҵ�ϵͳ������OST�����֣�

```bash
# ����1����mgs��ִ�У�
MGS# lctl get_param mgs.MGS.live.*  # ���������ʾ���е�mdt��ost���������Ҳ����ʾ
# ����2����client�����У�
client# lfs osts   # �о�����osts
client# lfs mdts   # �о�����mdts
# ����3����mds��ִ��
mds:/root# lctl get_param lov.*-mdtlov.target_obd # ��ʾ��������osts
```


## 3. ����lustre����ķ�ʽ���ط�����

���ʹ��MGT/MDT���ʹ�õ�����£���ֻ��Ҫ����MGS��������MDSʱ��

```bash
mount -t lustre /dev/mdt_partition -o nosvc /mount_point
# �ٸ����ӣ�testfs-MDT0000ͬʱ������mgt��mdt
$ mount -t lustre -L testfs-MDT0000 -o nosvc /mnt/test/mdt
```


## 4. �ؽ�lustre������־

���lustreϵͳ������־�����ļ�ϵͳ���ܹ��ص�״̬����Ҫʹ��tunefs.lustre --writeconf���ؽ����ǡ�ִ����writeconf֮��������־�ᱻ�ؽ����ļ�ϵͳ�൱��һ��ȫ�µ��ļ�ϵͳ������������MGS�ϡ�
ֻ�����������������ִ��writeconf��
- The configuration logs are in a state where the file system cannot start
- A server NID is being changed

**writeconf���û��ƻ��������ã���ost ����Ϣ��conf_param�ĵ������ã�����writeconfҪ����ʹ�á����齫ost�����ú�conf_param����ִ�нű�����ÿ��writeconf���ú�ִ�����ýű����ɡ����⣬ʹ��set_param -Pִ�е����ûᱻ������**

������־����ʹ���������

```bash
mgs# lctl --device MGS llog_print fsname-client
mgs# lctl --device MGS llog_print fsname-MDT0000
mgs# lctl --device MGS llog_print fsname-OST0000
```

�ؽ�lustre�ļ�ϵͳ������־�������£�
- ֹͣ�����ļ�ϵͳ����umount client--mdt--ost��ע�Ᵽ��mgs����״̬��
- ��֤���е�mdt��ost�����ӣ����ǲ����أ�
-  tunefs.lustre --writeconf ִ�����нڵ㣻
- ����mount���нڵ㣻

```bash
psh cn1 umount /fs00
psh io2 umount /fs00/mdt000
psh io3 umount /fs00/*
psh io4 umount /fs00/*
psh cn1,io1-io4 df -ht lustre 
psh io2 tunefs.lustre --writeconf mdtpool/mdt000
psh io3 tunefs.lustre --writeconf ost0pool/ost000
psh io3 tunefs.lustre --writeconf ost1pool/ost001
psh io4 tunefs.lustre --writeconf ost2pool/ost002
psh io3 tunefs.lustre --writeconf ost3pool/ost003
psh io4 tunefs.lustre --writeconf ost3pool/ost003
psh io2 mount.lustre mdtpool/mdt000 /fs00/mdt000
psh io3 mount.lustre ost0pool/ost000 /fs00/ost000
psh io3 mount.lustre ost1pool/ost001 /fs00/ost001
psh io4 mount.lustre ost2pool/ost002 /fs00/ost002
psh io4 mount.lustre ost3pool/ost003 /fs00/ost003
psh cn1 mount.lustre io1@tcp:/fs00 /fs00
```


## 5. �޸ķ�������nid

��2.4�汾֮ǰ��ֻ��ʹ��writeconf�ķ�ʽ�����ģ�����2.4�汾֮��������һ��replace_nids ������������ٵ��޸ķ�������nid��writeconf�ķ�ʽ��Ȼ���á�


�������£�

- ��˳��umout���з���

- �޸�/etc/modprobe.d/lustre.conf����ʹ��lnetctl�������޸�nid������ʹ��lctl list_nids���鿴nid��

- ��֤mgs��������״̬��

- ʹ������������޸ģ�

```bash

lctl replace_nids devicename nid1[,nid2,nid3 ...]
```

- ���mgt��mdt��ͬһ���̵Ļ�����Ҫ����ж��mgt�����¹��أ�



## 7. ����һ��mdt

�����mdt�������ӵ��ļ�ϵͳ�У�
- �����ļ�ϵͳ�п��Դ����ļ���������
- ����Ԫ���ݾۺ����ܣ�
- ����ĳЩӦ�õ�Ԫ������Ϣ��

����mdt�ķ������£�
- �ҵ�����mdt index

```bash
client$ lctl dl | grep mdc 
```
- ����һ��mdt

```bash
mds# mkfs.lustre --reformat --fsname=testfs --mdt --mgsnode=mgsnode --index 4 /dev/sdd
```
- ����mdt

```bash
mds# mount.lustre /dev/sdd /mnt/mdt4
```
- ������Ҫ������Ŀ¼

```bash
client# lfs mkdir -i 3 /mnt/testfs/new_dir_on_mdt3  # ʹ��index=3��mdt
client# lfs mkdir -i 4 /mnt/testfs/new_dir_on_mdt4  # ʹ��mdt4
client# lfs mkdir -c 4 /mnt/testfs/new_directory_striped_across_4_mdts  # ����һ��4 mdt���������ļ�
```

## 8. ����ost

Ϊ�˱�֤���ܵ�ƽ�⣬�������ڲ�ͬ��OSS�����ò�ͬ��OST������

���裺
- ����ost������

```bash
oss# mkfs.lustre --fsname=testfs --mgsnode=mds16@tcp0 --ost --index=12 /dev/sda
oss# mkdir -p /mnt/testfs/ost12
oss# mount -t lustre /dev/sda /mnt/testfs/ost12 
```

- ƽ��ost�Ŀ��пռ�

```bash
# �ļ�ϵͳ������ost���½����ļ����Զ�д���µ�ost�ϣ�
# �����Ҫ�ֶ��ƶ���Щ�ļ���ָ����ost�Ͽ���ʹ��lfs_migrate����
client# lfs_migrate /mnt/lustre/dir # �ƶ�/mnt/lustre/dir ���Ŀ¼���ļ�
# �ҵ�/testĿ¼����test-OST0004�ϣ����Ҵ���4G���ļ����ƶ������ost��
client# lfs find /test --ost test-OST0004 -size +4G | lfs_migrate -y 
```





