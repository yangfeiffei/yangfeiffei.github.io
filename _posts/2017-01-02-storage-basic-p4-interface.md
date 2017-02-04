---

layout: post
title: Storage Basics – Part IV Interface
author: felo

---


摘要：
介绍存储接口相关的概念和内容








在一、二、三部分中我们已经知道， 现代存储系统的基本构建设备是：硬盘。特别的，IOPS是主要性能特征，使用RAID能够增加存储的性能。今天我们说一个跟存储性能相关的另外一个重要谜题：接口（interface）。接口在存储系统中能够描述很多事情，让我给你解释一下。
在大多数情况下，接口表示硬盘和系统（如主板，控制器，阵列）之间的物理连接。接口表示处理单元和存储设备之间的电子通讯机制、线缆、控制器等。通常接口有:IDE,SATA,SCSI,SAS,FC。在数据写到硬盘盘片之前，它必须经过接口。标准的硬盘定义接口外，还定义了通过接口的速度和能力，这也是另外一个衡量存储性能的因素：吞吐量（throughput）。吞吐量通常使用GBps或MBps作为单位。
通常接口的吞吐量如下表所示：


接口   |     速度
--|--
IDE    |    100MBps or 133MBps
SATA   |    1.5Gbps, 3.0Gbps, 6.0Gbps
SCSI    |   160MBps (Ultra-160) and 320MBps (Ultra-320)
SAS      |  1.5Gbps, 3.0Gbps, 6.0Gbps
FC       |  1Gb, 2Gb, 4Gb, 8Gb or 16Gb ( 200MBps, 400MBps, 800MBps and 1600MBps)


如果我们测试实际值，我们会发现320MB的SCSI和2GB的FC区别不大。从更深层次来看的话，其实简单的速度级别并不是最终故事。举个例子，FC的吞吐量会受到线缆长度的影响，同样的，拓扑也会影响速度：在SCSI中串行比并行有效，在FC中出现环路会有影响。
每个接口类型也定义不同的能力，就像他们使用不同的协议，总线上允许的最大磁盘数量，存储系统通讯中的控制设置。举个例子，SATA NCQ 就比并行的ATA的控制队列提供更高的性能，因此，你会看到一些使用SAS背板的SATA磁盘，SAS背板负责将SAS指令转换为SATA指令。【是不是NLSAS就是这么来的？】
如果你离开现在的直连模块，到共享存储环境如VMware虚拟化环境，你就会发现“接口”有了新的含义。现在仍然有总线接口连接磁盘到背板，比较流行的阵列一般使用SAS或者FC的背板。如果你有多个磁盘柜，你有一个接口连接磁盘架和控制器/头/存储处理器，或者相邻的磁盘柜。举个例子，EMC Clariion就是用FC线缆连接磁盘柜到存储"后端"处理器。

如果我们看向存储“前端”，“接口”被描述为服务器连接到SAN之间的中间协议。通常在SAN上的“前端”接口就是FC或者以太网。前端FC接口一般标准的是2Gb，4Gb，8Gb；以太网一般是1Gbps或者10Gbps。很多存储阵列支持多个前端端口，能够通过聚合增加带宽，或者通过多路径软件来增加负载均衡和故障切换。

大多数协议都能够运行在这些中间中，vmware现在支持在以太网上运行FC协议，iSCSI，NFS。FC和iSCSI是基于SCSI的块协议。NFS是NAS协议。FCoE支持多个存储阵列，通过以太网发送FC协议包。

决定使用什么样的“前端”和“后端”接口，取决于你对于你的工作负载的理解和你希望的性能级别。我来提供一些简单的规则，第一，获取性能统计：使用windows性能监视器，查看Physical Disk|Disk Read Bytes/sec or Disk Write Bytes/sec，或者在你的vSphere客户端上查看。
- 如果你想获得低延迟，使用FC；
- 如果你的吞吐量要求是60MBps，你需要用FC连接主机；
- iSCSI和NFS通常适合VMware的部署；

当你在选择合适的vmware内部连接部署的时候，有一大堆的指导和性能参数，也有大量的变量能够影响性能。可以从vmware 的白皮书开始（http://www.vmware.com/resources/techresources/10034）。也可以看一下Duncan Epping的文章（http://www.yellow-bricks.com/2010/01/07/fc-vs-nfs-vs-iscsi/），有关netapp的FC，iSCSI和NFS。如果通过SAN购买方式，可以请第三方收集的你环境的性能数值。存储原厂有一些非常酷的工具，可以收集和计算这些数据--不要害怕问他们如何配置这些工具。

下一部分我们来看看cache，controllers和coalescing。下一部分将讲一些复杂一些，针对vmware和Tier1负载，同时包含物理和虚拟环境。


---
本文翻译了vm today的一篇系列文章，原文地址如下：
[Storage Basics – Part I: An Introduction](http://vmtoday.com/2009/12/storage-basics-part-i-intro/)
[Storage Basics – Part II: IOPS](http://vmtoday.com/2009/12/storage-basics-part-ii-iops/#comments)
[Storage Basics – Part III: RAID](http://vmtoday.com/2010/01/storage-basics-part-iii-raid/)
[Storage Basics – Part IV: Interface](http://vmtoday.com/2010/01/storage-basics-part-iv-interface/)
