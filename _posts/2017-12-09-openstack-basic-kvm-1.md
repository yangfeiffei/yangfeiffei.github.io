---
layout: post
title: Openstack学习笔记-基础知识-KVM-1
date: 2017-12-09 12:00
author: felo
tags: openstack kvm
---


>基于内核的虚拟机（英语：Kernel-based Virtual Machine，缩写为 KVM），是一种用于Linux内核中的虚拟化基础设施，可以将Linux内核转化为一个hypervisor。KVM在2007年2月被导入Linux 2.6.20核心中，以可加载核心模块的方式被移植到FreeBSD及illumos上。
KVM在具备Intel VT或AMD-V功能的x86平台上运行。它也被移植到S/390[1]，PowerPC[2]与IA-64平台上。在Linux内核3.9版中，加入ARM架构的支持[3]。
KVM目前由Red Hat等厂商开发，对CentOS/Fedora/RHEL等Red Hat系发行版支持极佳。

![](https://zh.wikipedia.org/wiki/File:Libvirt_support.svg)

- KVM是开源软件，全称是kernel-based virtual machine（基于内核的虚拟机）。
- 是x86架构且硬件支持虚拟化技术（如 intel VT 或 AMD-V）的Linux全虚拟化解决方案。
- 它包含一个为处理器提供底层虚拟化 可加载的核心模块kvm.ko（kvm-intel.ko或kvm-AMD.ko）。
- KVM还需要一个经过修改的QEMU软件（qemu-kvm），作为虚拟机上层控制和界面。
- KVM能在不改变linux或windows镜像的情况下同时运行多个虚拟机，（它的意思是多个虚拟机使用同一镜像）并为每一个虚拟机配置个性化硬件环境（网卡、磁盘、图形适配器……）同时KVM还能够使用ksm技术帮助宿主服务器节约内存。
- 在主流的Linux内核，如2.6.20以上的内核均已包含了KVM核心。

上述摘自维基百科![KVM](https://zh.wikipedia.org/wiki/%E5%9F%BA%E4%BA%8E%E5%86%85%E6%A0%B8%E7%9A%84%E8%99%9A%E6%8B%9F%E6%9C%BA)

# 1.安装

```bash

[root@localhost ~]# uname -a
Linux localhost.localdomain 3.10.0-693.el7.x86_64 #1 SMP Tue Aug 22 21:09:27 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
[root@localhost ~]# cat /etc/redhat-release 
CentOS Linux release 7.4.1708 (Core) 


# 内核
[root@localhost 3.10.0-693.el7.x86_64]# find . -name kvm*
./kernel/arch/x86/kvm
./kernel/arch/x86/kvm/kvm-amd.ko.xz
./kernel/arch/x86/kvm/kvm-intel.ko.xz
./kernel/arch/x86/kvm/kvm.ko.xz
./kernel/drivers/gpu/drm/i915/gvt/kvmgt.ko.xz
[root@localhost 3.10.0-693.el7.x86_64]# pwd
/lib/modules/3.10.0-693.el7.x86_64

# 包
[root@localhost ~]# yum grouplist 
Loaded plugins: fastestmirror, langpacks
Loading mirror speeds from cached hostfile
Installed Environment Groups:
   Virtualization Host
...

[root@localhost ~]# rpm -qa |grep -E "^virt*"
virt-who-0.19-5.el7.noarch
virt-top-1.0.8-23.el7.x86_64
virt-manager-1.4.1-7.el7.noarch
virtuoso-opensource-6.1.6-6.el7.x86_64
virt-manager-common-1.4.1-7.el7.noarch
virt-viewer-5.0-7.el7.x86_64
virt-what-1.13-10.el7.x86_64
virt-install-1.4.1-7.el7.noarch

# virt-install：命令行工具
# virt-manager：图形工具
```

```bash
# virt-install
[root@localhost ~]# virt-install 
WARNING  KVM acceleration not available, using 'qemu'
ERROR    
--name is required
--memory amount in MiB is required
--disk storage must be specified (override with --disk none)
An install method must be specified
(--location URL, --cdrom CD/ISO, --pxe, --import, --boot hd|cdrom|...)
```

![](/images/openstack-basic-kvm/virt-manager-cut.png)



# 2.创建一个虚拟机

使用`virt-manager`的方法和使用virtualBox或者vmware workstation差不多，
不重复了。这里使用virt-install命令行创建虚拟机。

```bash
# virt-install创建一个最简单的虚拟机

# 从 http://download.cirros-cloud.net/0.3.3/ 下载一个镜像，作为虚拟机的硬盘
# 太高的版本使用的是qcow3，不能使用，这里使用0.3.3
[root@localhost data]# file cirros-0.3.3-x86_64-disk.img 
cirros-0.3.3-x86_64-disk.img: QEMU QCOW Image (v2), 41126400 bytes

# 开始创建
[root@localhost ~]# virt-install -n c01 \
-r 128 \
--disk path=/data/cirros-0.3.3-x86_64-disk.img,format=qcow2 \
--network=default \
--boot hd \
--noautoconsole
WARNING  KVM acceleration not available, using 'qemu'
WARNING  No operating system detected, VM performance may suffer. Specify an OS with --os-variant for optimal results.

Starting install...
Domain creation completed.

# -n：c01，虚拟机名称
# -r：128，内存128M
# --disk：硬盘情况
# --network：网络情况
# --boot：从硬盘启动，也可以使用hd,network
# --noautoconsole：默认不弹出启动窗口

# 查看安装情况
[root@localhost ~]# virsh 
Welcome to virsh, the virtualization interactive terminal.

Type:  'help' for help with commands
       'quit' to quit

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 2     c01                            running

virsh # console 2
Connected to domain c01
Escape character is ^]
[    0.000000] Initializing cgroup subsys cpuset
[    0.000000] Initializing cgroup subsys cpu
...
=== datasource: None None ===
=== cirros: current=0.3.3 uptime=261.20 ===
  ____               ____  ____
 / __/ __ ____ ____ / __ \/ __/
/ /__ / // __// __// /_/ /\ \ 
\___//_//_/  /_/   \____/___/ 
   http://cirros-cloud.net


login as 'cirros' user. default password: 'cubswin:)'. use 'sudo' for root.
cirros login: 

```

```bash
# 创建一个centos7虚拟机

[root@localhost data]# qemu-img create -f qcow2 centos7-1.qcow2 10G
Formatting 'centos7-1.qcow2', fmt=qcow2 size=10737418240 encryption=off cluster_size=65536 lazy_refcounts=off 
[root@localhost data]# virt-install -n centos7-1 \
-r 512 \
--disk path=/data/centos7-1.qcow2,format=qcow2 \
--network=default \
--cdrom=/data/centos7.iso \
--graphics vnc,password=123456,listen=0.0.0.0 \
--noautoconsole

# qemu-img 创建一个虚拟机使用的硬盘；
# --cdrom 为虚拟机增加一个光驱，注意iso格式，还需要调整这个iso的权限
# --graphics 图形界面，默认vnc
```

# 3.virsh管理虚拟机

```bash
[root@localhost data]# virsh
Welcome to virsh, the virtualization interactive terminal.

Type:  'help' for help with commands
       'quit' to quit

virsh # help
Grouped commands:

 Domain Management (help keyword 'domain'):
    attach-device                  attach device from an XML file
    attach-disk                    attach disk device
    attach-interface               attach network interface
    autostart                      autostart a domain
    blkdeviotune                   Set or query a block device I/O tuning parameters.
    blkiotune                      Get or set blkio parameters
    blockcommit                    Start a block commit operation.
    blockcopy                      Start a block copy operation.
    blockjob                       Manage active block operations
    blockpull                      Populate a disk from its backing image.
    blockresize                    Resize block device of domain.
    change-media                   Change media of CD or floppy drive
    console                        connect to the guest console
    cpu-baseline                   compute baseline CPU
    cpu-compare                    compare host CPU with a CPU described by an XML file
    cpu-stats                      show domain cpu statistics
    create                         create a domain from an XML file
    define                         define (but don't start) a domain from an XML file
    desc                           show or set domain's description or title
    destroy                        destroy (stop) a domain
    detach-device                  detach device from an XML file
    detach-disk                    detach disk device
    detach-interface               detach network interface
    domdisplay                     domain display connection URI
    domfsfreeze                    Freeze domain's mounted filesystems.
    domfsthaw                      Thaw domain's mounted filesystems.
    domfsinfo                      Get information of domain's mounted filesystems.
    domfstrim                      Invoke fstrim on domain's mounted filesystems.
    domhostname                    print the domain's hostname
    domid                          convert a domain name or UUID to domain id
    domif-setlink                  set link state of a virtual interface
    domiftune                      get/set parameters of a virtual interface
    domjobabort                    abort active domain job
    domjobinfo                     domain job information
    domname                        convert a domain id or UUID to domain name
    domrename                      rename a domain
    dompmsuspend                   suspend a domain gracefully using power management functions
    dompmwakeup                    wakeup a domain from pmsuspended state
    domuuid                        convert a domain name or id to domain UUID
    domxml-from-native             Convert native config to domain XML
    domxml-to-native               Convert domain XML to native config
    dump                           dump the core of a domain to a file for analysis
    dumpxml                        domain information in XML
    edit                           edit XML configuration for a domain
    event                          Domain Events
    inject-nmi                     Inject NMI to the guest
    iothreadinfo                   view domain IOThreads
    iothreadpin                    control domain IOThread affinity
    iothreadadd                    add an IOThread to the guest domain
    iothreaddel                    delete an IOThread from the guest domain
    send-key                       Send keycodes to the guest
    send-process-signal            Send signals to processes
    lxc-enter-namespace            LXC Guest Enter Namespace
    managedsave                    managed save of a domain state
    managedsave-remove             Remove managed save of a domain
    memtune                        Get or set memory parameters
    perf                           Get or set perf event
    metadata                       show or set domain's custom XML metadata
    migrate                        migrate domain to another host
    migrate-setmaxdowntime         set maximum tolerable downtime
    migrate-compcache              get/set compression cache size
    migrate-setspeed               Set the maximum migration bandwidth
    migrate-getspeed               Get the maximum migration bandwidth
    migrate-postcopy               Switch running migration from pre-copy to post-copy
    numatune                       Get or set numa parameters
    qemu-attach                    QEMU Attach
    qemu-monitor-command           QEMU Monitor Command
    qemu-monitor-event             QEMU Monitor Events
    qemu-agent-command             QEMU Guest Agent Command
    reboot                         reboot a domain
    reset                          reset a domain
    restore                        restore a domain from a saved state in a file
    resume                         resume a domain
    save                           save a domain state to a file
    save-image-define              redefine the XML for a domain's saved state file
    save-image-dumpxml             saved state domain information in XML
    save-image-edit                edit XML for a domain's saved state file
    schedinfo                      show/set scheduler parameters
    screenshot                     take a screenshot of a current domain console and store it into a file
    set-user-password              set the user password inside the domain
    setmaxmem                      change maximum memory limit
    setmem                         change memory allocation
    setvcpus                       change number of virtual CPUs
    shutdown                       gracefully shutdown a domain
    start                          start a (previously defined) inactive domain
    suspend                        suspend a domain
    ttyconsole                     tty console
    undefine                       undefine a domain
    update-device                  update device from an XML file
    vcpucount                      domain vcpu counts
    vcpuinfo                       detailed domain vcpu information
    vcpupin                        control or query domain vcpu affinity
    emulatorpin                    control or query domain emulator affinity
    vncdisplay                     vnc display
    guestvcpus                     query or modify state of vcpu in the guest (via agent)
    setvcpu                        attach/detach vcpu or groups of threads
    domblkthreshold                set the threshold for block-threshold event for a given block device or it's backing chain element

 Domain Monitoring (help keyword 'monitor'):
    domblkerror                    Show errors on block devices
    domblkinfo                     domain block device size information
    domblklist                     list all domain blocks
    domblkstat                     get device block stats for a domain
    domcontrol                     domain control interface state
    domif-getlink                  get link state of a virtual interface
    domifaddr                      Get network interfaces' addresses for a running domain
    domiflist                      list all domain virtual interfaces
    domifstat                      get network interface stats for a domain
    dominfo                        domain information
    dommemstat                     get memory statistics for a domain
    domstate                       domain state
    domstats                       get statistics about one or multiple domains
    domtime                        domain time
    list                           list domains

 Host and Hypervisor (help keyword 'host'):
    allocpages                     Manipulate pages pool size
    capabilities                   capabilities
    cpu-models                     CPU models
    domcapabilities                domain capabilities
    freecell                       NUMA free memory
    freepages                      NUMA free pages
    hostname                       print the hypervisor hostname
    maxvcpus                       connection vcpu maximum
    node-memory-tune               Get or set node memory parameters
    nodecpumap                     node cpu map
    nodecpustats                   Prints cpu stats of the node.
    nodeinfo                       node information
    nodememstats                   Prints memory stats of the node.
    nodesuspend                    suspend the host node for a given time duration
    sysinfo                        print the hypervisor sysinfo
    uri                            print the hypervisor canonical URI
    version                        show version

 Interface (help keyword 'interface'):
    iface-begin                    create a snapshot of current interfaces settings, which can be later committed (iface-commit) or restored (iface-rollback)
    iface-bridge                   create a bridge device and attach an existing network device to it
    iface-commit                   commit changes made since iface-begin and free restore point
    iface-define                   define an inactive persistent physical host interface or modify an existing persistent one from an XML file
    iface-destroy                  destroy a physical host interface (disable it / "if-down")
    iface-dumpxml                  interface information in XML
    iface-edit                     edit XML configuration for a physical host interface
    iface-list                     list physical host interfaces
    iface-mac                      convert an interface name to interface MAC address
    iface-name                     convert an interface MAC address to interface name
    iface-rollback                 rollback to previous saved configuration created via iface-begin
    iface-start                    start a physical host interface (enable it / "if-up")
    iface-unbridge                 undefine a bridge device after detaching its slave device
    iface-undefine                 undefine a physical host interface (remove it from configuration)

 Network Filter (help keyword 'filter'):
    nwfilter-define                define or update a network filter from an XML file
    nwfilter-dumpxml               network filter information in XML
    nwfilter-edit                  edit XML configuration for a network filter
    nwfilter-list                  list network filters
    nwfilter-undefine              undefine a network filter

 Networking (help keyword 'network'):
    net-autostart                  autostart a network
    net-create                     create a network from an XML file
    net-define                     define an inactive persistent virtual network or modify an existing persistent one from an XML file
    net-destroy                    destroy (stop) a network
    net-dhcp-leases                print lease info for a given network
    net-dumpxml                    network information in XML
    net-edit                       edit XML configuration for a network
    net-event                      Network Events
    net-info                       network information
    net-list                       list networks
    net-name                       convert a network UUID to network name
    net-start                      start a (previously defined) inactive network
    net-undefine                   undefine a persistent network
    net-update                     update parts of an existing network's configuration
    net-uuid                       convert a network name to network UUID

 Node Device (help keyword 'nodedev'):
    nodedev-create                 create a device defined by an XML file on the node
    nodedev-destroy                destroy (stop) a device on the node
    nodedev-detach                 detach node device from its device driver
    nodedev-dumpxml                node device details in XML
    nodedev-list                   enumerate devices on this host
    nodedev-reattach               reattach node device to its device driver
    nodedev-reset                  reset node device
    nodedev-event                  Node Device Events

 Secret (help keyword 'secret'):
    secret-define                  define or modify a secret from an XML file
    secret-dumpxml                 secret attributes in XML
    secret-event                   Secret Events
    secret-get-value               Output a secret value
    secret-list                    list secrets
    secret-set-value               set a secret value
    secret-undefine                undefine a secret

 Snapshot (help keyword 'snapshot'):
    snapshot-create                Create a snapshot from XML
    snapshot-create-as             Create a snapshot from a set of args
    snapshot-current               Get or set the current snapshot
    snapshot-delete                Delete a domain snapshot
    snapshot-dumpxml               Dump XML for a domain snapshot
    snapshot-edit                  edit XML for a snapshot
    snapshot-info                  snapshot information
    snapshot-list                  List snapshots for a domain
    snapshot-parent                Get the name of the parent of a snapshot
    snapshot-revert                Revert a domain to a snapshot

 Storage Pool (help keyword 'pool'):
    find-storage-pool-sources-as   find potential storage pool sources
    find-storage-pool-sources      discover potential storage pool sources
    pool-autostart                 autostart a pool
    pool-build                     build a pool
    pool-create-as                 create a pool from a set of args
    pool-create                    create a pool from an XML file
    pool-define-as                 define a pool from a set of args
    pool-define                    define an inactive persistent storage pool or modify an existing persistent one from an XML file
    pool-delete                    delete a pool
    pool-destroy                   destroy (stop) a pool
    pool-dumpxml                   pool information in XML
    pool-edit                      edit XML configuration for a storage pool
    pool-info                      storage pool information
    pool-list                      list pools
    pool-name                      convert a pool UUID to pool name
    pool-refresh                   refresh a pool
    pool-start                     start a (previously defined) inactive pool
    pool-undefine                  undefine an inactive pool
    pool-uuid                      convert a pool name to pool UUID
    pool-event                     Storage Pool Events

 Storage Volume (help keyword 'volume'):
    vol-clone                      clone a volume.
    vol-create-as                  create a volume from a set of args
    vol-create                     create a vol from an XML file
    vol-create-from                create a vol, using another volume as input
    vol-delete                     delete a vol
    vol-download                   download volume contents to a file
    vol-dumpxml                    vol information in XML
    vol-info                       storage vol information
    vol-key                        returns the volume key for a given volume name or path
    vol-list                       list vols
    vol-name                       returns the volume name for a given volume key or path
    vol-path                       returns the volume path for a given volume name or key
    vol-pool                       returns the storage pool for a given volume key or path
    vol-resize                     resize a vol
    vol-upload                     upload file contents to a volume
    vol-wipe                       wipe a vol

 Virsh itself (help keyword 'virsh'):
    cd                             change the current directory
    echo                           echo arguments
    exit                           quit this interactive terminal
    help                           print help
    pwd                            print the current directory
    quit                           quit this interactive terminal
    connect                        (re)connect to hypervisor
```

```bash
# 列出所有虚拟机
virsh # list --all
 Id    Name                           State
----------------------------------------------------
 2     c01                            running

# 温柔得关闭虚拟机
virsh # shutdown 2
Domain 2 is being shutdown

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 -     c01                            shut off

# 启动虚拟机
virsh # start c01
Domain c01 started

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 4     c01                            running

# 强行关闭虚拟机
virsh # destroy c01
Domain c01 destroyed

virsh # list --all
 Id    Name                           State
----------------------------------------------------
 -     c01                            shut off

# 删除虚拟机，不会删除虚拟机的磁盘文件
virsh # undefine c01
Domain c01 has been undefined

virsh # list --all
 Id    Name                           State
----------------------------------------------------

```

> - 虚拟机的id，每次主机临时生成，不固定；
	- `list`只列举在运行的虚拟机；
