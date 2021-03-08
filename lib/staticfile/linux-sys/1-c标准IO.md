2021年2月24日 09:21:36

### 一、概述

IO:  input&output  一切实现的基础

分成两种：

- stdio：标准IO，优先使用标准IO；可移植；man手册3；
- sysio：系统调用IO，文件IO；不同操作系统实现不同；man手册2；



![1614129489800](C:\Users\yangfeilong\yangfeiffei.github.io\lib\staticfile\linux-sys\1614129489800.png)



### 二、stdio（标准IO）

有一个类型贯穿始终：FILE类型，实际上是一个typedef出来的结构体；

- fopen()：       FILE *fopen(const char *path, const char *mode);

  - const修饰，不会修改用户传递的参数

  - mode类型，是字符串，不是单个字符

    ```
           r      Open text file for reading.  The stream is positioned at the beginning of the file.
    
           r+     Open for reading and writing.  The stream is positioned at the beginning of the file.
    
           w      Truncate  file to zero length or create text file for writing.  The stream is positioned at the  beginning of the file.
    
           w+     Open for reading and writing.  The file is created if it does not exist, otherwise it is  truncated.  The stream is positioned at the beginning of the file.
    
           a      Open  for  appending  (writing at end of file).  The file is created if it does not exist.  The stream is positioned at the end of the file.
    
           a+     Open for reading and appending (writing at end of file).  The file is created if  it  does  not exist.   The  initial  file position for reading is at the beginning of the file, but output is always appended to the end of the file.
    
    ```

  
  fopen的返回的FILE结构体，在哪里？A栈  B静态区 C堆
  
  ​	栈：fopen内的一个内部局部变量；fopen出来的返回之后，栈就消失了，所以不会；
  
  ​	静态区，返回的FILE用的是static修饰的；但是这个不能被重复定义，是个问题。
  
  ​	所以，最后应该放在堆上。
  
  fopen上肯定malloc了一个FILE结构体的一块内存空间，因此在fclose是free的。
  
  *** 一般来说，有对应的逆操作的函数，会放在堆上 ***
  
  
  
  ***有宏的就用宏，没有再用-1 **

```c
$ cat max_fopen.c
#include <stdio.h>
#include <stdlib.h>

int main(void){

        int count = 0;
        FILE* fp = NULL;

        while(1){

                fp = fopen("1.txt", "r");
                if(fp == NULL){
                        perror("fopen()");
                        break;
                }

                count++;
        }
        printf("max open = %d \n", count);
        return 0;
}
```

fclose()：fclose()的返回值，一般不需要验证的。



fgetc() / getc()： getchar--->getc---->fgetc

fputc()/ putc()

fgets()/fputs() : 字符串相关，不要使用gets函数

puts是可以的，fputs也可以



fread() / fwrite() : 不错，但是有点小问题：

	-	比如某个SIZE里面有个错误，那么所有的对象都有问题；
	-	fread(buf, 10,1,fd)，不能保证最后一次是读到了10个对象啊
	-	fread(buf,1,10,fd)，这个是可以的。



printf()

scanf()



fseek()

ftell()

rewind()



fflush()





























