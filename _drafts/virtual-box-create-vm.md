

# 创建一个virtualbox虚拟机

```bash

#!/bin/bash
export vmName=$1

cd /Users/yangfeilong/VirtualBox\ VMs
echo "Creating a vm ..."
VBoxManage createvm  --name $vmName --ostype Linux26_64 --register
VBoxManage modifyvm $vmName --memory 1024
VBoxManage modifyvm $vmName --cpus 1
VBoxManage modifyvm $vmName --boot1 disk
VBoxManage modifyvm $vmName --nic1 bridged
VBoxManage modifyvm $vmName --bridgeadapter1 en1
VBoxManage createhd --filename $vmName.vdi --size 8192
VBoxManage storagectl $vmName --name "IDE" --add ide --controller PIIX4
VBoxManage storagectl $vmName --name "SATA" --add sata --controller IntelAHCI

VBoxManage storageattach $vmName --storagectl SATA --port 0 --device 0 --type hdd --medium c04.vdi
VBoxManage storageattach $vmName --storagectl IDE --port 0 --device 0 --type dvddrive \
--medium ~/Documents/Media/linux/CentOS-6.5-x86_64-minimal.iso

echo "Create vm:$vmName completed."
# echo "Starting vm:$vmName"
# VBoxManage startvm $vmName

```


# hdiutil makehybrid 创建iso文件

localhost:Desktop$ hdiutil makehybrid -o ~/Documents/Media/linux/c65.iso -iso c6.5/

启动不了，不能够自动识别repo


# 自动创建虚拟机脚本

```bash
#!/bin/bash

vmName=$1
vmPATH="/Users/yangfeilong/VirtualBoxVMs"

CreateVM(){
echo "Creating a vm ..."
VBoxManage createvm  --name $vmName --ostype Linux26_64 --register
VBoxManage modifyvm $vmName --memory 1024
VBoxManage modifyvm $vmName --cpus 1
VBoxManage modifyvm $vmName --boot1 disk
VBoxManage modifyvm $vmName --nic1 bridged
VBoxManage modifyvm $vmName --bridgeadapter1 en1
VBoxManage createhd --filename $vmPATH/$vmName/$vmName.vdi --size 8192
VBoxManage storagectl $vmName --name "IDE" --add ide --controller PIIX4
VBoxManage storagectl $vmName --name "SATA" --add sata --controller IntelAHCI

VBoxManage storageattach $vmName --storagectl SATA --port 0 --device 0 --type hdd --medium $vmPATH/$vmName/$vmName.vdi
VBoxManage storageattach $vmName --storagectl IDE --port 0 --device 0 --type dvddrive  --medium ~/Documents/Media/linux/Cent65_yfl.iso

echo "Create vm:$vmName completed."
}

StartVM(){
		echo "Starting vm:$vmName"
		VBoxManage startvm $vmName

}


if [ -z "$vmName" -o "$vmName" = "(none)" ] ; then
		
		echo "Usage:"
		echo "createVirtualboxVM [vm_name]"	
		exit 0
		
fi

CreateVM


if [ "$2" = "-start" ] ; then

		StartVM
fi

```