

## live linux Install ntfsprogs

```bash
# for RHEL 6
yum install ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/Virtualization:/Appliances/RedHat_RHEL-6/x86_64/libx86emu1-1.5-1.1.x86_64.rpm

yum install ftp://ftp.pbone.net/mirror/ftp.pramberger.at/systems/linux/contrib/rhel6/x86_64/libhd-20.1-1.el6.pp.x86_64.rpm

yum install ftp://ftp.pbone.net/mirror/ftp.pramberger.at/systems/linux/contrib/rhel6/x86_64/ntfs-3g-libs-2017.3.23-1.el6.pp.x86_64.rpm

yum install ftp://ftp.icm.edu.pl/vol/rzm8/linux-fedora/linux/epel/6/x86_64/Packages/n/ntfsprogs-2017.3.23-1.el6.x86_64.rpm


# for rhel 7 
ntfs-3g-2017.3.23-1.el7.x86_64.rpm  
ntfsprogs-2017.3.23-1.el7.x86_64.rpm
```



Here's how we transfer or clone Windows from one hard disk to another. This process should work for transferring any NTFS based filesystem, bootable or not.

## Preparation

The following equipment and tools are necessary to perform this task.

- Existing Windows System/NTFS File System
- Intermediate Storage Location
- New Hard Disk, same or larger size
- A copy of [SystemRescueCd](http://www.sysresccd.org/) or other suitable Live Linux

## Prepare the Windows System

Not much here, make sure the machine is stable and does have any updates pending. Run a Windows disk check (`chkdsk /f`) and reboot the system twice. When the boot process is verified clean reboot and start up from the SystemRescueCd.

## Backup the Partition Information

Using `sfdisk` we can dump to a reasonable format that we can feed back to sfdisk when restoring.

```
# sfdisk -d /dev/sda > /mnt/storage/sda.ptab
```

## Backup the BootSector/MBR

Some guides show only taking the first 512 bytes ([MBR](http://en.wikipedia.org/wiki/Master_boot_record)), others show the first 63 blocks (MBR + Bootloader).

```
# dd if=/dev/sda of=/mnt/storage/sda.mbr bs=512 count=1
# dd if=/dev/sda of=/mnt/storage/sda.vbr bs=512 count=63
```

## Create FileSystem Copy

SystemRescueCd should boot directly to a shell, we will dump the file system and power off.

There are many ways to store the file system image, not covered here. The `ntfsclone` tool does not care. We have sucessfully created images on external USB drives (ext3), over Samba shares (to NTFS & Samba 3), GlusterFS (reiser,ext4) and others. Just ensure capacity is available and the FS supports large files.

In the example the Windows system is installed to */dev/sda1*. The image storage location is mounted, through whatever means necessary, at */mnt/storage*. Adjust parameters as necessary for the environment.

Examine the disk before starting, take note of the *Space in use* value. The image created by `ntfsclone` will be slightly larger ( ~2%).

```
# ntfsresize --info /dev/sda1
ntfsresize v2.0.0 (libntfs 10:0:0)
Device name        : /dev/sda1
NTFS volume version: 3.1
Cluster size       : 4096 bytes
Current volume size: 25802666496 bytes (25803 MB)
Current device size: 25802671104 bytes (25803 MB)
Checking filesystem consistency ...
100.00 percent completed
Accounting clusters ...
Space in use       : 10949 MB (42.4%)
Collecting resizing constraints ...
You might resize at 10948591616 bytes or 10949 MB (freeing 14854 MB).
Please make a test run using both the -n and -s options before real resizing!
```

If `ntfsresize` complains follow it's request to run `chkdsk /f` and try again. The clone should only be created from a dirty file system in emergencies.

```
# ntfsclone --save-image --output /mnt/storage/windows.ntfsclone /dev/sda1
ntfsclone v2.0.0 (libntfs 10:0:0)
NTFS volume version: 3.1
Cluster size       : 4096 bytes
Current volume size: 25802666496 bytes (25803 MB)
Current device size: 25802671104 bytes (25803 MB)
Scanning volume ...
100.00 percent completed
Accounting clusters ...
Space in use       : 10949 MB (42.4)
Saving NTFS to image ...
100.00 percent completed
```

If the disk is in really bad shape the following incantations may help:

```
ntfsclone --save-image --rescue --output /mnt/storage/windows.ntfsclone /dev/sda1
ntfsclone --save-image --ignore-fs-check --rescue --output /mnt/storage/windows.ntfsclone /dev/sda1
ntfsclone --save-image --force --ignore-fs-check --rescue --output \
  /mnt/storage/windows.ntfsclone /dev/sda1
```

After creating the clone, poweroff the system and replace the current HDD with the new disk.

## Restoring Windows Image

Use cfdisk to partition, use ntfsclone to restore the image, then ntfsresize to resize to larger volume.

Many tools can partition, such as `fdisk` or `cfdisk` or which ever. Using any of these tools create a partition on the new disk similar to the original disk. In this example the original disk was replaced with one twice the size.

Cfdisk was used to create a partition (/dev/sda1) that was 50GiB in size and then the type was set to 0x07 (NTFS) and flagged as bootable. Do not format the partition with a file-system, leave it empty.

```
# ntfsclone --restore-image --overwrite /dev/sda1 /mnt/storage/windows.ntfsclone
ntfsclone v2.0.0 (libntfs 10:0:0)
Ntfsclone image version: 10.0
Cluster size           : 4096 bytes
Image volume size      : 25802666496 bytes (25803 MB)
Image device size      : 25802671104 bytes (25803 MB)
Space in use           : 10949 MB (42.4%)
Offset to image data   : 56 (0x38) bytes
Restoring NTFS from image ...
100.00 percent completed
```

Now reboot the system, Windows will run a file-system check and all should be well. It may be a good idea to start in Safe Mode (press F8 during startup)

We have successfully use this process to archive an 80GiB /dev/sda2 Windows (/dev/sda1 was recovery partition from Dell) to /dev/sda1 on a new disk (120GiB).

Moving NTFS volumes around is a dangerous game, care should be taken. Ensure backups are available, cross fingers, etc.

## ntfsclone direct disk to disk [§](http://edoceo.com/exemplar/ntfsclone-transfer-windows#disk2disk)

It's also possible, and very useful, to directly clone one disk to another, i.e. replacing a failing disk. It an ideal world this new disk would be identical, to minimize the possibility of Windows having a fit. But even if not identical, we can make it happen.

### Clone Identical Disks

In this example /dev/sdb is the old disk, /dev/sda is the new one, I've booted from a CD. We create the target partition, run clone|clone

If they disk are identical then manually create a similar partition table and just use dd

```
sfdisk -d /dev/sdb | sfdisk --force /dev/sda
sfdisk --re-read /dev/sda
dd if=/dev/sdb1 of=/dev/sda1
```

### Clone to Smaller Disk

If not identical then ntfsclone comes in, this allows us to copy differently sized NTFS partitions. Here, /dev/sdb3 is the source partition (160G) from our old dual-boot disk and /dev/sda3 is the smaller sized target (128G). First, shrink the old NTFS to less than the limit, clone, then resize new NTFS to fill the partition.

```
ntfsresize --size 120G /dev/sdb3
ntfsclone \
    --force \
    --overwrite /dev/sda3 \
    /dev/sdb3
# Boot the Window Here and chkdsk
ntfsresize --size MAX /dev/sda3
```

Note that it's not wise to try to move the partitions around on Windows. That is, don't clone from /dev/sdb3 to /dev/sda2 or other such, Windows is very sensitive to those changes and will likely blue-screen on you. It's a STOP 0x0000007B, inaccessible boot device.