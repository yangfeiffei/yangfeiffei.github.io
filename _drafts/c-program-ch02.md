---
layout: post
title: C语言学习笔记-02运算和数据类型
date: 2018-04-22 12:00
author: felo
tags: c
typora-root-url: ../../yangfeiffei.github.io
---



- 运算：就是加减乘除，当然还有别的；
- 数据类型：整数，浮点数，字符串等等；

# 1.运算

直接来一段代码吧([list0201.c](/scripts/c-program/list0201.c))：

```c
#include <stdio.h>
int main(void)
{
        int x,y;
        puts("input two ints:");
        printf("x:"); scanf("%d", &x);
        printf("y:"); scanf("%d", &y);

        printf("x + y = %d\n", x + y);
        printf("x - y = %d\n", x - y);
        printf("x * y = %d\n", x * y);
        printf("x / y = %d\n", x / y);
        printf("x %% y = %d\n", x % y);

        return (0);
}
```

执行一下：

```bash
[root@localhost c]# gcc list0201.c && ./a.out
input two ints:
x:53
y:21
x + y = 74
x - y = 32
x * y = 1113
x / y = 2
x % y = 11
```

加减乘除里面的加减乘没什么说的，都是正常的用法。这里求商的运算是`/`， `%`指的是求余数，而且这里求解出来的都是整数。其中：

```c
printf("x %% y = %d\n", x % y);
```

有两个`%%`，这里就能输出一个%，而不会转换。

| 运算符                  | 表示 | 备注               |
| ----------------------- | ---- | ------------------ |
| * （binary * operator） | a*b  | 求a和b的积         |
| / （/ operator）        | a/b  | 求a和b的商，取整   |
| %  (% operator)         | a%b  | 求a和b的商后的余数 |
| +  (binary + operator)  | a+b  | 求a和b的和         |
| -  (binary - operator)  | a-b  | 求a和b的差         |
| +  (unary + operator)   | +a   | a的值              |
| -  (unary - operator)   | -a   | a的相反数          |

>  其中有一个注意的地方：
>
> - 正数%正数=正数，这个没问题；
> - 但是%计算如果涉及到负数，根据不同的编译器可能会得到不同的结果
>
> 因此，尽量避免这样的运算。

再来一个例子（[list0202.c](/scripts/c-program/list0202.c)）：

```c
#include <stdio.h>
int main(void)
{
        int x,y;
        puts("input two ints:");
        printf("x:"); scanf("%d", &x);
        printf("y:"); scanf("%d", &y);

        printf("x 除以 y 等于 %d 余 %d\n", x / y, x % y);

        return (0);
}
```

这是个简单求余方式，其中有一句：

```c
printf("x 除以 y 等于 %d 余 %d\n", x / y, x % y);
```

`printf`可以同时转换两个变量，只需要在第二个位置依次填上需要转换的变量。注意前面有两个空，后面就必须是两个变量，一定要对应好。

几个悲催的概念：

- 赋值：就是`=`，准确的说是simple assignment operator；
- 表达式：`a`，`b`，`a+b`，`a+12`等都是表达式；
- 赋值表达式：如`a=b+2`；
- 表达式语句：一行代码，表达式+分号称为表达式语句；
- 操作数：如a+b，a和b就是操作数，a是第一操作数，b是第二操作数；
- 单目运算（unary）：只有一个操作数，如-a；
- 双目运算（binary）：有两个操作数，如a+b；



# 2. 数据类型

这里先只说两种，其他后面详细学习：

- int：整数类型，只可以赋值整数，即使赋值实数，也只会保留整数；
- double：浮点数类型，也就是带小数点的数。

我们在声明一个变量的时候，需要定义它的数据类型，也就是前面说到的，假设一个变量是一个“盒子”，那么声明一个变量就是定义这个盒子的大小和形状，数据类型就是盒子的大小和形状。

比如说，一个变量被声明为int，那么它的取值范围为-32767到32767，超过这个范围就会出错。



—— page 26

