---
layout: post
title: HPE DP A1000 & Oracle 11gR2 backup and restore
date: 2018-01-09 12:00
author: felo
tags: oracle dp
typora-root-url: ../../yangfeiffei.github.io
---


# 1.

DP Server
```bash
windows server2008R2
DPA.10.00
```


```bash
[root@db-server ~]# cat /etc/redhat-release
Red Hat Enterprise Linux Server release 6.4 (Santiago)
```


```bash
export ORACLE_BASE=/u01/app/oracle
export ORACLE_HOME=$ORACLE_BASE/product/11.2.0/dbhome_1
export TMP=/tmp
export PATH=$PATH:$ORACLE_HOME/bin
export ORACLE_SID=test
```


```bash
Oracle Database 11g Enterprise Edition Release 11.2.0.4.0 - 64bit Production
```



```bash
localhost:backup$ scp HPE_DP_10.00_for_Linux_DP_A1000_GPLx86_64.tar.gz root@192.168.56.176:/root/
root@192.168.56.176's password: 
HPE_DP_10.00_for_Linux_DP_A1000_GPLx86_64.tar 100% 2797MB  17.1MB/s   02:43    
localhost:backup$ 
```

# 2.

## 2.1 host

Oracle 
```bash
[root@db-server ~]# vim /etc/hosts
[root@db-server ~]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
 
192.168.56.176 db-server
192.168.56.160 backupserver
[root@db-server ~]# ping backupserver -c 2
PING backupserver (192.168.56.160) 56(84) bytes of data.
64 bytes from backupserver (192.168.56.160): icmp_seq=1 ttl=128 time=1.64 ms
64 bytes from backupserver (192.168.56.160): icmp_seq=2 ttl=128 time=0.695 ms
 
--- backupserver ping statistics ---
2 packets transmitted, 2 received, 0% packet loss, time 1001ms
rtt min/avg/max/mdev = 0.695/1.172/1.649/0.477 ms

```



```bash
192.168.56.176  db-server
```

## 2.2 oracle

```bash
[root@db-server ~]# tar -zxvf HPE_DP_10.00_for_Linux_DP_A1000_GPLx86_64.tar.gz
...
[root@db-server LOCAL_INSTALL]# yum install xinetd -y
[root@db-server LOCAL_INSTALL]# chkconfig xinetd on
[root@db-server LOCAL_INSTALL]# service xinetd restart
Stopping xinetd:                                           [FAILED]
Starting xinetd:                                           [  OK  ]
[root@db-server LOCAL_INSTALL]# pwd
/root/DP_A1000_GPLx86_64/LOCAL_INSTALL
# da  oracle 
[root@db-server LOCAL_INSTALL]# ./omnisetup.sh -install da
[root@db-server LOCAL_INSTALL]# ./omnisetup.sh -install oracle8
...
Data Protector software package successfully installed
  Client was not imported into the cell.
  Please, perform the import manually by following the steps below.
    step1: Configure the cell manager by running /opt/omni/bin/omnicc -secure_comm -configure_peer <Cell Server> from this host.
    step2: Perform the import manually from cell manager or from one of the other clients of the cell.
 
  Installation/upgrade session finished.
# 
[root@db-server LOCAL_INSTALL]#  /opt/omni/bin/omnicc -secure_comm -configure_peer backupserver
 - Please use the fingerprint to validate the certificate manually!
Certificate information:
 - Hostname:backupserver
 - Valid: from Jan  3 15:39:57 2018 GMT until Jan  1 15:39:57 2028 GMT
 - Fingerprint: 31:ee:88:39:fb:4a:78:06:3b:d7:0c:b1:fd:00:c3:20:1c:5d:ba:76:e6:5c:89:ff:92:83:b1:61:58:f0:f9:61
Do you want to continue (y/n)?y
Host 'backupserver' configured for secure configuration successfully.
```



2.3 



![](/images/dp-oracle-backup/02e.png)

db-server`hosts`

![](/images/dp-oracle-backup/02f.png)


```bash
[root@db-server LOCAL_INSTALL]# service iptables stop
iptables: Flushing firewall rules:                         [  OK  ]
iptables: Setting chains to policy ACCEPT: filter          [  OK  ]
iptables: Unloading modules:                               [  OK  ]
[root@db-server LOCAL_INSTALL]# chkconfig iptables off
```


![](/images/dp-oracle-backup/030.png)



## 2.4 oracle

