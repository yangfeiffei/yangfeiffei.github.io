# Compiling Lustre（编译lustre）

[TOC]

## 1.概述

lustre是一个开源软件项目，由全世界的软件工程师开发。这个项目的开发者提供持续集成、补丁的构建和测试。补丁为Lustre增加新的特性、更新已存在特性或者修复BUG。

The Lustre project's maintainers issue periodic releases of the software, extensively and comprehensively tested and qualified for general use. The releases include pre-built binary software packages for supported Linux-based operating system distributions. Many users of Lustre are content to rely upon the binary builds, and in general, it is recommended that the binary distributions from the Lustre project are used. Pre-built binary packages are available for download here:

https://wiki.whamcloud.com/display/PUB/Lustre+Releases

There are, of course, times when it is advantageous to be able to compile the Lustre software directly from source code, e.g. to apply a hot-fix patch for a recently uncovered issue, to test a new feature in development, or to allow Lustre to take advantage of a 3rd party device driver (vendor-supplied network drivers, for example).

## 2.约束

这个文档最初教导使用RHEL或者CentOS创建lustre包的方法。初步说明了如何在SLES 12 SP2编译lustre的方法。文档示范了编译基于ZFS的服务器和客户端，但未包含基于SLES和OFED或者LDISKFS。其他操作系统发行版会在将来增加。

**注意：**SUSE Linux 会将自定义编译的内核模块标记为不支持的。默认的情况下，SLES会拒绝加载没有支持标记的内核模块，会出现类似下面的错误：

```
sl12sp2-b:~ # modprobe zfs
modprobe: ERROR: module 'zavl' is unsupported
modprobe: ERROR: Use --allow-unsupported or set allow_unsupported_modules 1 in
modprobe: ERROR: /etc/modprobe.d/10-unsupported-modules.conf
modprobe: ERROR: could not insert 'zfs': Operation not permitted
sl12sp2-b:~ # vi /etc/modprobe.d/10-unsupported-modules.conf 
```

为了能够在SLES中加载自己编译的内核模块，需要在`/etc/modprobe.d/10-unsupported-modules.conf`中添加：

