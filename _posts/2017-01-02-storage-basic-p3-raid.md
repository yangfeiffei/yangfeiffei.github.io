---

layout: post
title: Storage Basics – Part III RAID
date: 2017-1-5 17:18:29
categories: storage
tags: storage RAID
author: felo

---

* content
{:toc}

摘要：
介绍RAID的定义和不同RAID的IOPS





这是这个系列的第三篇，前面两篇已经有一些很好的反馈，所以我们会坚持一些基本概念，在VMware环境中我们会深入到一些细节和实际应用。在这个系列的第二篇我介绍了IOPS的概念和如何解释一个硬盘驱动器的物理特性确定磁盘IOPS能力的理论。你可以总磁盘在特定的存储环境实现更大数量的IOPS。我们提出的概念可能不适合SAN或者最便宜的连接在计算机后的SATA，我们考虑中间状态-DAS（direct attached storage：直连存储）。
企业服务器和存储系统有能力将多个磁盘组合成一组使用冗余阵列的独立磁盘（RAID:磁盘阵列）技术。我们将假设一个RAID控制器负责配置和驱动存储IO到所连接的磁盘上。RAID控制器通常有电池支持的高速缓存，互连的驱动插件(如SCSI和SAS)，并持有的RAID配置设置包括条带大小和RAID级别。RAID控制器完成基本RAID设置：镜像，条带化，奇偶校验计算。有好几种RAID的级别，请移步维基百科中的[RAID](http://en.wikipedia.org/wiki/RAID)完成阅读后再回来。
好，我们现在知道RAID是通过多个磁盘的聚合来实现高性能，并通过镜像和奇偶校验来增加可靠性。现在让我们考虑一些基本的RAID级别对性能的影响。在IT行业的很多事情，是需要有所取舍的：比如**安全性与可用性**，**大脑与肌肉**，而现在是**性能与可靠性**。当我们通过镜像和奇偶校验在阵列中增加可靠性的时候，性能就会受到影响。确切的影响取决于RAID的类型，这里有一些常用的RAID级别，影响最大理论IOPS的因素有：

- I = 阵列总IOPS（注意读写分开）；
- i = 每个磁盘的IOPS(基于转速 RPM, 参考Part II: IOPS) ；
- n = 阵列中的磁盘数；
- r = 读百分比；
- w = 写百分比；

## RAID0 (striping条带化，不冗余)
这是最基本的聚合，没有冗余，一个磁盘的错误或失败就会使数据失效，不推荐生产中使用。

```
I =n*i 
```
因为没有镜像或校验开销，理论最大读与写的IOPS是相同的。

## RAID 1 & RAID10 (mirroring 镜像技术)

因为数据被镜像在多个磁盘中，
```
Read I = n*i    #读IOPS
```
举个例子，6个15k硬盘RAID 10,结果为：6 * 180 = 1080 IOPS
```
Write I = (n*i)/2       #写IOPS，每次写需要两个io
```

## RAID5 (一个校验盘，条带化)
```
Read I = (n-1)*i    #校验盘不参与读
```
举个例子: 5个15k磁盘组成RAID 5 (4 + 1) ，最大读IOPS值为： (5-1)*180 = 720 IOPS。 因为有一个磁盘是校验，没有数据。

```
Write I = (n*i)/4  #写IOPS
```
举个例子: 5个15k磁盘组成RAID 5 (4 + 1) ，最大写IOPS值为： (5*180)/4 = 225 IOPS

这些公式是非常基本的，实用价值不大，你会发现很少有一个系统只做读或只写。典型的VMware环境的情况下会是读取和写入的混合。你需要了解您的工作负载才是关键，以准确地估量您的存储环境的性能。下面是一个混合的读/写在RAID5的设置：
```
I = (n*i)/(r+4 *w)
```
举个例子: 60% read/40% write工作负载，5个15k磁盘 RAID5 would 提供 (5*180)/(.6+4*.4) = 409 IOPS.

前面都是从存储角度来看的，但我们从服务器/操作系统/应用程序方面看的话，就有趣了。用Windows perfmon 每隔15秒收集24个小时，用excel表统计95%的平均IOPS值。如果发现服务器95%需要1000 IOPS，60%读40%写负载，假设RAID5的负载上。因为raid5写惩罚因子4（RAID5写一次需要4次io，[看这个](http://www.yellow-bricks.com/2009/12/23/iops/)），我们调整公式：
```
I = Target workload IOPS  #目标负载iops
f = IO penalty # io惩罚，一次读或一次写需要的io次数
r = % Read 
w = % Write
```
```
IO = (I * r) + (I * w) * f  #
```
我们例子就像这样：
```
(1000 * .6) + ((1000 * .4) * 4) = 2200
```
简单的说，每1000IOPS的工作负载需求，需要后端存储提供2200IOPS，这也称作性能退化。再次，这是一个非常简单的方法，忽略了缓存、工作负载的随机性，IO大小，和分区对齐方式，都对后端的存储有影响，后续有说明。
正如你所看到的，物理定律加上简单的数学就可以得出一些有用的数据。一个基本的理解能够配置磁盘阵列，但是要配置存储还需要很长的一段路要走。您还可以发现，你需要根据你的要求购买更小、更快或更大、更慢的磁盘。

## 参考文献：
- [DB sizing for Microsoft Operations Manger, includes a nice chart with formulas similar to the ones I provided in this article: ](http://blogs.technet.com/jonathanalmquist/archive/2009/04/06/how-can-i-gauge-operations-manager-database-performance.aspx )
- [An Experts Exchange post with some good info in the last entry on the page (subscription required)]( http://www.experts-exchange.com/Storage/Storage_Technology/Q_22669077.html )
- [A Microsoft TechNet article with storage sizing for Exchange – a bit dated but still applicable: ](http://technet.microsoft.com/en-us/library/aa997052(EXCHG.65).aspx )
- [A simple whitepaper from Dell on their MD1000 DAS array – easy language to help the less technical along: ](http://support.dell.com/support/edocs/systems/md1120/multlang/whitepaper/SAS%20MD1xxx.pdf )
- [A great post that uses some math to show performance and cost trade-offs of RAID level, disk type, and spindle speed.  ](http://www.yonahruss.com/architecture/raid-10-vs-raid-5-performance-cost-space-and-ha.html )
- [Another nifty post that looks at cost vs. performance vs capacities of various disks speeds in an array ](http://blogs.zdnet.com/Ou/?p=322)

---
本文翻译了vm today的一篇系列文章，原文地址如下：
[Storage Basics – Part I: An Introduction](http://vmtoday.com/2009/12/storage-basics-part-i-intro/)
[Storage Basics – Part II: IOPS](http://vmtoday.com/2009/12/storage-basics-part-ii-iops/#comments)
[Storage Basics – Part III: RAID](http://vmtoday.com/2010/01/storage-basics-part-iii-raid/)