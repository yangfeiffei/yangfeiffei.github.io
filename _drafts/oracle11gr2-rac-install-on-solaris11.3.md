



```bash

----------------------------------------------
1.��������
  ora01:
  # netadm enable -p ncp DefaultFixed
# ipadm create-ip net0
# ipadm create-ip net1
# ipadm create-ip net2
# ipadm create-ip net3
# ipadm create-ip net4
# ipadm create-ip net5
# ipadm set-ifprop -p standby=on -m ip net4
# ipadm set-ifprop -p standby=on -m ip net5
# ipadm create-ipmp -i net2 -i net4 ipmp0
# ipadm create-ipmp -i net3 -i net5 ipmp1
# ipadm create-addr -T static -a local=210.10.1.18/24 ipmp0
# ipadm create-addr -T static -a local=10.10.10.1/24 ipmp1
# route �Cp add default 210.10.1.250
	ora02�������ã�
# netadm enable -p ncp DefaultFixed
# ipadm create-ip net0
# ipadm create-ip net1
# ipadm create-ip net2
# ipadm create-ip net3
# ipadm create-ip net4
# ipadm create-ip net5
# ipadm set-ifprop -p standby=on -m ip net4
# ipadm set-ifprop -p standby=on -m ip net5
# ipadm create-ipmp -i net2 -i net4 ipmp0
# ipadm create-ipmp -i net3 -i net5 ipmp1
# ipadm create-addr -T static -a local=210.10.1.18/24 ipmp0
# ipadm create-addr -T static -a local=10.10.10.1/24 ipmp1
# route �Cp add default 210.10.1.250



2.�޸�host
root@ora01:~# cat /etc/hosts
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Internet host table
#
#::1 ora01 localhost 
127.0.0.1  localhost loghost 
210.10.1.18     ora01   loghost
210.10.1.19     ora02   
210.10.1.20     ora01-vip
210.10.1.21     ora02-vip
10.10.10.1      ora01-priv
10.10.10.2      ora02-priv
210.10.1.40     rac-scanip
ora02�ڵ㣺
root@ora02:~# cat /etc/hosts
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Internet host table
#
#::1 ora01 localhost 
127.0.0.1  localhost loghost 
210.10.1.18     ora01   
210.10.1.19     ora02   loghost
210.10.1.20     ora01-vip
210.10.1.21     ora02-vip
10.10.10.1      ora01-priv
10.10.10.2      ora02-priv
210.10.1.40     rac-scanip



3.

c)	����nsloopup����scanip����
�����װʱûѡ��GNS���ڰ�װǰ��Ҫ��nslookup�������ã�����������±���
INS-20802
INFO: Checking name resolution setup for "rac-scanip"...
INFO: ERROR:
INFO: PRVG-1101 : SCAN name "rac-scanip" failed to resolve
INFO: ERROR:
INFO: PRVF-4657 : Name resolution setup check for "rac-scanip" (IP address: 210.10.1.40) failed
INFO: ERROR:
INFO: PRVF-4664 : Found inconsistent name resolution entries for SCAN name "rac-scanip"
INFO: Verification of SCAN VIP and Listener setup failed
���û������DNS����Ϊ���ܹ���װ11gRAC����Ҫ�ֹ�����ʹ�ܽ���, ��11gR2�У�Ϊ�˼򻯸������ù�����������SCAN��Single Client Access Name�������ԣ������Եĺô����ڣ������ݿ���ͻ���֮�䣬�����һ������ķ���㣬������ν��scan ip�Լ�scan ip listener���ڿͻ��˽���Ҫ����scan ip��tns��Ϣ��ͨ��scan ip listener�����Ӻ�̨��Ⱥ���ݿ⡣���������ۼ�Ⱥ���ݿ��Ƿ�����ӻ���ɾ���ڵ�Ĳ������������client����Ӱ�졣�ٷ��Ƽ�����ϵͳ��scan ip��3��������Ҫ��dns����������Ŀǰ����һ��scan ipҲ������ϵͳ���е������Ҳ���Ҫ����ר�ŵ�DNS��������������ֻ����/etc/hosts�ļ��м�����ؽ�����Ŀ���ɡ�������Grid�İ�װ�������޷��ɹ�����rac-scan�ĵ�ַ��������Ҫ�ֶ�ָ���������ݡ�
��ora01�ڵ�������nslookup������
# mv /usr/sbin/nslookup /usr/sbin/nslookup.original
# vi /usr/sbin/nslookup
��nslookup�ű������һ�����ݣ�
#!/bin/bash
HOSTNAME=${1}
if [[ $HOSTNAME = "rac-scanip" ]]; then
    echo "Server:         210.10.1.20"
    echo "Address:        210.10.1.20 #53"
    echo "Non-authoritative answer:"
    echo "Name:   scanip"
    echo "Address: 210.10.1.40 "
else
   /usr/sbin/nslookup.original $HOSTNAME
fi
�޸�nslookup�ű�Ȩ�ޣ�
# chmod 755 /usr/sbin/nslookup
# nslookup rac-scanip
��ora02�ڵ���Ҳ��Ҫ����nslookup������
# mv /usr/sbin/nslookup /usr/sbin/nslookup.original
# vi /usr/sbin/nslookup
��nslookup�ű������һ�����ݣ�
#!/bin/bash
HOSTNAME=${1}
if [[ $HOSTNAME = "scanip" ]]; then
    echo "Server:         210.10.1.21"
    echo "Address:        210.10.1.21 #53"
    echo "Non-authoritative answer:"
    echo "Name:   scanip"
    echo "Address: 210.10.1.40 "
else
   /usr/sbin/nslookup.original $HOSTNAME
fi
�޸�nslookup�ű�Ȩ�ޣ�
# chmod 755 /usr/sbin/nslookup
# nslookup rac-scanip
���ͣ������210.10.1.20  ip��ora01�����vip��ַ����210.10.1.40 ���ip������/etc/hosts������ӵ�rac-scanip��ַ��������û��ʹ�õ�ip����˼������rac�е�vip��ַ��ֻ��11.2�а����vipҲ�ٰ���һ��ip����ʵҲ��һ��vip��ַ��rac-scanip��host�ļ������Ӧ����������װracʱ����ʾҪ��д����װʱ����ע�⣡����

-----------------------------
2.1.2	�����û���Ͱ�װĿ¼
��2���ڵ��ϴ�����Ⱥ�����װ�û�grid�����ݿ������װ�û�oracle��
# groupadd -g 1000 oinstall
# groupadd -g 1020 asmadmin
# groupadd -g 1021 asmdba
# groupadd -g 1022 asmoper
# groupadd -g 1031 dba
# groupadd -g 1032 oper
# useradd -u 1100 -g oinstall -G dba,asmoper,asmadmin,asmdba -d /export/home/grid -m grid
# useradd -u 1101 -g oinstall -G oper,dba,asmdba -d /export/home/oracle -m oracle
# passwd grid
# passwd oracle
��2���ڵ��ϴ�����װĿ¼��
# mkdir -p /u01/app/grid
# mkdir -p /u01/app/11.2.0/grid
# chown -R grid:oinstall /u01
# mkdir -p /u01/app/oracle
# chown oracle:oinstall /u01/app/oracle
# chmod -R 775 /u01

-------------------------------------------

����grid�û�����������
��¼����ora01��grid�û����û�������:
# su - grid
$ vi /export/home/grid/.profile
��.profile�ļ�ĩβ�����������ݣ�
ORACLE_SID=+ASM1; export ORACLE_SID
ORACLE_BASE=/u01/app/grid; export ORACLE_BASE
ORACLE_HOME=/u01/app/11.2.0/grid; export ORACLE_HOME
JAVA_HOME=$ORACLE_HOME/jdk; export JAVA_HOME
ORACLE_PATH=/u01/app/oracle/common/oracle/sql; export ORACLE_PATH
ORACLE_TERM=xterm; export ORACLE_TERM
NLS_DATE_FORMAT="DD-MON-YYYY HH24:MI:SS"; export NLS_DATE_FORMAT
TNS_ADMIN=$ORACLE_HOME/network/admin; export TNS_ADMIN
ORA_NLS11=$ORACLE_HOME/nls/data; export ORA_NLS11
PATH=.:${JAVA_HOME}/bin:${PATH}:$HOME/bin:$ORACLE_HOME/bin
PATH=${PATH}:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin
PATH=${PATH}:/u01/app/common/oracle/bin
export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ORACLE_HOME/oracm/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/lib:/usr/lib:/usr/local/lib
export LD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/JRE
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/rdbms/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/network/jlib
export CLASSPATH
THREADS_FLAG=native; export THREADS_FLAG
export TEMP=/tmp
export TMPDIR=/tmp
umask 022
ulimit -t unlimited
ulimit -f unlimited
ulimit -d unlimited
ulimit -s unlimited
ulimit -v unlimited
if [ -t 0 ]; then
stty intr ^C
fi
��¼����ora02��grid�û������û�������:
# su - grid
$ vi /export/home/grid/.profile
��.profile�ļ�ĩβ�����������ݣ�
ORACLE_SID=+ASM2; export ORACLE_SID
ORACLE_BASE=/u01/app/grid; export ORACLE_BASE
ORACLE_HOME=/u01/app/11.2.0/grid; export ORACLE_HOME
JAVA_HOME=$ORACLE_HOME/jdk; export JAVA_HOME
ORACLE_PATH=/u01/app/oracle/common/oracle/sql; export ORACLE_PATH
ORACLE_TERM=xterm; export ORACLE_TERM
NLS_DATE_FORMAT="DD-MON-YYYY HH24:MI:SS"; export NLS_DATE_FORMAT
TNS_ADMIN=$ORACLE_HOME/network/admin; export TNS_ADMIN
ORA_NLS11=$ORACLE_HOME/nls/data; export ORA_NLS11
PATH=.:${JAVA_HOME}/bin:${PATH}:$HOME/bin:$ORACLE_HOME/bin
PATH=${PATH}:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin
PATH=${PATH}:/u01/app/common/oracle/bin
export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ORACLE_HOME/oracm/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/lib:/usr/lib:/usr/local/lib
export LD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/JRE
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/rdbms/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/network/jlib
export CLASSPATH
THREADS_FLAG=native; export THREADS_FLAG
export TEMP=/tmp
export TMPDIR=/tmp
umask 022
ulimit -t unlimited
ulimit -f unlimited
ulimit -d unlimited
ulimit -s unlimited
ulimit -v unlimited
if [ -t 0 ]; then
stty intr ^C
fi
��¼����ora01��oracle�û����û�������:
# su - oracle
$ vi /export/home/oracle/.profile
��.profile�ļ�ĩβ�����������ݣ�
ORACLE_SID=hisd1; export ORACLE_SID
ORACLE_UNQNAME=hisd; export ORACLE_UNQNAME
ORACLE_BASE=/u01/app/oracle; export ORACLE_BASE
ORACLE_HOME=$ORACLE_BASE/product/11.2.0/dbhome_1; export ORACLE_HOME
JAVA_HOME=$ORACLE_HOME/jdk; export JAVA_HOME
ORACLE_PATH=/u01/app/common/oracle/sql; export ORACLE_PATH
ORACLE_TERM=xterm; export ORACLE_TERM
NLS_DATE_FORMAT="DD-MON-YYYY HH24:MI:SS"; export NLS_DATE_FORMAT
TNS_ADMIN=$ORACLE_HOME/network/admin; export TNS_ADMIN
ORA_NLS11=$ORACLE_HOME/nls/data; export ORA_NLS11
PATH=.:${JAVA_HOME}/bin:${PATH}:$HOME/bin:$ORACLE_HOME/bin
PATH=${PATH}:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin
PATH=${PATH}:/u01/app/common/oracle/bin
export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ORACLE_HOME/oracm/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/lib:/usr/lib:/usr/local/lib
export LD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/JRE
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/rdbms/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/network/jlib
export CLASSPATH
THREADS_FLAG=native; export THREADS_FLAG
export TEMP=/tmp
export TMPDIR=/tmp
umask 022
stty erase ^H
��¼����ora02��oracle�û����û�������:
# su - oracle
$ vi /export/home/oracle/.profile
��.profile�ļ�ĩβ�����������ݣ�
ORACLE_SID=hisd2; export ORACLE_SID
ORACLE_UNQNAME=hisd; export ORACLE_UNQNAME
ORACLE_BASE=/u01/app/oracle; export ORACLE_BASE
ORACLE_HOME=$ORACLE_BASE/product/11.2.0/dbhome_1; export ORACLE_HOME
JAVA_HOME=$ORACLE_HOME/jdk; export JAVA_HOME
ORACLE_PATH=/u01/app/common/oracle/sql; export ORACLE_PATH
ORACLE_TERM=xterm; export ORACLE_TERM
NLS_DATE_FORMAT="DD-MON-YYYY HH24:MI:SS"; export NLS_DATE_FORMAT
TNS_ADMIN=$ORACLE_HOME/network/admin; export TNS_ADMIN
ORA_NLS11=$ORACLE_HOME/nls/data; export ORA_NLS11
PATH=.:${JAVA_HOME}/bin:${PATH}:$HOME/bin:$ORACLE_HOME/bin
PATH=${PATH}:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin
PATH=${PATH}:/u01/app/common/oracle/bin
export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ORACLE_HOME/oracm/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/lib:/usr/lib:/usr/local/lib
export LD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/JRE
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/rdbms/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/network/jlib
export CLASSPATH
THREADS_FLAG=native; export THREADS_FLAG
export TEMP=/tmp
export TMPDIR=/tmp
umask 022
stty erase ^H

-------------------------------------

2.1.4	����Solaris�ں˲���

��ora01��ora02�ڵ�������ͬ����Solaris�ں˲�����
a)	Ϊgrid�û���oracle�û�����projects�����ù����ڴ������
# /usr/sbin/projadd -U grid -K "project.max-shm-memory=(priv,28g,deny)" user.grid
# /usr/sbin/projmod -sK "project.max-sem-nsems=(priv,512,deny)" user.grid
# /usr/sbin/projmod -sK "project.max-sem-ids=(priv,128,deny)" user.grid
# /usr/sbin/projmod -sK "project.max-shm-ids=(priv,128,deny)" user.grid
# /usr/sbin/projmod -sK "project.max-shm-memory=(priv,6g,deny)" user.grid
# /usr/sbin/projadd -U oracle -K "project.max-shm-memory=(priv,28g,deny)" user.oracle
# /usr/sbin/projmod -sK "project.max-sem-nsems=(priv,512,deny)" user.oracle
# /usr/sbin/projmod -sK "project.max-sem-ids=(priv,128,deny)" user.oracle
# /usr/sbin/projmod -sK "project.max-shm-ids=(priv,128,deny)" user.oracle
# /usr/sbin/projmod -sK "project.max-shm-memory=(priv,28g,deny)" user.oracle
# /usr/sbin/projmod -sK "process.max-file-descriptor=(priv,65536,deny)" user.oracle
# /usr/sbin/projmod -sK "process.max-file-descriptor=(priv,65536,deny)" user.grid
# /usr/sbin/prctl -n project.max-shm-memory -r -v 28G -i project system
# /usr/sbin/projmod -s -K "project.max-shm-memory=(priv,28G,deny)" system
# /usr/sbin/projmod -s -K "project.max-shm-memory=(priv,28G,deny)" default
�༭/etc/user_attr�ļ�����������ݣ��и�root���û�����project=system��
root::::type=normal;auths=solaris.*,solaris.grant;profiles=Web Console Management,All;lock_after_retries=no;clearance=admin_high;min_label=admin_low;project=system
b)	����TCP��UDP�ں˲���
	ͨ��ʹ��ipadm�����������£�
# ipadm set-prop -p smallest_anon_port=9000 tcp
# ipadm set-prop -p largest_anon_port=65500 tcp
# ipadm set-prop -p smallest_anon_port=9000 udp
# ipadm set-prop -p largest_anon_port=65500 udp
c)	����2���ڵ��swap�ռ��С
# zfs set volsize=20G rpool/swap
# zfs get volsize rpool/swap
NAME        PROPERTY  VALUE  SOURCE
rpool/swap  volsize   20G    local
d)	�޸�/etc/system�ļ�����ӽ�������Ӳ����
set rlim_fd_cur = 65536
set rlim_fd_max = 65536

----------------------

2.1.5	�ر�ntp�����sendmail����
Oracle 11g R2 ���������ʱ��ͬ������Cluster Time Synchronization Service(CTSS)��Ҫ��ʹ�øù��ܣ��ͱ����ȹر�ntp����
��2���ڵ��Ϲر�NTP�����sendmail����
# svcadm disable ntp
# svcadm disable sendmail



------------------------------

2.1.6	ϵͳ�������鰲װ
��2���ڵ������ñ���ӳ��⣬����oracle��װ�������谲װ����װ���ϴ�ӳ����ļ����ڵ㣬ӳ����ļ�������oracle�������أ�ӳ����ļ�Ϊ��
sol-11_1-repo-full.iso-a
sol-11_1-repo-full.iso-b
����ӳ����Ŀ¼/inspur����ӳ����ϴ���/inspurĿ¼����ɺ�2���ļ��ϲ���
# mkdir /inspur
# cat sol-11_1-repo-full.iso-a sol-11_1-repo-full.iso-b > sol-11_1-repo-full.iso
����ӳ�����Ŀ¼��
# mkdir /iso
����ӳ����ļ�
# mount �CF hsfs /inspur/ sol-11_1-repo-full.iso /iso
��������ӳ��⣺
#pkg set-publisher -G '*' -g  file:///iso/repo solaris
	�����װ����ϵͳʱδ��װͼ�ν��棬��ͨ���������װͼ�ν��棺
# pkg install  solaris-desktop
	��װ��Ⱥ���谲װ����
# pkg install compatibility/packages/SUNWxwplt SUNWmfrun SUNWarc SUNWhea SUNWlibm

----------------------

2.1.7	����ssh����
	Oracle 11G RAC��װ��Ҫgrid�û���oracle�û���Ӧ��2���ڵ㶼Ҫ��ssh���ι�ϵ��
1.	����oracle�û������ι�ϵ�������£�
	)	��2���ڵ㶼Ҫִ���������������Կ��˽Կ
# su �C oracle
$ ssh-keygen -t rsa
$ ssh-keygen -t dsa
$ cd /export/home/oracle/.ssh
$ cat *.pub >> authorized_keys
b)	��ora01�ϵ�authorized_keys������ora02�ڵ���ӦĿ¼������
$ scp /export/home/oracle/.ssh/authorized_keys ora02: /export/home/oracle/.ssh/keys
c)	�ڽڵ�2�Ͻ�keys���ݺϲ���authorized_keys���������ļ������ڵ�1�ϣ�
$ cat keys >> authorized_keys
$ scp authorized_keys ora01: /export/home/oracle/.ssh/
d)	�����нڵ��ϲ�����֤���ι�ϵ
$ ssh ora01 date
$ ssh ora02 date
$ ssh ora01-priv date
$ ssh ora02-priv date
2.	����grid�û������ι�ϵ����������oracle�û����ι�ϵһ�£�
	)	��2���ڵ㶼Ҫִ���������������Կ��˽Կ
# su �C grid
$ ssh-keygen -t rsa
$ ssh-keygen -t dsa
$ cd /export/home/grid/.ssh
$ cat *.pub >> authorized_keys
b)	��ora01�ϵ�authorized_keys������ora02�ڵ���ӦĿ¼������
$ scp /export/home/grid/.ssh/authorized_keys ora02: /export/home/grid/.ssh/keys
c)	�ڽڵ�2�Ͻ�keys���ݺϲ���authorized_keys���������ļ������ڵ�1�ϣ�
$ cat keys >> authorized_keys
$ scp authorized_keys ora01: /export/home/grid/.ssh/
d)	�����нڵ��ϲ�����֤���ι�ϵ
$ ssh ora01 date
$ ssh ora02 date
$ ssh ora01-priv date
$ ssh ora02-priv date

------------------------------------

2.1.8	���ù�����̷���
�洢ΪInspur AS500Hӳ����TS850������Ϊ��·�������ڻ���x86��Solaris 11����ϵͳ��Ĭ���������˶�·�����ܡ�
1.	���ò���ϵͳ��·������
ȷ�϶�·���Ƿ����ã�
# prtconf -vc /devices/scsi_vhci |grep dev_link.*s2
����ض�·����Ϣȷ���Ѿ������˶�·�����ܣ���δ������ʹ���������ü��ɡ�
ȷ�϶�·����װ���Ƿ�װ��
# pkg info system/storage/multipath-utilities
          Name: system/storage/multipath-utilities
       Summary: Solaris Multipathing CLI
   Description: Path Administration Utility for a Solaris Multipathing device
      Category: Applications/System Utilities
         State: Installed
     Publisher: solaris
       Version: 0.5.11
 Build Release: 5.11
        Branch: 0.175.0.0.0.0.0
Packaging Date: Tue Sep 27 01:40:01 2011
          Size: 77.29 kB
          FMRI: pkg://solaris/system/storage/multipath-utilities@
0.5.11,5.11-0.175.0.0.0.0.0:20110927T014001Z
��װ��·�����������Ҫ����Solaris����ӳ��⣬�Ժ��½ڻὲ�����ñ���ӳ��⣺
# pkg install system/storage/multipath-utilities

������·���豸��
# stmsboot -e
���ö�·���豸��
# stmsboot -d


2.	���̷���
���̷���ֻ��Ҫ��һ���ڵ������ɣ��������һ�����̷�������˵�����������̷����������÷���һ�£�ʹ��format������з�����
root@ora01:~# format
Searching for disks...done
AVAILABLE DISK SELECTIONS:
       0. c0t60080E50003EA0E60000050B52CBA01Ad0 <LSI-INF-01-00-0784 cyl 44555 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea0e60000050b52cba01a
       1. c0t60080E50003EA0E60000050D52CBA03Cd0 <LSI-INF-01-00-0784 cyl 44555 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea0e60000050d52cba03c
	2. c0t60080E50003EA0E60000050F52CBA107d0 <LSI-INF-01-00-0784 cyl 44555 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea0e60000050f52cba107
       3. c0t60080E50003EA0E60000050752CB9FBCd0 <LSI-INF-01-00-0784 cyl 1021 alt 2 hd 128 sec 32>
          /scsi_vhci/disk@g60080e50003ea0e60000050752cb9fbc
       4. c0t60080E50003EA0E60000050952CB9FF9d0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea0e60000050952cb9ff9
       5. c0t60080E50003EA0E60000053152D59D24d0 <LSI-INF-01-00-0784 cyl 1022 alt 2 hd 128 sec 32>
          /scsi_vhci/disk@g60080e50003ea0e60000053152d59d24
       6. c0t60080E50003EA0E60000054752D6259Ad0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea0e60000054752d6259a
       7. c0t60080E50003EA3740000051A52CB9FF5d0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea3740000051a52cb9ff5
       8. c0t60080E50003EA3740000051C52CBA015d0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea3740000051c52cba015
       9. c0t60080E50003EA3740000051652CB9F99d0 <LSI-INF-01-00-0784 cyl 1022 alt 2 hd 128 sec 32>
          /scsi_vhci/disk@g60080e50003ea3740000051652cb9f99
      10. c0t60080E50003EA3740000051852CB9FB2d0 <LSI-INF-01-00-0784 cyl 1022 alt 2 hd 128 sec 32>
          /scsi_vhci/disk@g60080e50003ea3740000051852cb9fb2
      11. c0t60080E50003EA3740000055D52D62553d0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea3740000055d52d62553
      12. c0t60080E50003EA3740000054452D59D06d0 <LSI-INF-01-00-0784 cyl 1022 alt 2 hd 128 sec 32>
          /scsi_vhci/disk@g60080e50003ea3740000054452d59d06
      13. c0t60080E50003EA3740000051E52CBA0E3d0 <LSI-INF-01-00-0784 cyl 44556 alt 2 hd 255 sec 189>
          /scsi_vhci/disk@g60080e50003ea3740000051e52cba0e3
      14. c0t60080E50003EA3740000055F52D62597d0 <LSI-INF-01-00-0784 cyl 65268 alt 2 hd 255 sec 63>
          /scsi_vhci/disk@g60080e50003ea3740000055f52d62597
      15. c7t0d0 <LSI-MR9271-8i-3.19-278.88GB>
          /pci@0,0/pci8086,3410@9/pci1000,9271@0/sd@0,0
Specify disk (enter its number): 1     
selecting c0t60080E50003EA0E60000050D52CBA03Cd0
[disk formatted]

FORMAT MENU:
        disk       - select a disk
        type       - select (define) a disk type
        partition  - select (define) a partition table
        current    - describe the current disk
        format     - format and analyze the disk
        fdisk      - run the fdisk program
        repair     - repair a defective sector
        label      - write label to the disk
        analyze    - surface analysis
        defect     - defect list management
        backup     - search for backup labels
        verify     - read and display labels
        save       - save new disk/partition definitions
        inquiry    - show disk ID
        volname    - set 8-character volume name
        !<cmd>     - execute <cmd>, then return
        quit
format> p
PARTITION MENU:
        0      - change `0' partition
        1      - change `1' partition
        2      - change `2' partition
        3      - change `3' partition
        4      - change `4' partition
        5      - change `5' partition
        6      - change `6' partition
        7      - change `7' partition
        select - select a predefined table
        modify - modify a predefined partition table
        name   - name the current table
        print  - display the current table
        label  - write partition map and label to the disk
        !<cmd> - execute <cmd>, then return
        quit
partition> 6
Part      Tag    Flag     Cylinders         Size            Blocks
  6 unassigned    wm       0                0         (0/0/0)              0

Enter partition id tag[unassigned]: usr
Enter partition permission flags[wm]: 
Enter new starting cyl[1]: 3
Enter partition size[0b, 0c, 3e, 0.00mb, 0.00gb]: $

partition> p
Current partition table (unnamed):
Total disk cylinders available: 44555 + 2 (reserved cylinders)

Part      Tag    Flag     Cylinders         Size            Blocks
  0 unassigned    wm       0                0         (0/0/0)              0
  1 unassigned    wm       0                0         (0/0/0)              0
  2     backup    wu       0 - 44554     1023.93GB    (44555/0/0) 2147328225
  3 unassigned    wm       0                0         (0/0/0)              0
  4 unassigned    wm       0                0         (0/0/0)              0
  5 unassigned    wm       0                0         (0/0/0)              0
  6        usr    wm       3 - 44554     1023.86GB    (44552/0/0) 2147183640
  7 unassigned    wm       0                0         (0/0/0)              0
  8       boot    wu       0 -     0       23.53MB    (1/0/0)          48195
  9 unassigned    wm       0                0         (0/0/0)              0

partition> label
Ready to label disk, continue? yes
partition> q
���̷���������£�
/dev/rdsk/c0t60080E50003EA0E60000050B52CBA01Ad0s6 
/dev/rdsk/c0t60080E50003EA0E60000050D52CBA03Cd0s6 /dev/rdsk/c0t60080E50003EA0E60000050F52CBA107d0s6 /dev/rdsk/c0t60080E50003EA0E60000050752CB9FBCd0s6 
/dev/rdsk/c0t60080E50003EA0E60000050952CB9FF9d0s6 
/dev/rdsk/c0t60080E50003EA0E60000053152D59D24d0s6 
/dev/rdsk/c0t60080E50003EA0E60000054752D6259Ad0s6 
/dev/rdsk/c0t60080E50003EA3740000051A52CB9FF5d0s6 
/dev/rdsk/c0t60080E50003EA3740000051C52CBA015d0s6 
/dev/rdsk/c0t60080E50003EA3740000051652CB9F99d0s6 
/dev/rdsk/c0t60080E50003EA3740000051852CB9FB2d0s6 
/dev/rdsk/c0t60080E50003EA3740000055D52D62553d0s6 
/dev/rdsk/c0t60080E50003EA3740000054452D59D06d0s6 
/dev/rdsk/c0t60080E50003EA3740000051E52CBA0E3d0s6

3.	����������̷���������
���������ӵ�Ŀ���ǰѱ��ش�������ӳ��Ϊ��ͬ�����ƣ����⣬ӳ��֮����豸���ƱȽ�ֱ�ۣ����������������������Ŀ¼�����������ӣ�
# mkdir /sharedisk
# chmod 775 /sharedisk
# ln -s /dev/rdsk/c0t60080E50003EA0E60000050B52CBA01Ad0s6 /sharedisk/asm1
# ln -s /dev/rdsk/c0t60080E50003EA0E60000050D52CBA03Cd0s6 /sharedisk/asm2
# ln -s /dev/rdsk/c0t60080E50003EA0E60000050F52CBA107d0s6 /sharedisk/asm3
# ln -s /dev/rdsk/c0t60080E50003EA0E60000050752CB9FBCd0s6 /sharedisk/crs1
# ln -s /dev/rdsk/c0t60080E50003EA0E60000050952CB9FF9d0s6 /sharedisk/asm4
# ln -s /dev/rdsk/c0t60080E50003EA0E60000053152D59D24d0s6 /sharedisk/crs2
# ln -s /dev/rdsk/c0t60080E50003EA0E60000054752D6259Ad0s6 /sharedisk/asm5
# ln -s /dev/rdsk/c0t60080E50003EA3740000051A52CB9FF5d0s6 /sharedisk/asm6
# ln -s /dev/rdsk/c0t60080E50003EA3740000051C52CBA015d0s6 /sharedisk/asm7
# ln -s /dev/rdsk/c0t60080E50003EA3740000051652CB9F99d0s6 /sharedisk/crs3
# ln -s /dev/rdsk/c0t60080E50003EA3740000051852CB9FB2d0s6 /sharedisk/crs4
# ln -s /dev/rdsk/c0t60080E50003EA3740000055D52D62553d0s6 /sharedisk/fra1
# ln -s /dev/rdsk/c0t60080E50003EA3740000054452D59D06d0s6 /sharedisk/crs5
# ln -s /dev/rdsk/c0t60080E50003EA3740000051E52CBA0E3d0s6 /sharedisk/fra2
4.	�޸Ĵ����û��顢�������Լ�Ȩ��
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000050B52CBA01Ad0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000050D52CBA03Cd0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000050F52CBA107d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000050752CB9FBCd0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000050952CB9FF9d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000053152D59D24d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA0E60000054752D6259Ad0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000051A52CB9FF5d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000051C52CBA015d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000051652CB9F99d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000051852CB9FB2d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000055D52D62553d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000054452D59D06d0s6 
# chmod -R 660 /dev/rdsk/c0t60080E50003EA3740000051E52CBA0E3d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000050B52CBA01Ad0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000050D52CBA03Cd0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000050F52CBA107d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000050752CB9FBCd0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000050952CB9FF9d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000053152D59D24d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA0E60000054752D6259Ad0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000051A52CB9FF5d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000051C52CBA015d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000051652CB9F99d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000051852CB9FB2d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000055D52D62553d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000054452D59D06d0s6
# chown -R grid:asmadmin /dev/rdsk/c0t60080E50003EA3740000051E52CBA0E3d0s6	

# chmod -R 660 /sharedisk/*
# chown -R grid:asmadmin /sharedisk/*

-----------------------------

2.1.9	��Ⱥ�����װǰ���
����2���ڵ����ϵͳ����м�Ⱥ�����װǰ��鹤�����ϴ���װ����/inspurĿ¼����ѹ��װ����
# cd /inspur
# unzip p10404530_112030_Solaris86-64_1of6.zip
# unzip p10404530_112030_Solaris86-64_1of6.zip
# unzip p10404530_112030_Solaris86-64_1of6.zip
�����ѹ������grid��װ����ִ�������鰲װ�������������
# su �C grid
$ cd /inspur/grid
$ ./runcluvfy.sh stage -pre crsinst -n ora01,ora02 -fixup �Cverbose


������������

-------------------------

2.2	��װ��Ⱥ���
	��Ⱥ�����װֻ��Ҫ��һ���ڵ�ʹ��grid�û���װ���ɣ���¼grid�û����뼯Ⱥ��װ��Ŀ¼ִ�У�
$ cd /inspur/grid
$ ./runInstaller



�ڰ�װ����󣬻ᵯ��ִ�нű��Ի���Ҫִ�����ýű���һ��Ҫ����ʾ��˳�����
��root�û�ִ�нű��� 
a)	��RAC�ڵ�ora01��ִ��orainstRoot.sh�� 
# /u01/app/oraInventory/orainstRoot.sh 

Changing permissions of /u01/app/oraInventory.
Adding read,write permissions for group.

Removing read,write,execute permissions for world.
Changing groupname of /u01/app/oraInventory to oinstall.
The execution of the script is complete.
b)	��RAC�ڵ�ora02��ִ��orainstRoot.sh�� 
# /u01/app/oraInventory/orainstRoot.sh 



c)	��RAC�ڵ�ora01��ִ��root.sh�� 
# /u01/app/11.2.0/grid/root.sh 

Performing root user operation for Oracle 11g 

The following environment variables are set as:
    ORACLE_OWNER= grid
    ORACLE_HOME=  /u01/app/11.2.0/grid

Enter the full pathname of the local bin directory: [/usr/local/bin]: 
Creating /usr/local/bin directory...
   Copying dbhome to /usr/local/bin ...
   Copying oraenv to /usr/local/bin ...
   Copying coraenv to /usr/local/bin ...


Creating /var/opt/oracle/oratab file...
Entries will be added to the /var/opt/oracle/oratab file as needed by
Database Configuration Assistant when a database is created
Finished running generic part of root script.
Now product-specific root actions will be performed.
Using configuration parameter file: /u01/app/11.2.0/grid/crs/install/crsconfig_params
Creating trace directory
User ignored Prerequisites during installation
OLR initialization - successful
  root wallet
  root wallet cert
  root cert export
  peer wallet
  profile reader wallet
  pa wallet
  peer wallet keys
  pa wallet keys
  peer cert request
  pa cert request
  peer cert
  pa cert
  peer root cert TP
  profile reader root cert TP
  pa root cert TP
  peer pa cert TP
  pa peer cert TP
  profile reader pa cert TP
  profile reader peer cert TP
  peer user cert
  pa user cert
Adding Clusterware entries to inittab
CRS-2672: Attempting to start 'ora.mdnsd' on 'ora01'
CRS-2676: Start of 'ora.mdnsd' on 'ora01' succeeded
CRS-2672: Attempting to start 'ora.gpnpd' on 'ora01'
CRS-2676: Start of 'ora.gpnpd' on 'ora01' succeeded
CRS-2672: Attempting to start 'ora.cssdmonitor' on 'ora01'
CRS-2672: Attempting to start 'ora.gipcd' on 'ora01'
CRS-2676: Start of 'ora.cssdmonitor' on 'ora01' succeeded
CRS-2676: Start of 'ora.gipcd' on 'ora01' succeeded
CRS-2672: Attempting to start 'ora.cssd' on 'ora01'
CRS-2672: Attempting to start 'ora.diskmon' on 'ora01'
CRS-2676: Start of 'ora.diskmon' on 'ora01' succeeded
CRS-2676: Start of 'ora.cssd' on 'ora01' succeeded

ASM created and started successfully.

Disk Group CRS created successfully.

clscfg: -install mode specified
Successfully accumulated necessary OCR keys.
Creating OCR keys for user 'root', privgrp 'root'..
Operation successful.
CRS-4256: Updating the profile
Successful addition of voting disk 0e12cbd251d34f75bfd7dfc27b639d42.
Successful addition of voting disk 0da64751d2944f92bff9796e99d56bab.
Successful addition of voting disk 85e7ab70f9794f6ebf733ae56a99d475.
Successfully replaced voting disk group with +CRS.
CRS-4256: Updating the profile
CRS-4266: Voting file(s) successfully replaced
##  STATE    File Universal Id                File Name Disk group
--  -----    -----------------                --------- ---------
 1. ONLINE   0e12cbd251d34f75bfd7dfc27b639d42 (/sharedisk/crs1) [CRS]
 2. ONLINE   0da64751d2944f92bff9796e99d56bab (/sharedisk/crs2) [CRS]
 3. ONLINE   85e7ab70f9794f6ebf733ae56a99d475 (/sharedisk/crs3) [CRS]
Located 3 voting disk(s).
CRS-2672: Attempting to start 'ora.asm' on 'ora01'
CRS-2676: Start of 'ora.asm' on 'ora01' succeeded
CRS-2672: Attempting to start 'ora.CRS.dg' on 'ora01'
CRS-2676: Start of 'ora.CRS.dg' on 'ora01' succeeded
Configure Oracle Grid Infrastructure for a Cluster ... succeeded


d)	RAC�ڵ�ora02�� 
# /u01/app/11.2.0/grid/root.sh 

����������



----------------------


2.2.16	��Ⱥ���У��
ͨ�������鼯Ⱥ״̬��Ϣ��

grid@ora01:~$ crsctl check cluster -all



grid@ora01:~$ crs_stat -t -v

-----------------------
2.2.17	����ASM������
����2��ASM�����飬DATA���������ڴ�������ļ���FRA����������ϵͳ���������������鴴����Ҫʹ��grid�û������岽�����£�
	��¼��grid�û���ִ��asmca���


------------------------------
2.3	��װ���ݿ����

��¼oracle�û������뵽database����Ŀ¼���������ݿ������ֻ��װ���ݿ�����������н��⡣
$ cd /inspur/database
$ ./runInstaller



..............

������ʾ��Ϣ��ʹ��root�û��ֱ���2���ڵ���ִ�нű���
# /u01/app/oracle/product/11.2.0/dbhome_1/root.sh


������ݿ������װ



----------------------------

2.4	�������ݿ�
	��¼��oracle�û���ʹ��dbca��������ݿ⣺


ע��㣺���ַ�������ΪZHS16GBK

����Redo Log Groups�����������޸�redo log��ʹ�С

----------------------------------------
��Ⱥ���ݿ���֤ά��
3.1	��Ⱥ���ݿ���֤ά��
A.	��鼯Ⱥ������Դ״̬
grid@ora01:~$ crs_stat -t -v
Name           Type           R/RA   F/FT   Target    State     Host        
----------------------------------------------------------------------
ora.CRS.dg     ora....up.type 0/5    0/     ONLINE    ONLINE    ora01       
ora.DATA.dg    ora....up.type 0/5    0/     ONLINE    ONLINE    ora01       
ora.FRA.dg     ora....up.type 0/5    0/     ONLINE    ONLINE    ora01       
ora....ER.lsnr ora....er.type 0/5    0/     ONLINE    ONLINE    ora01       
ora....N1.lsnr ora....er.type 0/5    0/0    ONLINE    ONLINE    ora01       
ora.asm        ora.asm.type   0/5    0/     ONLINE    ONLINE    ora01       
ora.cvu        ora.cvu.type   0/5    0/0    ONLINE    ONLINE    ora01       
ora.gsd        ora.gsd.type   0/5    0/     OFFLINE   OFFLINE               
ora.hisd.db    ora....se.type 0/2    0/1    ONLINE    ONLINE    ora01       
ora....network ora....rk.type 0/5    0/     ONLINE    ONLINE    ora01       
ora.oc4j       ora.oc4j.type  0/1    0/2    OFFLINE   OFFLINE               
ora.ons        ora.ons.type   0/3    0/     ONLINE    ONLINE    ora01       
ora....SM1.asm application    0/5    0/0    ONLINE    ONLINE    ora01       
ora....01.lsnr application    0/5    0/0    ONLINE    ONLINE    ora01       
ora.ora01.gsd  application    0/5    0/0    OFFLINE   OFFLINE               
ora.ora01.ons  application    0/3    0/0    ONLINE    ONLINE    ora01       
ora.ora01.vip  ora....t1.type 0/0    0/0    ONLINE    ONLINE    ora01       
ora....SM2.asm application    0/5    0/0    ONLINE    ONLINE    ora02       
ora....02.lsnr application    0/5    0/0    ONLINE    ONLINE    ora02       
ora.ora02.gsd  application    0/5    0/0    OFFLINE   OFFLINE               
ora.ora02.ons  application    0/3    0/0    ONLINE    ONLINE    ora02       
ora.ora02.vip  ora....t1.type 0/0    0/0    ONLINE    ONLINE    ora02       
ora.scan1.vip  ora....ip.type 0/0    0/0    ONLINE    ONLINE    ora01            
B.	��鼯Ⱥ״̬
grid@ora01:~$ crsctl check cluster -all
**************************************************************
ora01:
CRS-4537: Cluster Ready Services is online
CRS-4529: Cluster Synchronization Services is online
CRS-4533: Event Manager is online
**************************************************************
ora02:
CRS-4537: Cluster Ready Services is online
CRS-4529: Cluster Synchronization Services is online
CRS-4533: Event Manager is online
**************************************************************
C.	�������ʵ��״̬
grid@ora01:~$ srvctl status database -d hisd
Instance hisd1 is running on node ora01
Instance hisd2 is running on node ora02
D.	���ڵ�Ӧ�ó���״̬
grid@ora01:~$ srvctl status nodeapps
VIP ora01-vip is enabled
VIP ora01-vip is running on node: ora01
VIP ora02-vip is enabled
VIP ora02-vip is running on node: ora02
Network is enabled
Network is running on node: ora01
Network is running on node: ora02
GSD is disabled
GSD is not running on node: ora01
GSD is not running on node: ora02
ONS is enabled
ONS daemon is running on node: ora01
ONS daemon is running on node: ora02
E.	�г��������ݿ�����
grid@ora01:~$ srvctl config database
hisd

grid@ora01:~$ srvctl config database -d hisd -a
Database unique name: hisd
Database name: hisd
Oracle home: /u01/app/oracle/product/11.2.0/dbhome_1
Oracle user: oracle
Spfile: +DATA/hisd/spfilehisd.ora
Domain: 
Start options: open
Stop options: immediate
Database role: PRIMARY
Management policy: AUTOMATIC
Server pools: hisd
Database instances: hisd1,hisd2
Disk Groups: DATA,FRA
Mount point paths: 
Services: 
Type: RAC
Database is enabled
Database is administrator managed
F.	��ѯASM״̬������
grid@ora01:~$ srvctl status asm
ASM is running on ora01,ora02

grid@ora01:~$ srvctl config asm -a
ASM home: /u01/app/11.2.0/grid
ASM listener: LISTENER
ASM is enabled.
G.	TNS������״̬�Լ�����
grid@ora01:~$ srvctl status listener
Listener LISTENER is enabled
Listener LISTENER is running on node(s): ora01,ora02

grid@ora01:~$ srvctl config listener -a
Name: LISTENER
Network: 1, Owner: grid
Home: <CRS home>
  /u01/app/11.2.0/grid on node(s) ora02,ora01
End points: TCP:1521
H.	SCAN״̬�Լ�����
grid@ora01:~$ srvctl status scan
SCAN VIP scan1 is enabled
SCAN VIP scan1 is running on node ora01

grid@ora01:~$ srvctl config scan
SCAN name: rac-scanip, Network: 1/210.10.1.0/255.255.255.0/ipmp0
SCAN VIP name: scan1, IP: /rac-scanip/210.10.1.40
I.	VIP�����ڵ��״̬�Լ�����
grid@ora01:~$ srvctl status vip -n ora01
VIP ora01-vip is enabled
VIP ora01-vip is running on node: ora01

grid@ora01:~$ srvctl status vip -n ora02
VIP ora02-vip is enabled
VIP ora02-vip is running on node: ora02

grid@ora01:~$ srvctl config vip -n ora01
VIP exists: /ora01-vip/210.10.1.20/210.10.1.0/255.255.255.0/ipmp0, hosting node ora01

grid@ora01:~$ srvctl config vip -n ora02
VIP exists: /ora02-vip/210.10.1.21/210.10.1.0/255.255.255.0/ipmp0, hosting node ora02
J.	�ڵ�Ӧ�ó������� ����VIP��GSD��ONS����������
grid@ora01:~$ srvctl config nodeapps -a -g -s -l
Warning:-l option has been deprecated and will be ignored.
Network exists: 1/210.10.1.0/255.255.255.0/ipmp0, type static
VIP exists: /ora01-vip/210.10.1.20/210.10.1.0/255.255.255.0/ipmp0, hosting node ora01
VIP exists: /ora02-vip/210.10.1.21/210.10.1.0/255.255.255.0/ipmp0, hosting node ora02
GSD exists
ONS exists: Local port 6100, remote port 6200, EM port 2016
Name: LISTENER
Network: 1, Owner: grid
Home: <CRS home>
  /u01/app/11.2.0/grid on node(s) ora02,ora01
End points: TCP:1521
K.	��֤���м�Ⱥ�ڵ���ʱ��ͬ��
grid@ora01:~$ cluvfy comp clocksync -verbose
Verifying Clock Synchronization across the cluster nodes 

Checking if Clusterware is installed on all nodes...
Check of Clusterware install passed

Checking if CTSS Resource is running on all nodes...
Check: CTSS Resource running on all nodes
  Node Name                             Status                  
------------------------------------  ------------------------
  ora01                                 passed                  
Result: CTSS resource check passed


Querying CTSS for time offset on all nodes...
Result: Query of CTSS for time offset passed

Check CTSS state started...
Check: CTSS state
  Node Name                             State                   
------------------------------------  ------------------------
  ora01                                 Active                  
CTSS is in Active state. Proceeding with check of clock time offsets on all nodes...
Reference Time Offset Limit: 1000.0 msecs
Check: Reference Time Offset
  Node Name     Time Offset               Status                  
------------  ------------------------  ------------------------
  ora01         0.0                       passed                  

Time offset is within the specified limits on the following set of nodes: 
"[ora01]" 
Result: Check of clock time offsets passed


Oracle Cluster Time Synchronization Services check passed

Verification of Clock Synchronization across the cluster nodes was successful.
L.	���ݿ�״̬��֤
oracle@ora01:~$ sqlplus / as sysdba

SQL*Plus: Release 11.2.0.3.0 Production on Mon Jan 20 10:21:58 2014

Copyright (c) 1982, 2011, Oracle.  All rights reserved.


Connected to:
Oracle Database 11g Enterprise Edition Release 11.2.0.3.0 - 64bit Production
With the Partitioning, Real Application Clusters, Automatic Storage Management, OLAP,
Data Mining and Real Application Testing options

SQL> SELECT inst_id , instance_number inst_no , instance_name inst_name , parallel , status , database_status db_status , active_state state , host_name host FROM gv$instance ORDER BY inst_id; 

INST_ID    INST_NO INST_NAME  PAR STATUS       DB_STATUS         STATE     HOST
------- ---------- ---------- --- ------------ ----------------- --------- ----------
      1          1 hisd1      YES OPEN         ACTIVE            NORMAL    ora01
      2          2 hisd2      YES OPEN         ACTIVE            NORMAL    ora02

SQL>

```