```
allow_unsupported_modules 1
```

 更多信息查看[SUSE documentation](https://www.suse.com/documentation/sles-12/book_sle_admin/data/sec_admsupport_kernel.html)。

## 3.计划

自从Lustre作为基于内核模块的网络文件系统，依赖于其他内核模块，包括设备驱动。一项最常规的任务是让发行版本的Linux允许Lustre内核模块作为第三方设备驱动运行。举个例子，OFA OFED提供的Infiniband驱动和RoCE网络，就是一个需要编译lustre的原因。

从源文件创建Lustre软件包有好几个可选项，每个选项有不同的作用。对于Lustre服务器，一个必选项是块存储文件系统。Lustre文件系统数据是分布在多个块存储文件系统的存储服务器中的。后端块存储被抽象成为对象存储设备（OSD）。OSD让Lustre能够使用不同的后端文件系统。

OSD有两个选择LDISKFS（基于EXT4）和ZFS，Lustre必须基于这两种OSD进行编译。

此外，用户必须决定使用那种网络驱动器。Linux内核内建支持了以太网和Infiniband，但是系统原厂商也提供了他们自己的设备驱动和工具。Lustre的网络栈（LNET），在编译Lustre的时候需要能够连接这些设备。

开始之前，阅读本文档并决定构建Lustre对的选项，文档包含下面几个方面：

1. 基于LDISKFS的Lustre
2. 基于ZFS的Lustre
3. Lustre包含下面几种网络驱动
   1. 内核已有的驱动
   2. OpenFabrics Alliance (OFA) OFED
   3. Mellanox OFED
   4. Intel Fabrics

## 4.建立编译环境

编译Lustre软件需要一台具有全功能安装开发工具的计算机。推荐用于编译的计算机，独立于安装Lustre的服务器。这台构建设备可以是专用服务器或者一台虚拟机，需要安装与目标服务器尽量相近的操作系统。

构建设备需要满足如下最小规格：

- 最小32GB存储空间，用于存放所有的源文件和软件开发工具

- 最小2GB内存（虚拟机当然最好）
- 网络接口能够访问外部软件库
- Linux发行版操作系统，关系到Lustre源代码中变更日志（changelog）中指定的发行版本信息
- 能够访问到文档中操作系统编译软件需要的软件包，通常这些包都包含在网络repo或者mirrors中

这个文档是基于Intel-compatible 64-bit (x86_64)架构开发的，这是Lustre文件系统大量部署且最流行的处理器架构。

除了正常的开源项目要求外，还依赖于编译和开发库，Lustre依赖的其他项目也需要从源代码进行创建。这就是说，在编译Lustre包的过程中，其他的软件包也会被编译并安装到构建设备中。Lustre在不使用超级用户权限下能够正常被编译，只要构建设备安装了标准的软件开发包，但是ZFS是不支持这种方法的。

然而，所有努力都是为了在构建过程中降低对超级用户权限的要求。对于RHEL或者CentOS用户来说，文档也包含了如何使用项目的描述，使用chroot创建一个用户构建软件包的环境。

GitHub中有一个项目，详细说明如https://github.com/rpm-software-management/mock

使用Mock是一个可选项，它提供了一个折中选项，相比传统的编译用法是一个异类用法。

### 4.1 创建一个用户来管理构建过程

在大部分情况下，虽然用户需要安装软件开发包和第三方软件，超级用户权限不是必须的。推荐使用一个具备附加权限的普通用户（可以使用sudo）来安装软件包。

### 4.2 RHEL/CentOS7：安装软件开发工具

有两个选项来创建Lustre包：使用Mock来创建chroot虚拟环境，或者直接使用构建服务器的操作系统。如何选择基于你的需要，两种方式都在下面进行描述。

#### 4.2.1 为Lustre创建Mock（模仿）配置

Mock提供了一个简单的方法来隔离操作系统中复杂的包构建配置。这是可选项，但是确实很有用的，特别是构建或者工作在多个项目的情况下。这个软件可以部署在RHEL、CentOS和Fedora。Mock通常用于开发者测试RPM构建，启动SRPM包，但是这个环境通常被用于开发区域。

安装Mock：

```
sudo yum -y install mock
```

还可以，安装Git，将repo克隆到Mock chroot环境之外：

```
sudo yum -y install git
```

增加一个Mock环境中的用户，在mock组之下：

```
sudo useradd -m <username>
sudo usermod -a -G mock[,wheel] <username>
```

wheel组是一个可选的，让用户能够使用`sudo`命令的组，用于提高构建主机的安全性。

下面的例子创建了`build`用户：

```
sudo useradd -m build
sudo usermod -a -G mock build
```

当软件包被安装后，为构建目标创建一个合适的配置。`Mock`配置文件被记录在`/etc/mock``，默认配置是default.cfg`，通常情况下是一个文件的软连接。使用RHEL或者CentOS的系统默认配置，使用下面的命令：

```
ln -snf /etc/mock/centos-7-x86_64.cfg /etc/mock/default.cfg
```

这些配置文件说明了当chroot环境启动后的包和repo，使用YUM会自动移至chroot并开始下载和安装。这些配置能够为这些用户被自定义，详细信息可以查看mock man page。

创建一个新的配置，指定编译Lustre和ZFS、OFED的要求，使用下面的命令（需要超级用户的权限）：

```
# Create a copy of the default CentOS 7 x86_64 Mock template and add the source repos
# 创建一个默认 CentOS 7 x86_64 Mock模版的拷贝，并增加源repo

sr=`cat /etc/yum.repos.d/CentOS-Sources.repo` \
awk '/^"""$/{print ENVIRON["sr"]; printf "\n%s\n",$0;i=1}i==0{print}i==1{i=0}' \
/etc/mock/centos-7-x86_64.cfg > /etc/mock/lustre-c7-x86_64.cfg
 
# Change the config name. Populate the Mock chroot with prerequisite packages.
# 修改配置名称，移植必要的包到Mock chroot环境
sed -i -e 's/\(config_opts\['\''root'\''\]\).*/\1 = '\''lustre-c7-x86_64'\''/' \
-e 's/\(config_opts\['\''chroot_setup_cmd'\''\]\).*/\1 = '\''install bash bc openssl gettext net-tools hostname bzip2 coreutils cpio diffutils system-release findutils gawk gcc gcc-c++ grep gzip info make patch redhat-rpm-config rpm-build yum-utils sed shadow-utils tar unzip util-linux wget which xz automake git xmlto asciidoc elfutils-libelf-devel zlib-devel binutils-devel newt-devel python-devel hmaccalc perl-ExtUtils-Embed patchutils pesign elfutils-devel bison audit-libs-devel numactl-devel pciutils-devel ncurses-devel libtool libselinux-devel flex tcl tcl-devel tk tk-devel expect glib2 glib2-devel libuuid-devel libattr-devel libblkid-devel systemd-devel device-mapper-devel parted lsscsi ksh libyaml-devel krb5-devel keyutils-libs-devel net-snmp-devel'\''/' \
/etc/mock/lustre-c7-x86_64.cfg
 
# Modify the %_rpmdir RPM macro to prevent build failures.
# 修改 %_rpmdir RPM macro避免构建失败

echo "config_opts['macros']['%_rpmdir'] = \"%{_topdir}/RPMS/%{_arch}\"" >> /etc/mock/lustre-c7-x86_64.cfg
 
# Make the new configuration the default
# 创建一个新配置
ln -snf /etc/mock/lustre-c7-x86_64.cfg /etc/mock/default.cfg
```

这个配置确保每次Mock环境创建的时候，所有的Lustre构建依赖都会自动下载和安装。

**Note:** Some of the build scripts and `Makefiles` used by Lustre and other projects assume that there will always be an architecture sub-directory (e.g. `x86_64`) in the RPM build directories. This is not always the case. In particular, Mock does not create sub-directories based on target architecture. To work around this problem, a custom RPM macro was added into the mock configuration above. If this does not work, then the same macro can be added by hand by running the following command after creating a Mock chroot environment:

```
mock --shell "echo '%_rpmdir %{_topdir}/RPMS/%{_arch}' >>\$HOME/.rpmmacros"
```

For information, an example of the build error for SPL manifests as follows:

```
cp: cannot stat ‘/tmp/spl-build-root-uDSQ5Bay/RPMS/*/*’: No such file or directory
make[1]: *** [rpm-common] Error 1
make[1]: Leaving directory `/builddir/spl'
make: *** [rpm-utils] Error 2
```

Once the Mock configuration has been created, login as the user that will be managing the builds and then run the following command to prepare the `chroot`environment:

```
mock [-r <config>] --init
```

The `-r` flag specifies the configuration to use, if the default configuration is unsuitable. This is either the name of one of the files in the `/etc/mock` directory, minus the `.cfg` suffix, or the name of a file.

To work interactively within the Mock environment, launch a shell:

```
mock [-r <config>] --shell
```

**Note:** some mock commands will attempt to clean the chroot directory before executing. This will remove any files considered temporary by Mock, which means anything that Mock itself has not provisioned. To avoid this situation, use the `-n` flag. The `--shell` command does not run a clean operation, so the `-n` flag is not required.

#### 4.2.2 为常规构建程序安装开发软件

如果使用Mock来构建Lustre包，跳过这个步骤。使用下面的命令来安装必要软件包：

```
sudo yum install asciidoc audit-libs-devel automake bc binutils-devel \
bison device-mapper-devel elfutils-devel elfutils-libelf-devel expect \
flex gcc gcc-c++ git glib2 glib2-devel hmaccalc keyutils-libs-devel \
krb5-devel ksh libattr-devel libblkid-devel libselinux-devel libtool \
libuuid-devel libyaml-devel lsscsi make ncurses-devel net-snmp-devel \
net-tools newt-devel numactl-devel parted patchutils pciutils-devel \
perl-ExtUtils-Embed pesign python-devel redhat-rpm-config rpm-build \
systemd-devel tcl tcl-devel tk tk-devel wget xmlto yum-utils zlib-devel
```

这些包足够用户构建Lustre、ZFS和第三方OFED设备驱动。

#### 4.2.3 CentOS：指定YUM使用Vault Repos安装旧的操作系统包

安装和升级软件的时候获取大版本最新的更新，是CentOS和RHEL的一个约定。yum repos这么定义是为了减低下载到废弃软件包的风险。然而，这种方式经常不是期望的。安装和升级策略可能增加限制，超过应用指定的软件包版本，包括内核。一个站定可能固定操作系统的版本和特殊的修正更新，或可能被他们的基础设施上的应用软件所限制。

所以，包括Lustre在内，对构建软件环境有影响。如果运行环境是指定的特殊操作系统版本，那么必须构建相似的环境。

为了在CentOS中促进这些限制，一种办法是使用CentOS Vault Repo，包含在线的各个CentOS版本的软件包。每个CentOS安装包含一个centos-release的软件包用于确认OS版本和提供YUM repo定义。这个软件包定义了当前CentOS安装的版本之前的repo，比如：CentOS6.9的centos-release，将定义Vault repo包括CentOS 6.0 - 6.8。

这个能够帮助构建服务器环境匹配目标环境。最简单的办法就是下载最新的centos-release包，覆盖现有Vault Repo配置。然后，禁用默认的repo，启用匹配目标版本的Vault repo。例如：

```
# Download and install an updated Vault definition:
mkdir $HOME/tmp
cd $HOME/tmp
yumdownloader centos-release
rpm2cpio centos-release*.rpm | cpio -idm
cp etc/yum.repos.d/CentOS-Vault.repo /etc/yum.repos.d/.

# Configure YUM to use only the repositories for the current OS:
yum-config-manager --disable \*

# Get the current OS major and minor version
ver=`sed 's/[^0-9.]*//g' /etc/centos-release`
# Enable the Vault repos that match the OS version
yum-config-manager --enable C$ver-base,C$ver-extras,C$ver-updates
```

**注意：**centos-release软件包不是自更新的。

### 4.3 SLES 12: Install the Software Development Tools

SUSE Linux Enterprise Server (SLES), like Red Hat Enterprise Linux, uses an RPM-based package management system, although there are some significant differences between the two platforms. In addition to the main subscription, the SUSE Linux Enterprise SDK (`SLE-SDK12-SP2`) add-on must also be enabled in order to be able to install the developer (i.e. `-devel`) packages.

Use the following command to install the prerequisite software on an SLES 12 SP2 build server:

```
sudo zypper install asciidoc automake bc binutils-devel bison bison \
device-mapper-devel elfutils libelf-devel flex gcc gcc-c++ git \
glib2-tools glib2-devel hmaccalc  libattr-devel libblkid-devel \
libselinux-devel libtool libuuid-devel lsscsi make mksh ncurses-devel \
net-tools numactl parted patchutils pciutils-devel perl pesign expect \
python-devel rpm-build sysstat systemd-devel tcl tcl-devel tk tk-devel wget \
xmlto zlib-devel libyaml-devel krb5-devel keyutils-devel net-snmp-devel
```

In some circumstances, the `zypper` may flag a dependency issue with `rpm-build`. For example:

```
Problem: rpm-build-4.11.2-15.1.x86_64 requires gettext-tools, but this requirement cannot be provided
  uninstallable providers: gettext-tools-0.19.2-1.103.x86_64[SUSE_Linux_Enterprise_Server_12_SP2_x86_64:SLES12-SP2-Pool]
 Solution 1: Following actions will be done:
  do not install rpm-build-4.11.2-15.1.x86_64
  do not install sysstat-10.2.1-9.2.x86_64
 Solution 2: deinstallation of gettext-runtime-mini-0.19.2-1.103.x86_64
 Solution 3: break rpm-build-4.11.2-15.1.x86_64 by ignoring some of its dependencies

Choose from above solutions by number or cancel [1/2/3/c] (c):
```

If this occurs, select `Solution 2: deinstallation of gettext-runtime-mini-<version>` to resolve.

## 5.获取Lustre源代码

下面的信息可以获取Lustre社区版本。获得其他发行版本的源代码，如Intel Enterprise Edition for Lustre，请查看厂商文档。

Lustre源代码使用git进行维护，获取克隆请运行如下命令：

```
# Mock users: run "mock --shell" first
cd $HOME
git clone git://git.whamcloud.com/fs/lustre-release.git
```

当repository 被克隆后，可以查看分支情况：

```
cd $HOME/lustre-release
git branch -av
```

For example:

```
[build@ctb-el73 lustre-release]$ git branch -av
* master                    fc7c513 LU-9306 tests: more debug info for hsm test_24d
  remotes/origin/HEAD       -> origin/master
  remotes/origin/b1_8       cfcb628 LU-4090 kernel: a kernel patch for jbd2 hung
  remotes/origin/b2_1       e91f649 LU-3546 mdt: define mdt_obd_name()
  remotes/origin/b2_2       25a1427 LU-549 llite: Improve statfs performance if selinux is disabled
  remotes/origin/b2_3       b491780 2.3.0-RC6
  remotes/origin/b2_4       d00e4d6 LU-4222 mdt: extra checking for getattr RPC.
  remotes/origin/b2_5       35bb857 LU-0000 build: update build version to 2.5.3.90
  remotes/origin/b2_6       73ea776 New tag 2.6.0-RC2
  remotes/origin/b2_7       8eb2659 New tag 2.7.0-RC4
  remotes/origin/b2_8       ea79df5 New tag 2.8.0-RC5
  remotes/origin/b2_9       e050996 New Lustre release 2.9.0
  remotes/origin/dne2       6354216 LU-5349 include: use __vmalloc() to avoid __GFP_FS default
  remotes/origin/master     fc7c513 LU-9306 tests: more debug info for hsm test_24d
  remotes/origin/multi-rail aa32cc5 LU-8998 pfl: PFL feature implementation
  remotes/origin/pfl        3e66aeb LU-9335 pfl: calculate PFL file LOVEA correctly
```

master分支是主要开发分支，是下个版本的基础分支。分支命名b开头，用版本数字来说明。如b2_9就是Lustre的2.9.0分支。其他分支用来长期开发，如Progressive File Layouts (PFL) 和LNet Multi-rail。

查看tag（标签）如下命令：

```
git tag
```

标签比分支要多得多，每个标签标识的是开发中的某个变化点。Lustre版本号有4位组成，major（主版本号）, minor（小版本号）, maintenance（修订版本号） and hot fix（补丁版本号）

- major feature release number
- minor feature release number
- maintenance releases number
- hot fix release number

A maintenance release version number of 0 (zero) indicates that the version is complete and is ready for general use (also referred to as generally available, or GA), and maintenance versions <=10 represent maintenance releases (bug fixes or minor operating system support updates). Tags with a maintenance version greater than 50 are development tags not to be considered for general use.

The tag labels in the `lustre-release` repository have two different formats:

tag标签使用两种格式：

- A dot-separated numerical version number (e.g. 2.9.0)
- A label beginning with lower-case "v" followed by the version number, separated by underscores (e.g. `v2_9_0_0`)

The different tag formats for a given version number are equivalent and refer to the same point in the git repository history. That is, tags `v2_9_0` and `2.9.0` refer to the same commit.

For example, the following tags represent the generally available release of Lustre version 2.9.0:

```
2.9.0
v2_9_0
v2_9_0_0
```

The next list of tags all point to the same development build:

```
2.9.56
v2_9_56
v2_9_56_0
```

Tags ending with the letters "RC" are release candidates: these are pre-production builds made for testing in anticipation of a final generally available (GA) release. If a release candidate is considered to be stable enough for general use, it is promoted to GA. There may be one or several RC builds before GA is declared for a given version of Lustre.

Use Git to checkout the Lustre release version that will be built. For example, to checkout Lustre version 2.9.0:

```
git checkout 2.9.0
```

or

```
git checkout v2_9_0_0
```

Prepare the build:

```
sh autogen.sh
```

Lustre source code is also available in package format, distributed alongside the binaries for a release. The latest software releases are available from the following URL:

<https://wiki.whamcloud.com/display/PUB/Lustre+Releases>

This page has links to all of the releases. For example, the source RPM for the latest Lustre release on RHEL or CentOS 7 can be downloaded here:

<https://downloads.whamcloud.com/public/lustre/latest-release/el7/server/SRPMS/>

**Note:** the examples used in the remainder of the documentation are based on a preview version of Lustre version 2.10.0 but the process applies equally to all recent Lustre releases.

## 6.LDISKFS和Linux内核补丁

### 6.1 概述

如果Lustre服务器将要使用LDISKFS OSD（起源于ext4），有两种编译Lustre和LDISKFS内核模块的选项。简单来说，给内核打补丁，不给内核打补丁（2.10版本后）。不打补丁的内核支持LDISKFS OSD在2017年开发并测试。然而，这简化了维护，提高的支持能力。使用不打补丁的服务器在RHEL和CentOS中意味着能够使用KABI兼容特性，以及更弱的内核升级支持。

从历史上看，Lustre服务器使用的Linux内核通常被要求增加额外的补丁，且不被高版本的内核和发行操作系统版本所支持。Lustre开发社区经常为了降低这些补丁包的依赖而工作，到现在这个差别已经足够小，至少在RHEL和CentOS服务器上，能够运行LDISKFS Lustre服务器而不用使用基于内核的补丁。

There are some caveats:

- 项目配额（Project quota ）支持，2.10开始，需要一些补丁包不在主流的Linux发行版本中。如果项目需要配额功能，就必须打内核补丁。
- 运行在没有补丁的环境中可能对性能有负面影响，虽然这个风险可能性很小。
- 没有内核补丁的LDISKFS服务器所进行的测试范围覆盖面很小。

选择不是很明确：内核补丁脱离了操作系统包，增加了维护量，但是具有很大的测试范围和长期的继承性。此外，某些功能只在内核补丁中提供。无内核补丁选项是新的功能，具有一定的质量风险，但是却便于维护。

> **注意：**运行“无补丁”的方式不是意味着Lustre使用了ext4设备，OSD仍然使用的是LDISKFS（ext4衍生而来），不管是否使用内核补丁，Lustre仍然需要访问内核源码来创建LDISKFS内核模块；

> **注意：**Lustre使用ZFS OSD不需要内核补丁，Lustre安装使用ZFS不需要一个自定义内核；

> **注意：**Lustre客户端不需要内核补丁

创建一个内核补丁，请阅读本节下面的内容。否则，可以略过。

### 6.2 接受Lustre内核补丁

本节余下内容描述了Lustre提供的操作系统内核的修改。使用“无补丁”内核的LDISKFS Lustre服务器在[creating the Lustre packages](#Create_the_Lustre_Packages)节中说明。

这些补丁提供了性能增强或者用户测试的额外用途。此外，项目配额支持需要很多内核的补丁，如果需要项目配额功能，那么这些补丁是必需的。

Lustre社区将为降低LDISKFS补丁的依赖而工作，期望将来有一天能够完全不再需要补丁。

### 6.3 Obtain the Kernel Source Code（获取内核源码）

如果目标构建是基于LDISKFS存储，下载对应的操作系统发行版本的内核源码。changelog中有每个版本的内核列表，维护了所有Lustre版本的历史记录。

下面是Lustre2.10.0版本支持的内容：

```
TBD Intel Corporation
       * version 2.10.0
       * See https://wiki.whamcloud.com/display/PUB/Lustre+Support+Matrix
         for currently supported client and server kernel versions.
       * Server known to build on patched kernels:
         2.6.32-431.29.2.el6 (RHEL6.5)
         2.6.32-504.30.3.el6 (RHEL6.6)
         2.6.32-573.26.1.el6 (RHEL6.7)
         2.6.32-642.15.1.el6 (RHEL6.8)
         2.6.32-696.el6      (RHEL6.9)
         3.10.0-514.16.1.el7 (RHEL7.3)
         3.0.101-0.47.71     (SLES11 SP3)
         3.0.101-97          (SLES11 SP4)
         3.12.69-60.64.35    (SLES12 SP1)
         4.4.49-92.14        (SLES12 SP2)
         vanilla linux 4.6.7 (ZFS only)
       * Client known to build on unpatched kernels:
         2.6.32-431.29.2.el6 (RHEL6.5)
         2.6.32-504.30.3.el6 (RHEL6.6)
         2.6.32-573.26.1.el6 (RHEL6.7)
         2.6.32-642.15.1.el6 (RHEL6.8)
         2.6.32-696.el6      (RHEL6.9)
         3.10.0-514.16.1.el7 (RHEL7.3)
         3.0.101-0.47.71     (SLES11 SP3)
         3.0.101-97          (SLES11 SP4)
         3.12.69-60.64.35    (SLES12 SP1)
         4.4.49-92.14        (SLES12 SP2)
         vanilla linux 4.6.7
```

In the above list, Lustre version 2.10.0 supports version `3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 kernel. Use YUM to download a copy of the source RPM. For example:

在上面的列表中，Lustre 2.10.0版本支持`3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 kernel，使用yum工具下载源RPM，如下：

```
cd $HOME
yumdownloader --source  kernel-3.10.0-514.16.1.el7
```

The following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then use that to download the kernel source:

下面的脚本能够鉴别内核版本和给定的Lustre版本，并使用下载内核源码：

```
cd $HOME
kernelversion=`os=RHEL7.3 lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`
[ -n "$kernelversion" ] && yumdownloader --source  kernel-$kernelversion || echo "ERROR: kernel version not found."
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

设置os和lu两个变量

If Mock is being used to build Lustre, you can download the source RPM from outside the mock shell and then copy it in as follows:

如果使用Mock，可以通过下面的脚本进行下载

```
mock --copyin <package> /builddir/.
```

For example:

```
mock --copyin kernel-3.10.0-514.16.1.el7.src.rpm /builddir/.
```

An alternative solution for Mock is to enable the CentOS-Source repository configuration, then run the `yumdownloader` command directly from the Mock shell. A simple, but crude way to add the source repositories into Mock's YUM configuration is to run the following from the Mock shell:

```
cat /etc/yum.repos.d/CentOS-Sources.repo >> /etc/yum/yum.conf
```

However, this will be overwritten on the next invocation of the mock shell. One can permanently update the configuration by appending the CentOS source repositories to the appropriate configuration file in the `/etc/mock` directory on the build host, and this what was done when preparing the Mock configuration earlier.

If it is necessary to create a build for an older kernel version, it might not be available in the active YUM repository for the distribution. CentOS maintains an archive or all previous releases in a set of YUM repositories called Vault. The CentOS Vault is located at:

[http://vault.centos.org](http://vault.centos.org/)

The Vault includes source RPMS, as well as binaries. Unfortunately, CentOS does not include YUM configuration descriptions for the archived source repositories. Instead of YUM, go the the Vault site directly and navigate through the directory structure to get the required files. For example, the source RPMS for the CentOS 7.2 package updates can be found here:

<http://vault.centos.org/7.2.1511/updates/Source/>

### Prepare the Kernel Source（准备内核源码）

Install the kernel source RPM that was downloaded in the previous step. This will create a standard RPM build directory structure and extract the contents of the source RPM:

下载内核源rpm包，这将会创建一个标准的rpm构建目录，并解压源rpm包中的内容：

```
cd $HOME
rpm -ivh kernel-[0-9].*.src.rpm
```

Determine the set of patches that need to be applied to the kernel, based on the operating system distribution. The file `lustre-release/lustre/kernel_patches/which_patch` maps the kernel version to the appropriate patch series. For example, for RHEL / CentOS 7.3 on Lustre 2.10.0, the file contains:

决定那些补丁包是发行版本系统内核需要的。lustre-release/lustre/kernel_patches/which_patch指定了内核版本和补丁包的对应关系，举个例子，RHEL / CentOS 7.3 on Lustre 2.10.0，包含如下内容：

```
3.10-rhel7.series       3.10.0-514.16.1.el7 (RHEL 7.3)
```

Review the list of patches in the series, e.g.:

查看补丁包：

```
[build@ctb-el7 ~]$ cat $HOME/lustre-release/lustre/kernel_patches/series/3.10-rhel7.series
raid5-mmp-unplug-dev-3.7.patch
dev_read_only-3.7.patch
blkdev_tunables-3.8.patch
jbd2-fix-j_list_lock-unlock-3.10-rhel7.patch
vfs-project-quotas-rhel7.patch
```

**Note:** one of the new features introduced with Lustre 2.10 is support for project quotas. This is a powerful administration feature that allows for additional quotas to be applied to the file system based on a new identifier called a project ID. To implement project quotas for LDISKFS means making a change to EXT4 code in the kernel. Unfortunately, this particular change breaks the kernel ABI (KABI) compatibility guarantee that is a feature of RHEL kernels. If this is a concern, then remove the patch called `vfs-project-quotas-rhel7.patch` from the patch series file. This action will effectively disable project quota support from Lustre LDISKFS builds.

注意：一个新的功能在2.10是支持“项目配额”。这是一个有力的管理功能，允许文件系统根据project ID使用额外的配额功能。为了使用LDISKFS项目配额功能，需要修改内核中的ext4代码。不巧的是，这个挑剔的改变破坏了RHEL内核中kernel ABI (KABI)兼容性保证。如果需要可以在文件中删除vfs-project-quotas-rhel7.patch。

When the correct patch series has been identified, create a patch file containing all of the kernel patches required by Lustre's LDISKFS OSD:

正确的补丁集被认证后，创建一个补丁文件包含LDISKFS OSD所需的所有内核补丁。

```
_TOPDIR=`rpm --eval %{_topdir}`
for i in `cat $HOME/lustre-release/lustre/kernel_patches/series/3.10-rhel7.series`; do
cat $HOME/lustre-release/lustre/kernel_patches/patches/$i
done > $_TOPDIR/SOURCES/patch-lustre.patch
```

Apply the following changes to the Kernel RPM spec file:

```
_TOPDIR=`rpm --eval %{_topdir}`
sed -i.inst -e '/find $RPM_BUILD_ROOT\/lib\/modules\/$KernelVer/a\
    cp -a fs/ext3/* $RPM_BUILD_ROOT/lib/modules/$KernelVer/build/fs/ext3 \
    cp -a fs/ext4/* $RPM_BUILD_ROOT/lib/modules/$KernelVer/build/fs/ext4' \
-e '/^# empty final patch to facilitate testing of kernel patches/i\
Patch99995: patch-lustre.patch' \
-e '/^ApplyOptionalPatch linux-kernel-test.patch/i\
ApplyOptionalPatch patch-lustre.patch' \
-e '/^%define listnewconfig_fail 1/s/1/0/' \
$_TOPDIR/SPECS/kernel.spec
```

These modifications ensure that the patches that Lustre requires for the LDISKFS OSD are applied to the kernel during compilation.

这些修改确保了LDISKFS OSD在编译中被内核所接收。

The following changes to the kernel configuration specification are also strongly recommended:

强烈推荐下面的内核配置的修改（为啥？不知道）：

```
CONFIG_FUSION_MAX_SGE=256
CONFIG_SCSI_MAX_SG_SEGMENTS=128
```

To apply these changes, run the following commands from the command shell:

在shell中运行下面的命令：

```
_TOPDIR=`rpm --eval %{_topdir}`
sed -i.inst -e 's/\(CONFIG_FUSION_MAX_SGE=\).*/\1256/' \
-e 's/\(CONFIG_SCSI_MAX_SG_SEGMENTS\)/\1128/' \
$_TOPDIR/SOURCES/kernel-3.10.0-x86_64.config
! `grep -q CONFIG_SCSI_MAX_SG_SEGMENTS $_TOPDIR/SOURCES/kernel-3.10.0-x86_64.config.inst` && \
echo "CONFIG_SCSI_MAX_SG_SEGMENTS=128" >> $_TOPDIR/SOURCES/kernel-3.10.0-x86_64.config
```

Alternatively, there is a `kernel.config` file distributed with the Lustre source code that can be used in place of the standard file distributed with the kernel. If using a file from the Lustre source, make sure that the first line of the file is as follows:

二选一，包含在Lustre源码中的kernel.config配置文件需要被替换到发行内核的标准文件。如果选用Lustre源码中的文件，请确保该文件第一行是：

```
# x86_64
```

The following script demonstrates the method for a RHEL / CentOS 7.3 kernel configuration:

下面的脚本说明了RHEL / CentOS 7.3 内核的配置方式：

```
_TOPDIR=`rpm --eval %{_topdir}`
echo '# x86_64' > $_TOPDIR/SOURCES/kernel-3.10.0-x86_64.config
cat $HOME/lustre-release/lustre/kernel_patches/kernel_configs/kernel-3.10.0-3.10-rhel7-x86_64.config >> $_TOPDIR/SOURCES/kernel-3.10.0-x86_64.config
```

### Create the kernel RPM packages （创建内核rpm包）

Use the following command to build the patched Linux kernel:

```
_TOPDIR=`rpm --eval %{_topdir}`
rpmbuild -ba --with firmware --with baseonly \
[--without debuginfo] \
[--without kabichk] \
--define "buildid _lustre" \
--target x86_64 \
$_TOPDIR/SPECS/kernel.spec
```

**Note:** the "`--with baseonly`" flag means that only the essential kernel packages will be created and the "`debug`" and "`kdump`" options will be excluded from the build. If the project quotas patch is used, the KABI verification must also be disabled using the "`--without kabichk`" flag.

注意："`--with baseonly`"意为只有必要的内核包被创建，"`debug`" 和 "`kdump`"选项将会被排除。如果项目配额补丁被使用，KABI必须被禁用，使用"`--without kabichk`"标记。

### Save the Kernel RPMs （保存内核rpm）

Copy the resulting kernel RPM packages into a directory tree for later distribution:

拷贝结果rpm包到指定的发行版本目录中：

```
_TOPDIR=`rpm --eval %{_topdir}`
mkdir -p $HOME/releases/lustre-kernel
mv $_TOPDIR/RPMS/*/{kernel-*,python-perf-*,perf-*} $HOME/releases/lustre-kernel
```

## 7.ZFS

Lustre servers that will be using ZFS-based storage targets require packages from the ZFS on Linux project ([http://zfsonlinux.org](http://zfsonlinux.org/)). The Linux port of ZFS is developed in cooperation with the OpenZFS project and is a versatile and powerful alternative to EXT4 as a file system target for Lustre OSDs. The source code is hosted on GitHub:

Lustre服务器使用基于ZFS的存储目标要求使用Linux ZFS项目提供的软件包。Linux支持的ZFS与OpenZFS项目合作，提供替代ext4作为Lustre OSD。源码包在GitHub上：

<https://github.com/zfsonlinux>

Pre-compiled packages maintained by the ZFS on Linux project are available for download. For instructions on how to incorporate the ZFS on Linux binary distribution into one of the supported operating systems, refer to the "Getting Started" documentation:

预编译包可以在ZFS on Linux项目中下载，如何将ZFS on Linux二进制安装到支持的操作系统中，参考"Getting Started"文档。

<https://github.com/zfsonlinux/zfs/wiki/Getting-Started>

The remainder of this section describes how to create ZFS packages from source.

本节剩余内容说明如何构建从源码构建ZFS包。

When compiling packages from the source code, there are three options for creating ZFS on Linux packages:

从源码编译安装包时，有三个创建ZFS on Linux的选项：

1. DKMS: packages are distributed as source code and compiled on the target against the installed kernel[s]. When an updated kernel is installed, DKMS-compatible modules will be recompiled to work with the new kernel. The module rebuild is usually triggered automatically on system reboot, but can also be invoked directly from the command-line
2. DKMS：发布为源码包，并在目标内核中进行编译。当升级的内核被安装时，DKMS兼容的模块将要被重新编译。这个模块通常在系统重启的时候被重建，也可能在命令行中直接重建。
3. KMOD: kernel modules built for a specific kernel version and bundled into a binary package. These modules are not portable between kernel versions, so a change in kernel version requires that the kernel modules are recompiled and re-installed.
4. KMOD：为指定的内核版本构建的内核模块，被捆绑为二进制包。这些模块在内核版本之间不可移动，因此内核版本的微小变化，要求重新编译和安装。
5. KMOD with kernel application binary interface (KABI) compatibility, sometimes referred to as "weak-updates" support. KABI-compliant kernel modules exploit a feature available in certain operating system distributions, such as RHEL, that ensure ABI compatibility across kernel updates in the same family of releases. If a minor kernel update is installed, the KABI guarantee means that modules that were compiled against the older variant can be loaded unmodified by the new kernel without requiring re-compilation from source.
6. 兼容KABI的KMOD，有时被认为是“弱升级”支持。KABI兼容的内核模块在特定的发行Linux版本中，如RHEL，确保在同一个家族的内核版本升级中ABI的兼容性。如果一个小内核升级安装后，KABI能够让模块被新内核不修改的情况下加载，而不用从源码重新编译。

The process for compiling ZFS and SPL is thoroughly documented on the ZFSonLinux GitHub site, but will be summarised here, as compiling ZFS has an implication on the Lustre build process. Each approach has its benefits and drawbacks.

编译ZFS和SPL十分详细的记录在ZFSonLinux GitHub上，在这里简单介绍，因为编译ZFS包含在Lustre编译过程中。

DKMS provides a straightforward packaging system and attempts to accommodate changes in the operating system by automatically rebuilding kernel modules, reducing manual overhead when updating OS kernels. DKMS packages are also generally easy to create and distribute.

DKMS提供一个可靠的包管理系统，通过自动重建内核模块，来试图适应操作系统，在升级操作系统内核的时候减低手动管理。DKMS包创建和发布通常很简单。

The KMOD packages take more work to create, but are easier to install. However, when the kernel is updated, the modules may need to be recompiled. KABI-compliant kernel modules reduce this risk by providing ABI compatibility across minor updates, but only work for some distributions (currently RHEL and CentOS).

KMOD包的创建更复杂，单是安装更简单。然而，当内核被更新后，模块可能需要重新编译。KABI-compliant内核模块在小版本升级中降低了提供ABI兼容的风险，但是只在部分发行版本有效（当前RHEL和CentOS）。

The premise of DKMS is simple: each time the OS kernel of a host is updated, DKMS will rebuild any out of tree kernel modules so that they can be loaded by the new kernel. This can be managed automatically on the next system boot, or can be triggered on demand. This does mean that the run-time environment of Lustre servers running ZFS DKMS modules is quite large, as it needs to include a compiler and other development libraries, but it also means that creating the packages for distribution is quick and simple.

DKMS的前提很简单：每次操作系统内核更新时，DKMS将要重建内核模块，使得他们可以被新内核加载。这些动作能够在下次重启自动进行，或者根据需要触发。这意味着Lustre服务器在运行状态下ZFS DKMS是很大的，因为它需要包含一个编译器和其他开发库，但是也说明创建和发布这些包都是简单而快速的。

Unfortunately, even the simple approach has its idiosyncrasies. You cannot build the DKMS packages for distribution without also building at least the SPL development packages, since the ZFS build depends on SPL, and the source code is simply not sufficient by itself.

不巧的是，甚至一个简单的处理都有它的特性。你不能创建DKMS包而不包含SPL开发包，因为ZFS构建依赖于SPL且源码简单。

There is also a cost associated with recompiling kernel modules from source that needs to be planned for. In order to be able to recompile the modules, DKMS packages require a full software development toolkit and dependencies to be installed on all servers. This does represent a significant overhead for servers, and is usually seen as undesirable for production environments, where there is often an emphasis placed on minimising the software footprint in order to streamline deployment and maintenance, and reduce the security attack surface.

从源码重新编译内核还有一个需要计划的地方。为了重新编译模块，DKMS包要求所有服务器安装全部软件开发工具包和依赖包。这需要加强开发和维护的软件包管理，将会降低安全性，在生产环境中通常是不友好的。

Rebuilding packages also takes time, which will lengthen maintenance windows. And there is always some risk that rebuilding the modules will fail for a given kernel release, although this is rare. DKMS lowers the up-front distribution overhead, but moves some of the cost of maintenance directly onto the servers and the support organisations maintaining the data centre infrastructure.

重建软件包需要一些时间，这样会增加维护窗口。虽然很罕见，但是也可能存在重建失败的风险。DKMS降低了版本升级的消耗，但是也增加了服务器和数据中心基础设施的直接维护成本。

When choosing DKMS, it is not only the ZFS and SPL modules that need to be recompiled, but also the Lustre modules. To support this, Lustre can also be distributed as a DKMS package.

当选择使用DKMS，不止是ZFS和SPL模块需要被重编译，同样包括Lustre模块。为了支持这些，Lustre也能够以DKMS包的方式发布。

**Note:** The DKMS method was in part adopted in order to work-around licensing compatibility issues between the Linux Kernel project, licensed under GPL, and ZFS which is licensed under CDDL, with respect to the distribution of binaries. While both licenses are free open source licenses, there are restrictions on distribution of binaries created using a combination of software source code from projects with these different licenses. There is no restriction on the separate distribution of source code, however. The DKMS modules provide a convenient workaround that simplifies packaging and distribution of the ZFS source with Lustre and Linux kernels. There are differences of opinion in the open source community regarding packaging and distribution, and currently no consensus has been reached.

注意：DKMS方式是解决Linux内核项目（GPL许可）和ZFS项目（CDDL许可）许可兼容性的一个方法。当两个许可都是开源许可时，有一些二进制文件的限制，但是从源码单独发布是没有限制的。DKMS模块提供了一个ZFS源码在Lustre和Linux内核中简化包装和发布的方法。当前，在开源社区仍有一些不同的主张还未被解决。

The vanilla KMOD build process is straightforward to execute and will generally work for any supported Linux distribution. The KABI variant of the KMOD build is very similar with the restriction that it is only useful for distributions that support KABI compatibility. The KABI build is also has some hard-coded directory paths in the supplied RPM spec files, which has effectively mandated a dedicated build environment for creating packages.

KMOD构建是可靠的，能够工作在支持的所有Linux发行版本。具有KABI变体的KMOD构建只能在支持KABI兼容的发行版本中。KABI构建有一些硬编码路径在rpm spec文件中，xxx

### Obtain the ZFS Source Code (获取ZFS源码)

If the target build will be based on ZFS, then acquire the ZFS software sources from the ZFS on Linux project. ZFS is comprised of two projects:

如果要基于ZFS创建，请到ZFSonLInux项目获得软件源码。ZFS由两部分组成：

- SPL: Solaris portability layer. This is a shim that presents ZFS with a consistent interface and allows OpenZFS to be ported to multiple operating systems.
- SPL：Solaris轻便层。这是一个允许OpenZFS能够在多操作系统运行的接口和垫片。
- ZFS: The OpenZFS file system implementation for Linux.
- ZFS：在linux上OpenZFS文件系统部署

Clone the SPL and ZFS repositories as follows:

克隆SPL和ZFS的repo如下：

```
# Mock users run "mock --shell" first
cd $HOME
git clone https://github.com/zfsonlinux/spl.git
git clone https://github.com/zfsonlinux/zfs.git
```

When the repositories have been cloned, change into the clone directory of each project and review the branches:

克隆repo后，为每个项目查看详细分支：

```
cd $HOME/spl
git branch -av
 
cd $HOME/zfs
git branch -av
```

For example:

举个例子：

```
[build@ctb-el73 spl]$ cd $HOME/spl
[build@ctb-el73 spl]$ git branch -av
* master                           8f87971 Linux 4.12 compat: PF_FSTRANS was removed
  remotes/origin/HEAD              -> origin/master
  remotes/origin/master            8f87971 Linux 4.12 compat: PF_FSTRANS was removed
  remotes/origin/spl-0.6.3-stable  ce4c463 Tag spl-0.6.3-1.3
  remotes/origin/spl-0.6.4-release c8acde0 Tag spl-0.6.4.1
  remotes/origin/spl-0.6.5-release b5bed49 Prepare to release 0.6.5.9
```

The master branch In each project is the main development branch and will form the basis of the next release of SPL and ZFS, respectively.

master分支是主要开发分支，分别是SPL和ZFS下一个版本的基础。

Review the tags as follows:

查看tags：

```
git tag
```

Just like the Lustre project, there are many more tags than there are branches, although the naming convention is simpler. Tags have the format `<name>-<version>`. The following output lists some of the tags in the spl repository:

就像Lustre项目一样，tags的数量远多于分支。tags的命名规格是<name>-<version>。下面的命令列出了SPL项目中的tags：

```
[build@ctb-el73 spl]$ git tag | tail -8
spl-0.6.5.6
spl-0.6.5.7
spl-0.6.5.8
spl-0.6.5.9
spl-0.7.0-rc1
spl-0.7.0-rc2
spl-0.7.0-rc3
spl-0.7.0-rc4
```

Tags with an `rc#` suffix are release candidates.

tags带rc#的表示候选发行版本。

Use Git to checkout the release version of SPL and ZFS that will be built and then run the `autogen.sh` script to prepare the build environment. For example, to checkout SPL version 0.6.5.9:

使用Git命令可以切换SPL和ZFS的发行版本，能否被创建且运行autogen.sh脚本准备构建环境。举个例子，切换到SPL 0.6.5.9版本。

```
cd $HOME/spl
git checkout spl-0.6.5.9
sh autogen.sh
```

To check out SPL version 0.7.0-rc4:

切换到SPL 0.7.0-rc4版本：

```
cd $HOME/spl
git checkout spl-0.7.0-rc4
sh autogen.sh
```

Do the same for ZFS. for example:

同样的对ZFS，举例：

```
cd $HOME/zfs
git checkout zfs-0.6.5.9
sh autogen.sh
```

For ZFS 0.7.0-rc4:

```
cd $HOME/zfs
git checkout zfs-0.7.0-rc4
sh autogen.sh
```

Make sure that the SPL and ZFS versions match for each respective checkout.

确定SPL和ZFS之间的版本需要互相匹配。

The ZFS on Linux source code is also available in the package format distributed alongside the binaries for a release. The latest software releases are available from the following URL:

ZFSonLinux提供源码的同时，也提供二进制发行版本。最新的软件版本如下URL：

https://github.com/zfsonlinux

Links are also available on the main ZFS on Linux site:

链接也在ZFSonLinux主站上：

http://zfsonlinux.org

**Note:** the examples used in the remainder of the documentation are based on a release candidate version of ZFS version 0.7.0, but the process applies equally to all recent releases.

注意：下面的例子是基于ZFS 0.7.0版本，但是构建过程与最近的版本是相同的。

### Install the Kernel Development Package

**安装内核开发包**

The SPL and ZFS projects comprise kernel modules as well as user-space applications. To compile the kernel modules, install the kernel development packages relevant to the target OS distribution. This must match the kernel version being used to create the Lustre packages. Review the ChangeLog file in the Lustre source code to identify the appropriate kernel version.

SPL和ZFS项目既包含内核模块，也包含用户层应用。为了编译内核模块，为有关的目标发行版安装内核开发包。这些必须与创建Lustre包的内核版本相匹配。在Lustre源码包查看Lustre的更新日志确定内核版本。

The following excerpt shows that Lustre version 2.10.0 supports version `3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 kernel, and version `4.4.49-92.14` of the SLES 12 SP2 kernel (output has been truncated):

下面的脚本显示了Lustre 2.10.0支持3.10.0-514.16.1.el7的RHEL / CentOS 7.3内核，`4.4.49-92.14` 的SLES 12 SP2内核。

```
TBD Intel Corporation
       * version 2.10.0
       * See https://wiki.whamcloud.com/display/PUB/Lustre+Support+Matrix
         for currently supported client and server kernel versions.
       * Server known to build on patched kernels:
...
         3.10.0-514.16.1.el7 (RHEL7.3)
...
         4.4.49-92.14        (SLES12 SP2)
...
```

**Note:** it is also possible to compile the SPL and ZFS packages against the LDISKFS patched kernel development tree, in which case, substitute the kernel development packages from the OS distribution with those created with the LDISKFS patches.

注意：有可能出现编译SPL和ZFS需要的内核开发树与LDISKFS内核补丁相冲突，这种情况下，使用LDISKFS内核补丁创建的替换内核开发包。

#### RHEL and CentOS

For RHEL / CentOS systems, use YUM to install the `kernel-devel` RPM. For example:

对于RHEL/CentOS系统，使用yum安装kernel-devel包：

```
sudo yum install kernel-devel-3.10.0-514.16.1.el7
```

If Mock is being used to create packages, install the `kernel-devel` RPM using the `mock --install` command:

在Mock环境下使用：

```
mock --install kernel-devel-3.10.0-514.16.1.el7
```

**Note:** you can, in fact, run YUM commands within the mock shell, as well.

注意：实际上mock shell中也运行的是yum命令

**Note:** similar to the way in which the kernel source can be automatically identified and installed for the LDISKFS patched kernel, the following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then use that to install the `kernel-devel` package:

注意：和构建LDISKFS内核补丁一样，使用下面的脚本确认内核版本，然后安装kernel-devel包：

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os=RHEL7.3 lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`
[ -n "$kernelversion" ] && $SUDOCMD yum -y install kernel-devel-$kernelversion || echo "ERROR: kernel version not found."
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

#### SLES 12 SP2

For SLES12 SP2 systems, use `zypper` to install the kernel development packages. For example:

```
sudo zypper install \
kernel-default-devel=4.4.59-92.17 \
kernel-devel=4.4.59-92.17 \
kernel-syms=4.4.59-92.17 \
kernel-source=4.4.59-92.17 
```

**Note:** the following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then use that to install the packages:

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os="SLES12 SP2" lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`

[ -n "$kernelversion" ] && $SUDOCMD zypper install \
kernel-default-devel=$kernelversion \
kernel-devel=$kernelversion \
kernel-syms=$kernelversion \
kernel-source=$kernelversion || echo "ERROR: kernel version not found."
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

### Create the SPL Packages

**创建SPL包**

Run the configure script:

运行配置脚本

```
cd $HOME/spl
# For RHEL and CentOS, set the --spec=redhat flag. Otherwise do not use.
./configure [--with-spec=redhat] \
[--with-linux=<path to kernel-devel>] \
[--with-linux-obj=<path to kernel-devel>]
```

The simplest invocation is to run the configure script with no arguments:

最简单的方法是无参数运行configure：

```
cd $HOME/spl
./configure
```

This is usually sufficient for most distributions such as SLES 12. To compile KABI-compliant kernel module packages for RHEL and CentOS distributions, use the `--with-spec=redhat` option:

这多用于SELS12，如果要为RHEL或者CentOS编译KABI-compliant的内核模块，使用`--with-spec=redhat` 选项：

```
cd $HOME/spl
# For RHEL and CentOS, set the --spec=redhat flag. Otherwise do not use.
./configure [--with-spec=redhat]
```

This option is not usable for other OS distributions.

这个选项对于其他操作系统发行版是无效的。

If there is only one set of kernel development packages installed, the `configure` script should automatically detect the location of the relevant directory tree. However, if there are multiple kernel development packages installed for different kernel versions and revisions, then use the `--with-linux` and optionally `--with-linux-obj`flags to identify the correct directory for the target kernel. For example:

如果只有一个内核开发包被安装，configure脚本会自动加载。然而，如果有多个内核开发包的话，需要使用`--with-linux` 和可选的 `--with-linux-obj进行选择。举个例子：`

```
cd $HOME/spl
./configure --with-spec=redhat \
--with-linux=/usr/src/kernels/3.10.0-514.16.1.el7.x86_64
```

Packages are created using the `make` command. There are three types of package that can be created from the SPL project. These are selected by providing parameters to the make command. One must create, at a minimum, the user-space packages, at least one other set of packages: the KMOD and/or DKMS packages.

使用make命令进行创建包，SPL项目有三个类型的包可以选择，通过不同的make命令。有一个必须创建，最小的用户空间的包，还有一个：KMOD或者DKMS包；

To compile the user-space tools, run this command:

编译用户空间的工具：

```
make pkg-utils
```

To create the kernel modules packages:

创建内核模块包：

```
make pkg-kmod
```

To create the DKMS package:

创建DKMS包：

```
make rpm-dkms
```

Since later process steps require that dependent packages be installed on the build server, always compile the user-space and KMOD packages even when the intended distribution will be DKMS. To compile all required sets of packages from a single command line invocation:

只要依赖包安装好，用户空间的工具和KMOD包或者DKMS包可以通过一条命令进行编译：

```
make pkg-utils pkg-kmod [rpm-dkms]
```

**Note:** DKMS packaging has not been evaluated for SLES.

注意：DKMS包在SLES未进行评估。

### Save the SPL RPMs

Copy the resulting RPM packages into a directory tree for later distribution:

复制结果rpm包到发行版本目录中：

```
mkdir -p $HOME/releases/zfs-spl
mv $HOME/spl/*.rpm $HOME/releases/zfs-spl
```

### Create the ZFS Packages

**创建ZFS包**

The build process for ZFS is very similar to that for SPL. The ZFS package build process has a dependency on SPL, so make sure that the SPL packages created in the previous step have been installed on the build host.

ZFS的创建过程与SPL十分相似，ZFS包构建过程依赖于SPL，所以要确保SPL包已经被安装在构建主机上。

#### RHEL / CentOS

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/zfs-spl/{spl-[0-9].*,kmod-spl-[0-9].*,kmod-spl-devel-[0-9].*}.x86_64.rpm
```

**Note:** it is not unusual for the installation to resolve additional dependencies, including the full kernel package for the version of the kernel that SPL was compiled for.

注意：安装额外的依赖是十分常见的，包含包含SPL的全部内核包。

#### SLES 12 SP2

```
sudo rpm -ivh kmod-spl-* spl-0.7.0*.x86_64.rpm
```

**Note:** The `rpm` command is used in the above example due to a peculiarity of the SLES packages for SPL (and which also affects ZFS). In the set of RPMs that are created, two of the packages have very similar names (`kmod-spl-devel-*`), differing only by the version numbering, as can be seen in the following example:

```
kmod-spl-devel-0.7.0-rc4.x86_64.rpm
kmod-spl-devel-4.4.21-69-default-0.7.0-rc4.x86_64.rpm
```

It is essential to install both packages but if both are specified on the command line invocation, the `zypper` command will only install one of them. The `rpm` command is not affected. To use `zypper` instead, so that dependencies are automatically resolved, run the command twice, with the second command containing just the "conflicting" RPM. For example:

```
sudo zypper install kmod-spl-4.4.21-69-default-0.7.0-rc4.x86_64.rpm \
kmod-spl-devel-0.7.0-rc4.x86_64.rpm \
spl-0.7.0*.x86_64.rpm
sudo zypper install kmod-spl-devel-4.4.21-69-default-0.7.0-rc4.x86_64.rpm
```

#### Prepare the build 准备构建

Run the configure script:

运行配置脚本：

```
cd $HOME/zfs
# For RHEL and CentOS only, set the --spec=redhat flag.
./configure [--with-spec=redhat] \
[--with-spl=<path to spl-devel> \
[--with-linux=<path to kernel-devel>] \
[--with-linux-obj=<path to kernel obj>]
```

The simplest invocation is to run the configure script with no arguments:

最简单的是不带参数的配置脚本：

```
cd $HOME/zfs
./configure
```

This is usually sufficient for most distributions such as SLES 12.

这通常用于大多数的发行版，如SLES 12

To compile KABI-compliant kernel module packages for RHEL and CentOS distributions, use the `--with-spec=redhat` option:

要编译包含KABI-compliant的内核如RHEL或CentOS发行版，需要增加参数--with-spec=redhat：

```
cd $HOME/zfs
# For RHEL and CentOS, set the --spec=redhat flag. Otherwise do not use.
./configure [--with-spec=redhat]
```

This option is not usable for other OS distributions.

这个选项对其他发行版无效。

If there is only one set of kernel development packages installed, the `configure` script should automatically detect the location of the relevant directory tree. However, if there are multiple kernel development packages installed for different kernel versions and revisions, then use the `--with-linux` and optionally `--with-linux-obj`flags to identify the correct directory for the target kernel.

如果只有一个kernel-devel版本configure会自动匹配，如果有多个kernel-devel版本，需要使用--with-linux或--with-linux-obj指定正确的内核开发包。

For example:

```
cd $HOME/zfs
./configure --with-spec=redhat \
--with-linux=/usr/src/kernels/3.10.0-514.16.1.el7.x86_64
```

In addition to the location of the kernel-devel RPM, the `configure` script may also need to be informed of the location of the SPL development installation (i.e. the location of the files installed from the `spl-devel` package, not the Git source code repository). For example:

除了指定kernel-devel包的位置外，可能还需要SPL开发包的安装位置（即spl-devel的位置），如下：

```
cd $HOME/zfs
./configure --with-spec=redhat \
--with-spl=/usr/src/spl-0.7.0 \
--with-linux=/usr/src/kernels/3.10.0-514.16.1.el7.x86_64
```

Packages are created using the `make` command. Just like SPL, there are three types of package that can be created from the ZFS project. These are selected by providing parameters to the `make` command. One must create, at a minimum, the user-space packages, at least one other set of packages: the KMOD and/or DKMS packages.

包的创建使用make命令。就像SPL，也有三种方式创建ZFS项目。至少需要创建用户空间的工具，此外，可选KMOD和或DKMS包；

#### Compile the Packages

To compile the user-space tools, run this command:

```
make pkg-utils
```

To create the kernel modules packages:

```
make pkg-kmod
```

To create the DKMS package:

```
make rpm-dkms
```

It is recommended that the user-space and KMOD packages are always compiled even when the intended distribution will be DKMS. To compile all sets of packages from a single command line invocation:

也可以使用一条命令：

```
make pkg-utils pkg-kmod [rpm-dkms]
```

### Save the ZFS RPMs

Copy the resulting RPM packages into a directory tree for later distribution:

```
mkdir -p $HOME/releases/zfs-spl
mv $HOME/zfs/*.rpm $HOME/releases/zfs-spl
```

## 8.3rd Party Network Fabric Support

**第三方光纤网络支持**

This section is optional since, by default, Lustre will use the device drivers supplied by the Linux kernel. Complete this section if 3rd party InfiniBand drivers are required for the target environment. The procedure for creating InfiniBand drivers from external sources varies depending upon the version of the InfiniBand software being used.

这个章节是可选的，默认情况下Lustre将使用Linux内核支持的设备驱动。如果目标环境需要第三方Infiniband驱动，请完成本章。这个从源码创建Infiniband驱动的过程依赖于使用的Infiniband软件。

Instructions are provided for each of the following driver distributions:

提供了下面集中驱动发行版：

- OpenFabrics Alliance (OFA) OFED*
- Mellanox OFED
- True Scale OFED
- Intel OmniPath Architecture (OPA)

*OFED: Open Fabrics Enterprise Distribution

**Note:** whichever distribution of OFED is selected, the resulting RPMs created during the build process for Lustre must be saved for distribution with the Lustre server packages.

**注意：**无论那个OFED被选择，结果rpm包都必须保存到发行版。

**Note:** The procedure in this section only prepares the distribution packages needed to compile Lustre from source. To create a full installation, follow the instructions provided by the driver vendor. Naturally, one can also use the full installation of the OFED packages on the build server instead of using the stripped-down procedure described here.

注意：本节的内容是从源码编译lustre唯一需要准备的内容，为了创建完整的安装包，下面说明提供了第三方驱动。

### Preparation

**准备**

Any 3rd party drivers must be compiled against the target kernel that will be used by Lustre. This is true for each of the InfiniBand driver distributions, regardless of vendor. If the target systems will be using LDISKFS for the storage, then use kernel packages that have been created with the Lustre LDISKFS patches applied. If the kernel for the target servers has not been patched for LDISKFS, then use the binary kernel packages supplied by the operating system.

任何第三方驱动程序都必须根据Lustre将使用的目标内核进行编译。这适用于所有InfiniBand驱动程序发行版，与供应商无关。如果目标系统使用LDISKFS存储，使用的内核包必须包含LDISKFS补丁。如果目标服务器的内核没有为LDISKFS补丁，则使用操作系统提供的二进制内核包。

**Note:** Only the `kernel-devel` package is needed for this part of the build process.

注意：本阶段构建过程只需要kernel-devel包。

#### Lustre-patched kernel-devel Package (for LDISKFS Server Builds)

**基于Lustre补丁包的kernel-devel包（LDISKFS服务器）**

For Lustre LDISKFS patched kernels, where the patched kernel has been recompiled from source, install the kernel-devel package as follows:

安装从源码编译的Lustre补丁包，生成的kernel-devel包：

```
SUDOCMD=`which sudo 2>/dev/null`
find `rpm --eval %{_rpmdir}` -type f -name kernel-devel-\*.rpm -exec $SUDOCMD yum localinstall {} \;
```

#### Unpatched `kernel-devel` Package (for ZFS-only Server and Lustre Client Builds)

**无补丁的kernel-devel包（为ZFS服务器或客户端）**

For "patchless" kernels, install the `kernel-devel` RPM that matches the supported kernel for the version of Lustre being compiled. Refer to the Lustre changelog in the source code distribution (`lustre-release/lustre/ChangeLog`) for the list of kernels for each OS distribution that are known to work with Lustre. The ChangeLog file contains a historical record of all Lustre releases.

找到lustre支持的内核版本，安装无补丁的kernel-devel rpm包。可以查看lustre源码中的changelog，找到合适的版本。

For example, Lustre version 2.10.0 supports version `3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 kernel. Use YUM to install the kernel-devel RPM:

例如，Lustre2.10.0版本支持`3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 内核。使用yum直接安装kernel-devel包。

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum install kernel-devel-3.10.0-514.16.1.el7
```

If Mock is being used to create packages, exit the Mock shell and install the kernel-devel RPM using the `mock --install` command:

使用mock的话用如下命令：

```
mock --install kernel-devel-3.10.0-514.16.1.el7
```

**Note:** similar to the way in which the kernel source can be automatically identified and installed for the LDISKFS patched kernel, the following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then use that to install the kernel-devel package:

注意：下面的命令监测并安装合适的kernel-devel版本

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os=RHEL7.3 lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`
[ -n "$kernelversion" ] && $SUDOCMD yum -y install kernel-devel-$kernelversion || echo "ERROR: kernel version not found."
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

设置os和lu变量即可。

For older RHEL / CentOS distributions, the required kernel might not be available in the active YUM repository for the distribution. CentOS maintains an archive or all previous releases in a set of YUM repositories called Vault, located at:

如果使用更老的RHEL或CentOS发行版，需要修改YUM repo，不同版本的YUM repo叫做Vault，可以查看：

[http://vault.centos.org](http://vault.centos.org/)

For example, the source RPMS for the CentOS 7.2 package updates can be found here:

比如CentOS7.2的源RPM和更新可以下面的位置找到：

<http://vault.centos.org/7.2.1511/updates/x86_64/Packages>

When the kernel-devel package has been downloaded, install it:

下载相应版本的kernel-devel，安装：

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum -y install kernel-devel-<version>*.rpm
```

#### OpenFabrics Alliance (OFA) Open Fabrics Enterprise Distribution (OFED)：OFA OFED

OFED Is maintained by the OpenFabrics alliance: [http://openfabrics.org](http://openfabrics.org/).

OFED被OpenFabrics所维护。

**Note:** At the time of writing, OFED 4.8-rc2 does not work with the latest Lustre release (Lustre 2.10.0), and that OFED 3.18-3 does not compile on RHEL / CentOS 7.3. It is therefore recommended that integrators and systems administrators use the in-kernel InfiniBand drivers, or the drivers supplied by the HCA vendor (Mellanox or Intel True Scale). Since it is rare for systems to make direct use of OFA OFED for production installations, using an alternative driver distribution is preferred in any case.

注意：到撰写本文时，OFED 4.8-rc2是不支持Lustre 2.10.0的，OFED 3.18-3不能被编译在RHEL/CentOS7.3。因此使用系统内置Infiniband驱动，或者使用厂商提供的驱动。

There are several releases of the OFED distribution, distinguished by version number, and the build process for each is different. OFED version 4 is the latest stable release at the time of writing (May 2017). There is also a version 3.18-3 stable release that is currently more mature but does not compile cleanly on RHEL / CentOS 7.3 or newer. Check the OFA web site for updates and to verify the releases that are compatible with the target operating system distribution.

有多个OFED发行版，使用版本号和构建号进行区分。OFED版本4是最新的版本。同时3.18-3也在维护但是还没有在RHEL和CentOS7.3得到支持。检查OFA网站查看更新和目标操作系统发行版。

Instructions are provided for OFED-4.8-rc2 but the method is equivalent for all 4.x and 3.x releases.

本文档说明了OFED-4.8-rc2版本，但是与所有的4.x和3.x版本是一样的。

**Note:** in OFED version 3 and 4, the kernel drivers are contained in the `compat_rdma` RPM. In versions of OFED prior to release 3, the IB kernel drivers were contained in a source RPM called `ofa_kernel`, which in turn built `kernel-ib` and related binary packages.

注意：在OFED版本3和4，内核驱动包含在`compat_rdma` RPM中。在版本3，IB内核驱动包含在源码RPM中，叫做ofa_kernel又构建kernel-ib和相关的二进制包。

Download the OpenFabrics (OFA) OFED software distribution from <http://downloads.openfabrics.org/OFED>., and extract the tarball bundle. For example, to download OFED-4.8-rc2:

下载OpenFabrics (OFA) OFED软件发行版。

```
cd $HOME
wget http://downloads.openfabrics.org/OFED/ofed-4.8/OFED-4.8-rc2.tgz
tar zxf $HOME/OFED-4.8-rc2.tgz
```

#### Intel True Scale InfiniBand

Intel provides a software distribution, derived from OFED, to support its True Scale InfiniBand host channel adapters (HCAs). The distribution can be downloaded from Intel's download centre:

Intel提供OFED的软件发行版驱动，支持True Scale InfiniBand的HCA卡，可以在Intel网站下载：

[https://downloadcenter.intel.com](https://downloadcenter.intel.com/)

Once downloaded, extract the Intel-IB bundle. For example:

下载解压，如下：

```
cd $HOME
tar zxf $HOME/IntelIB-Basic.RHEL7-x86_64.7.4.2.0.6.tgz
```

#### Mellanox InfiniBand

Mellanox provides its own distribution of OFED, optimised for the Mellanox chipsets. Occasionally referred to as MOFED. The software can be downloaded from the Mellanox web site:

Mellanox提供了自己的OFED发行版，为Mellanox主板进行优化。叫做MOFED。可以在Mellanox网站下载：

<http://www.mellanox.com/page/software_overview_ib>

Once downloaded, extract the Mellanox OFED bundle. For example:

下载解压，如下：

```
cd $HOME
tar zxf $HOME/MLNX_OFED_SRC-3.4-2.1.8.0.tgz
```

While the overall process for compiling the Mellanox kernel driver is similar to that for OFA and Intel OFED distributions, Mellanox packages the kernel drivers into an source RPM called `mlnx-ofa_kernel`, rather than `compat-rdma`.

编译过程和OFA以及Intel的发行版本类似，Mellanox将内核驱动包打包命名为mlnx-ofa_kernel，而不是compat-rmda。

#### Intel Omni-Path Architecture

Recent releases of the Intel Omni-Path host fabric interface (HFI) adapters use the drivers supplied by the distribution kernel and do not normally require a customised driver build. However, there are occasionally driver updates included in the IFS distribution from Intel, which may need to be recompiled for LDISKFS kernels. The same is true for older releases of the Intel Omni-Path software. Kernel driver updates are distributed in a `compat-rdma` kernel driver package which can be treated in the same way as for Intel True Scale OFED distributions.

Intel Omni-Path主机卡HFI的驱动使用发行内核，而不需要自己创建驱动。然而，有一些驱动的更新，可能需要编译到LDISKFS内核中。老的Intel Omni-Path软件版本也是如此。内核驱动程序更新发行在compat-rdma内核驱动程序包中，可以用与Intel True Scale OFED分发版相同的方式处理该内核驱动程序包。

### Compiling the Network Fabric Kernel Drivers

**编译网络光纤内核驱动**

There are many options available for the IB kernel driver builds and it is important to review the documentation supplied with the individual driver distributions to ensure that appropriate options required by the target environment are selected.



The options used in the following example are based on the default selections made by the distributions' install software. These should meet most requirements for x86_64-based systems and be suitable for each of the different vendors. The command-line can be used to build the `compat-rdma` packages for OFA OFED, Intel True Scale, as well as the `mlnx-ofa_kernel` package for Mellanox OFED. Some options for OFED are only available on specific kernels or processor architectures and these have been omitted from the example:

```
rpmbuild --rebuild --nodeps --define 'build_kernel_ib 1' --define 'build_kernel_ib_devel 1' \
--define 'configure_options --with-addr_trans-mod --with-core-mod --with-cxgb3-mod --with-cxgb4-mod --with-ipoib-mod --with-iser-mod --with-mlx4-mod --with-mlx4_en-mod --with-mlx5-mod --with-nes-mod --with-srp-mod --with-user_access-mod --with-user_mad-mod --with-ibscif-mod --with-ipath_inf-mod --with-iscsi-mod --with-qib-mod --with-qlgc_vnic-mod' \
 --define 'KVERSION <version>-<release>.<os-dist>.x86_64' \ 
--define 'K_SRC /usr/src/kernels/<version>-<release>.<os-dist>.x86_64' \
--define 'K_SRC_OBJ /usr/src/kernels/<version>-<release>.<os-dist>.x86_64' \ 
--define '_release <version>_<release>.<os-dist>' \ 
<distribution directory>/SRPMS/<package-name>-<version>-<release>.src.rpm 
```

**Note:** In the command line arguments, the definition of the variable `configure_options` must appear on a single line. 

Pay special attention to the `KVERSION</code<, <code>K_SRC`, `K_SRC_OBJ` and `_release` variables. These must match the target kernel version. In addition, the `_release`variable must not contain any hyphen (-) characters. Instead, replace hyphens with underscore (_). The `_release` variable is optional, but recommended as it will help to associate the package build with the kernel version.

The following is a complete example, using kernel version `3.10.0-514.16.1.el7_lustre.x86_64` (a Lustre-patched kernel for RHEL / CentOS 7.3 built using the process described earlier in this document). At the beginning of the example are variables pointing to the kernel driver packages for each of the major distributions:

```
# OFA OFED 3.x
# ofed_driver_srpm=$HOME/OFED-3.*/SRPMS/compat-rdma-3.*.rpm
# OFA OFED 4.x
# ofed_driver_srpm=$HOME/OFED-4.*/SRPMS/compat-rdma-4.*.src.rpm
# Intel True Scale
# ofed_driver_srpm=$HOME/IntelIB-Basic.RHEL7-x86_64.7.*/IntelIB-OFED.RHEL7-x86_64.3.*/SRPMS/compat-rdma-3.*.src.rpm
# Mellanox OFED 3.x
# ofed_driver_srpm=$HOME/MLNX_OFED_SRC-3.*/SRPMS/mlnx-ofa_kernel-3.*-OFED.3.*.src.rpm
 
ofed_driver_srpm=$HOME/IntelIB-Basic.RHEL7-x86_64.7.*/IntelIB-OFED.RHEL7-x86_64.3.*/SRPMS/compat-rdma-3.*.src.rpm
kernel_dev=3.10.0-514.16.1.el7_lustre.x86_64
kernel_release=`echo $kernel_dev|sed s'/-/_/g'`
 
rpmbuild --rebuild --nodeps --define 'build_kernel_ib 1' --define 'build_kernel_ib_devel 1' \
--define 'configure_options --with-addr_trans-mod --with-core-mod --with-cxgb3-mod --with-cxgb4-mod --with-ipoib-mod --with-iser-mod --with-mlx4-mod --with-mlx4_en-mod --with-mlx5-mod --with-nes-mod --with-srp-mod --with-user_access-mod --with-user_mad-mod --with-ibscif-mod --with-ipath_inf-mod --with-iscsi-mod --with-qib-mod --with-qlgc_vnic-mod' \
--define "KVERSION $kernel_dev" \
--define "K_SRC /usr/src/kernels/$kernel_dev" \
--define "K_SRC_OBJ /usr/src/kernels/$kernel_dev" \
--define "_release $kernel_release" \
$ofed_driver_srpm
```

The result is a set of kernel drivers for InfiniBand devices that are compatible with the kernel that will be used by Lustre.

An alternative method is to use the standard OFED install script. The following example shows how to supply additional options the the standard OFED installer:

```
cd $HOME/*OFED*/
./install.pl \
--kernel <kernel version> \
--linux /usr/src/kernels/<kernel-devel version> \
--linux-obj /usr/src/kernels/<kernel-devel version>
```

This will run through the insteractive build and install process, with options to select the various packages. Since the Lustre build process requires only the kernel drivers, the documentation uses the direct rpmbuild command, which in turn makes the possibility of automation easier to incorporate.

The Mellanox OFED `install.pl` script is similar, but has more options to control how the build is prosecuted. For example:

```
cd $HOME/*OFED*/
./install.pl --build-only --kernel-only \
--kernel <kernel version> \
--kernel-sources /usr/src/kernels/<kernel-devel version>
```

Intel's IFS IB install script is quite different form the OFA and Mellanox OFED scripts, and does not provide an obvious means to specify the kernel version. Nevertheless, using the more direct `rpmbuild` command above should result in suitable kernel drivers being created for whichever driver distribution is required.

### Save the Driver RPMs

Copy the resulting RPM packages into a directory tree for later distribution:

```
_TOPDIR=`rpm --eval %{_topdir}`
mkdir -p $HOME/releases/ofed
mv $_TOPDIR/RPMS/*/*.rpm $HOME/releases/ofed
```

## 9.Create the Lustre Packages 创建Lustre包

### Preparation 准备工作

When compiling the Lustre packages from source, the build environment requires access to the kernel development package for the target Linux kernel, and in the case of patchless LDISKFS servers, the kernel source code is also needed. The requirements are as follows:

当从源码编译Lustre包，构建环境需要能够访问目标Linux内核的内核开发包。即使是无补丁包的LDISKFS服务器，内核源码也是必须的。要求如下：

- Patched LDISKFS Lustre servers require the kernel development package that has been created with the Lustre patches applied (the "lustre-patched kernel")
- 有补丁LDISKFS Lustre服务器要求包含Lustre补丁的内核开发包（lustre-patched kernel）
- [Experimental] **\*Patchless*** LDISKFS Lustre servers require the standard kernel development package and the matching kernel source code package.
- 无补丁LDISKFS Lustre服务器要求匹配内核版本的标准内核开发包
- ZFS-based Lustre servers and all Lustre clients require the standard kernel development package
- 基于ZFS的Lustre服务器和所有客户端要求标准内核开发包

Also required are any 3rd party network device drivers not distributed with the kernel itself; typically this means InfiniBand drivers from one of the OFED distributions (either `compat-rdma-devel` or `mlnx-ofa_kernel-devel`).

同时要求不是内核自带的第三方网络设备驱动，通常意味着其中一种OFED发行版（`compat-rdma-devel` 或者`mlnx-ofa_kernel-devel`）

### Lustre Server (DKMS Packages only)

The process for creating a Lustre server DKMS package is straightforward:

```
_TOPDIR=`rpm --eval %{_topdir}`
cd $HOME/lustre-release
./configure --enable-dist
make dist
cp lustre-*.tar.gz $_TOPDIR/SOURCES/
rpmbuild -bs lustre-dkms.spec
rpmbuild --rebuild $_TOPDIR/SRPMS/lustre-dkms-*.src.rpm
mkdir -p $HOME/releases/lustre-server-dkms
mv $_TOPDIR/RPMS/*/*.rpm $HOME/releases/lustre-server-dkms
```

If the objective is to create a set of DKMS server packages for use with ZFS, then there is no further work required. See also the section on creating DKMS packages for Lustre clients, if required.

### Lustre Server (All other builds)

To compile the Lustre server packages requires the development packages for the Linux kernel, and optionally, SPL, ZFS and OFED. The packages used in the following examples have been taken from the builds created in the earlier stages of this process.

#### Patched LDISKFS Server Builds

For Lustre LDISKFS patched kernels (including optional project quota patches), install the kernel development package or packages with the patches compiled in. For example:

```
SUDOCMD=`which sudo 2>/dev/null`
INSTCMD=`which yum 2>/dev/null || which zypper 2>/dev/null`
$SUDOCMD $INSTCMD localinstall $HOME/releases/lustre-kernel/kernel-devel-\*.rpm
```

#### ZFS and/or Patchless LDISKFS Server Builds

For "patchless" kernels, install the `kernel-devel` package that matches the supported kernel for the version of Lustre being compiled. Refer to the Lustre ChangeLog in the source code distribution (`lustre-release/lustre/ChangeLog`) for the list of kernels that are known to work with Lustre. The `ChangeLog` file contains a historical record of all Lustre releases.

For LDISKFS patchless kernels, also download and install the kernel source code package that matches the target kernel.

##### RHEL / CentOS 7 Kernel Development Packages

For RHEL / CentOS 7, use `yum` to install the set of kernel development packages required by Lustre. For example, to install version `3.10.0-514.16.1.el7` of the RHEL / CentOS 7.3 `kernel-devel` RPM:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum install kernel-devel-3.10.0-514.16.1.el7
```

For LDISKFS support with an unpatched kernel, install the `kernel-debuginfo-common` package, which contains the EXT4 source code needed to create the LDISKFS kernel module. For example:

```
sudo yum install --disablerepo=* --enablerepo=base-debuginfo kernel-debuginfo-common-x86_64-3.10.0-514.16.1.el7
```

Alternatively, one can download the source RPM package and copy the EXT4 source code into place:

```
yumdownloader --source kernel-3.10.0-514.16.1.el7
rpm -ivh kernel-3.10.0-514.16.1.el7.src.rpm
tar Jxf $HOME/rpmbuild/SOURCES/linux-3.10.0-514.16.1.el7.tar.xz linux-*/fs/ext{3,4}
sudo cp -an $HOME/linux-3.10.0-514.16.1.el7/fs/ext{3,4} \
  /usr/src/kernels/3.10.0-514.16.1.el7.x86_64/fs/.
```

The following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then use that to install the `kernel-devel` and kernel source RPMs:

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os=RHEL7.3 lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`
[ -n "$kernelversion" ] && $SUDOCMD yum -y install kernel-devel-$kernelversion || echo "ERROR: kernel version not found."

#For patchless LDISKFS support:
sudo yum install --disablerepo=* --enablerepo=base-debuginfo kernel-debuginfo-common-x86_64-$kernelversion
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

##### SLES 12 SP2 Kernel Development Packages

For SLES 12 SP2, use `zypper` to install the set of kernel development packages required by Lustre. For example:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD zypper install \
kernel-default-devel=4.4.59-92.17 \
kernel-devel=4.4.59-92.17 \
kernel-syms=4.4.59-92.17 \
kernel-source=4.4.59-92.17 
```

Similarly, the following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then install the kernel development packages for SLES:

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os="SLES12 SP2" lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`

[ -n "$kernelversion" ] && $SUDOCMD zypper install \
kernel-default-devel=$kernelversion \
kernel-devel=$kernelversion \
kernel-syms=$kernelversion \
kernel-source=$kernelversion || echo "ERROR: kernel version not found."
```

**Note:** To compile Lustre, SLES 12 SP2 development environments require the `kernel-syms` package as well as `kernel-default-devel`, `kernel-devel`, and `kernel-source`. `zypper` may also incorporate other packages as dependencies.

#### Install ZFS Development Packages (for ZFS server builds)

Install the SPL and ZFS development packages, if required.

##### RHEL / CentOS 7

There are two options available:

- Use the packages from the ZFS on Linux project
- Install the packages created from source, as documented in the section on [compiling ZFS](http://wiki.lustre.org/Compiling_Lustre#ZFS).

To use the binary package distribution maintained by the ZFS on Linux project, follow the instructions in the [Getting Started](https://github.com/zfsonlinux/zfs/wiki/Getting-Started) guide to establish the YUM repository configuration, then run the following commands:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum-config-manager --disable zfs
$SUDOCMD yum-config-manager --enable zfs-kmod
$SUDOCMD yum install \
spl zfs \
kmod-spl kmod-spl-devel \
kmod-zfs kmod-zfs-devel \
libzfs2-devel
```

For self-compiled packages that were created using the process documented earlier in this guide, use the following command:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/zfs-spl/{spl-[0-9].*,kmod-spl-[0-9].*,kmod-spl-devel-[0-9].*}.x86_64.rpm \
$HOME/releases/zfs-spl/{zfs-[0-9].*,zfs-dracut-[0-9].*,kmod-zfs-[0-9].*,kmod-zfs-devel-[0-9].*,lib*}.x86_64.rpm
```

##### SLES 12 SP2

**Note:** The ZFS on Linux project does not appear to provide a ZFS binary distribution for SLES.

For self-compiled packages created using the process documented earlier in this guide, use the following command:

```
cd $HOME/releases/zfs-spl
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD rpm -ivh kmod-spl-* spl-*.x86_64.rpm \
kmod-zfs-[0-9].*-default-*.x86_64.rpm \
kmod-zfs-devel-[0-9].*.x86_64.rpm \
lib*.x86_64.rpm \
zfs-[0-9].*.x86_64.rpm \
zfs-dracut-[0-9].*.x86_64.rpm
```

#### Optional: Additional Drivers

If there are 3rd party InfiniBand drivers, they must also be installed.

For OFA OFED and Intel True Scale drivers:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/ofed/{compat-rdma-devel-[0-9].*,compat-rdma-[0-9].*}.x86_64.rpm
```

For Mellanox OFED drivers:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/ofed/{mlnx-ofa_kernel-[0-9].*,mlnx-ofa_kernel-devel-[0-9].*,mlnx-ofa_kernel-modules-[0-9].*}.x86_64.rpm
```

#### Create the Server RPMS

From a command line shell on the build server, go to the directory containing the cloned Lustre Git repository:

```
cd $HOME/lustre-release
```

Make sure that artefacts from any previous builds have been cleaned up, leaving a pristine build environment:

```
make distclean
```

Run the configure script:

```
./configure --enable-server \
[ --disable-ldiskfs ] \
[ --with-linux=<kernel devel or src path> ] \
[ --with-linux-obj=<kernel obj path> ] \
[ --with-o2ib=<IB driver path> ] \
[ --with-zfs=<ZFS devel path> | no ] \
[ --with-spl=<SPL devel path> ]
```

To create server packages, incorporate the `--enable-server` flag. The `--with-linux` and `--with-o2ib` flags should refer to the locations of the extracted `kernel-devel`(or `kernel-source`) and InfiniBand kernel drivers, respectively. SLES 12 builds also require the `--with-linux-obj` parameter.

The `configure` script will normally detect the ZFS development tree automatically if it is installed in the default location, but if not, use the `--with-zfs` and `--with-spl` options to identify the directories containing the respective development package directories. Lustre will automatically determine if it is compiling server packages with LDISKFS and/or ZFS support. To force Lustre to disable ZFS support, set `--with-zfs=no`. To explicitly disable LDISKFS support, use `--disable-ldiskfs`.

##### RHEL / CentOS 7 Examples

To create Lustre server packages for OFA OFED or Intel True Scale with the LDISKFS-patched kernel:

```
./configure --enable-server \
--with-linux=/usr/src/kernels/*_lustre.x86_64 \
--with-o2ib=/usr/src/compat-rdma
```

To create packages for Mellanox OFED with a patched kernel:

```
./configure --enable-server \
--with-linux=/usr/src/kernels/*_lustre.x86_64 \
--with-o2ib=/usr/src/ofa_kernel/default
```

To create Lustre packages using the standard, unpatched OS kernel version `3.10.0-514.16.1.el7.x86_64`, for ZFS server support and/or patchless LDISKFS server support:

```
./configure --enable-server \
--with-linux=/usr/src/kernels/3.10.0-514.16.1.el7.x86_64
```

##### SLES 12 SP2 Examples

To create Lustre server packages for an unpatched kernel, referencing a specific target kernel:

```
./configure --enable-server \
--with-linux=/usr/src/linux-4.4.59-92.17 \
--with-linux-obj=/usr/src/linux-4.4.59-92.17-obj/x86_64/default 
```

The above example command line references an unpatched kernel, suitable for ZFS builds and LDISKFS builds where kernel patching is not required.

#### Compile the Server Packages

To build the Lustre server packages:

```
make rpms
```

On successful completion of the build, packages will be created in the current working directory.

#### Save the Lustre Server RPMs

Copy the Lustre server RPM packages into a directory tree for later distribution:

```
mkdir -p $HOME/releases/lustre-server
mv $HOME/lustre-release/*.rpm $HOME/releases/lustre-server
```

### Lustre Client (DKMS Packages only)

The process for creating a Lustre client DKMS package is straightforward:

```
_TOPDIR=`rpm --eval %{_topdir}`
cd $HOME/lustre-release
make distclean
./configure --enable-dist --disable-server --enable-client
make dist
cp lustre-*.tar.gz $_TOPDIR/SOURCES/
rpmbuild -bs --without servers lustre-dkms.spec
rpmbuild --rebuild --without servers $_TOPDIR/SRPMS/lustre-client-dkms-*.src.rpm
mkdir -p $HOME/releases/lustre-client-dkms
mv $_TOPDIR/RPMS/*/*.rpm $HOME/releases/lustre-client-dkms
```

If the objective is to create a set of DMKS client packages, then there is no further work required.

### Lustre Client (All other Builds)

#### Install the kernel-devel Package

The Lustre client build requires an unpatched version of the `kernel-devel` package.

##### RHEL / CentOS 7

Use the following shell script fragment to identify and download the appropriate kernel version for a given operating system and Lustre version:

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os=RHEL7.3 lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`
[ -n "$kernelversion" ] && $SUDOCMD yum -y install kernel-devel-$kernelversion || echo "ERROR: kernel version not found."
```

Set the `os` and `lu` variables at the beginning of the script to the required operating system release and Lustre version respectively.

##### SLES 12 SP2

For SLES 12 SP2, use `zypper` to install the set of kernel development packages required by Lustre. The following shell script fragment can be used to identify the kernel version for a given operating system and Lustre version, and then install the kernel development packages for SLES:

```
SUDOCMD=`which sudo 2>/dev/null`
kernelversion=`os="SLES12 SP2" lu=2.10.0 \
awk '$0 ~ "* version "ENVIRON["lu"]{i=1; next} \
$0 ~ "* Server known" && i {j=1; next} \
(/\*/ && j) || (/\* version/ && i) {exit} \
i && j && $0 ~ ENVIRON["os"]{print $1}' $HOME/lustre-release/lustre/ChangeLog`

[ -n "$kernelversion" ] && $SUDOCMD zypper install \
kernel-default-devel=$kernelversion \
kernel-devel=$kernelversion \
kernel-syms=$kernelversion \
kernel-source=$kernelversion || echo "ERROR: kernel version not found."
```

**Note:** To compile Lustre, SLES 12 SP2 development environments require the `kernel-syms` package as well as `kernel-default-devel`, `kernel-devel`, and `kernel-source`.

#### Optional: Additional Drivers

If there are 3rd party InfiniBand drivers, they must also be installed. The following examples assume that the drivers have been compiled from source code against an unpatched `kernel-devel` RPM, using the process described earlier. Be careful to distinguish between driver packages created for LDISKFS patched kernels, and drivers compiled against the standard unpatched kernel.

For OFA OFED and Intel True Scale drivers:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/ofed/{compat-rdma-devel-[0-9].*,compat-rdma-[0-9].*}.x86_64.rpm
```

For Mellanox OFED drivers:

```
SUDOCMD=`which sudo 2>/dev/null`
$SUDOCMD yum localinstall \
$HOME/releases/ofed/{mlnx-ofa_kernel-[0-9].*,mlnx-ofa_kernel-devel-[0-9].*,mlnx-ofa_kernel-modules-[0-9].*}.x86_64.rpm
```

#### Create the Client RPMs

From a command line shell on the build host, go to the directory containing the cloned Lustre Git repository:

```
cd $HOME/lustre-release
```

Make sure that artefacts from any previous builds have been cleaned up, leaving a pristine build environment:

```
make distclean
```

Run the configure script. To create client packages, incorporate the `--disable-server</code< and <code>--enable-client` flags:

```
./configure --disable-server --enable-client \
[ --with-linux=<kernel devel path> ] \
[ --with-linux-obj=<kernel obj path> ] \
[ --with-o2ib=<IB driver path> ]
```

The `--with-linux` and `--with-o2ib` flags should refer to the locations of the extracted kernel-devel and InfiniBand kernel drivers, respectively.

For example, to create Lustre client packages for OFA OFED or Intel True Scale:

```
./configure --disable-server --enable-client \
--with-linux=/usr/src/kernels/*.x86_64 \
--with-o2ib=/usr/src/compat-rdma
```

To create Lustre client packages for Mellanox OFED:

```
./configure --disable-server --enable-client \
--with-linux=/usr/src/kernels/*.x86_64 \
--with-o2ib=/usr/src/ofa_kernel/default
```

To create Lustre client packages using the standard, unpatched OS kernel version `3.10.0-514.16.1.el7.x86_64`:

```
./configure --disable-server --enable-client \
--with-linux=/usr/src/kernels/3.10.0-514.16.1.el7.x86_64
```

Build the Lustre client packages:

```
make rpms
```

On successful completion of the build, packages will be created in the current working directory.

#### Save the Lustre Client RPMs

Copy the Lustre client RPM packages into a directory tree for later distribution:

```
mkdir -p $HOME/releases/lustre-client
mv $HOME/lustre-release/*.rpm $HOME/releases/lustre-client
```



来源： <http://wiki.lustre.org/Compiling_Lustre>