```bash
# disabled
SQL> archive log list
Database log mode              No Archive Mode
Automatic archival             Disabled
Archive destination            /u01/app/oracle/product/11.2.0/dbhome_1/dbs/arch
Oldest online log sequence     2
Current log sequence           4
SQL>
SQL>
SQL> select name,log_mode from v$database;
 
NAME      LOG_MODE
--------- ------------
TEST      NOARCHIVELOG
 
# 
SQL> shutdown immediate;
Database closed.
Database dismounted.
ORACLE instance shut down.
# mount
SQL> startup mount
ORACLE instance started.
 
Total System Global Area  839282688 bytes
Fixed Size                  2257880 bytes
Variable Size             545262632 bytes
Database Buffers          289406976 bytes
Redo Buffers                2355200 bytes
Database mounted.
# 
SQL> alter database archivelog;
 
Database altered.
# 
SQL> alter database open;
 
Database altered.
# enabled
SQL> archive log list;
Database log mode              Archive Mode
Automatic archival             Enabled
Archive destination            /u01/app/oracle/product/11.2.0/dbhome_1/dbs/arch
Oldest online log sequence     2
Next log sequence to archive   4
Current log sequence           4
```

## 2.5  oracle

Oracle Server

![](/images/dp-oracle-backup/031.png)

oracle

![](/images/dp-oracle-backup/032.png)

db-server
test
oracleDP
dba

![](/images/dp-oracle-backup/033.png)

oracle_home

![](/images/dp-oracle-backup/034.png)

`sysdba``sys`,`show parameter service`

![](/images/dp-oracle-backup/035.png)



![](/images/dp-oracle-backup/036.png)

`media-2`

![](/images/dp-oracle-backup/037.png)



# 3.

## 3.1  




![](/images/dp-oracle-backup/038.png)

## 3.2  



```bash
SQL> create user user001 identified by oracle;
User created.
SQL> grant connect,resource,dba to user001;
Grant succeeded.
SQL> create table user001.t1 as select * from dba_source;
Table created.
SQL> desc user001.t1
 Name                                      Null?    Type
 ----------------------------------------- -------- ----------------------------
 OWNER                                              VARCHAR2(30)
 NAME                                               VARCHAR2(30)
 TYPE                                               VARCHAR2(12)
 LINE                                               NUMBER
 TEXT                                               VARCHAR2(4000)
SQL> commit ;
Commit complete.
```



![](/images/dp-oracle-backup/039.png)



```bash
SQL> select name from v$datafile;
 
NAME
--------------------------------------------------------------------------------
/u01/app/oracle/oradata/TEST/datafile/o1_mf_system_f51j89no_.dbf
/u01/app/oracle/oradata/TEST/datafile/o1_mf_sysaux_f51j89ol_.dbf
/u01/app/oracle/oradata/TEST/datafile/o1_mf_undotbs1_f51j89oq_.dbf
/u01/app/oracle/oradata/TEST/datafile/o1_mf_users_f51j89p9_.dbf
SQL> exit
Disconnected from Oracle Database 11g Enterprise Edition Release 11.2.0.4.0 - 64bit Production
With the Partitioning, OLAP, Data Mining and Real Application Testing options
[oracle@db-server ~]$
[oracle@db-server ~]$ ll /u01/app/oracle/oradata/TEST/datafile/
total 1455176
-rw-r-----. 1 oracle oinstall 534781952 Jan  6 22:33 o1_mf_sysaux_f51j89ol_.dbf
-rw-r-----. 1 oracle oinstall 775954432 Jan  6 22:33 o1_mf_system_f51j89no_.dbf
-rw-r-----. 1 oracle oinstall  30416896 Jan  6 21:35 o1_mf_temp_f51jcc4f_.tmp
-rw-r-----. 1 oracle oinstall  78651392 Jan  6 22:33 o1_mf_undotbs1_f51j89oq_.dbf
-rw-r-----. 1 oracle oinstall  99622912 Jan  6 22:33 o1_mf_users_f51j89p9_.dbf
[oracle@db-server ~]$ rm /u01/app/oracle/oradata/TEST/datafile/*
[oracle@db-server ~]$ ll /u01/app/oracle/oradata/TEST/datafile/
total 0
```

mount

```bash
SQL> shutdown abort;
ORACLE instance shut down.
SQL> startup mount;
ORACLE instance started.
 
Total System Global Area  839282688 bytes
Fixed Size    2257880 bytes
Variable Size  545262632 bytes
Database Buffers  289406976 bytes
Redo Buffers    2355200 bytes
Database mounted.
SQL>
```

restorerecover

![](/images/dp-oracle-backup/03c.png)



![](/images/dp-oracle-backup/03b.png)



![](/images/dp-oracle-backup/03a.png)


,
```bash
SQL> select status from v$instance;
 
STATUS
------------
OPEN
SQL> select count(*) from user001.t1;
 
  COUNT(*)
----------
    636842
```

