



## 标准IO和系统调用IO的区别

- 标准IO通过系统调用IO实现，标准IO可以扩平台，一般优先使用标准IO

![1614129489800](C:\Users\yangfeilong\yangfeiffei.github.io\lib\staticfile\linux-sys\1614129489800.png)

标准IO和系统调用IO的区别如下表：

|                 | 标准IO                 | 系统调用IO         |
| --------------- | ---------------------- | ------------------ |
| 帮助（man手册） | man 3                  | man 2              |
| 文件描述符      | FILE 类型的流          | int 类型文件描述符 |
| 缓存            | 行缓存、全缓存、不缓存 | 不带缓存           |



## 标准IO常用的函数和注意项

有些问题：

1. 从流中取任意数据，如何实现？标准函数貌似没办法实现？





常规知识：

- 输入输出

  ```
  #include <stdio.h>
  
         int printf(const char *format, ...);
         int fprintf(FILE *stream, const char *format, ...); //打印到FILE，如stderr
         int sprintf(char *str, const char *format, ...); // 打印到字符串
         int snprintf(char *str, size_t size, const char *format, ...); 
  ```

  来一个fprintf的小例子，[sprintf.c](/lib/staticfile/linux-sys/sprintf.c)

  ```
   #include <stdio.h>
  
         int scanf(const char *format, ...); // 慎重使用%s
         int fscanf(FILE *stream, const char *format, ...);
         int sscanf(const char *str, const char *format, ...);
  
  ```

  

- 文件位置定位

  ```
  #include <stdio.h>
  
         int fseek(FILE *stream, long offset, int whence);  //long  
  
         long ftell(FILE *stream);  // long 没有说明，大小不好说，2G？
  
         void rewind(FILE *stream);
  
         int fgetpos(FILE *stream, fpos_t *pos);
         int fsetpos(FILE *stream, fpos_t *pos);
  
  ```

  有两个函数解决这个问题，但是方言...

  ```
  #include <stdio.h>
  
         int fseeko(FILE *stream, off_t offset, int whence);  //不用long，使用off_t
         off_t ftello(FILE *stream);
         
  On many architectures both off_t and long are 32-bit types, but compilation with               #define _FILE_OFFSET_BITS 64
  will turn off_t into a 64-bit type.
  #需要添加 CFLAGS+=-D_FILE_OFFSET_BITS=64 到makefile
  ```

  那如果文件真的超过2G，怎么办？不知道

  如何获取文件的长度？[fseek.c](/lib/staticfile/linux-sys/fseek.c)

  

  





















































