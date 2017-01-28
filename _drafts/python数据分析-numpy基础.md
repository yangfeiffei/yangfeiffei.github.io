Numpy（Numerical Python的简称）高性能科学计算和数据分析的基础包。其部分功能如下：
- ndarray，具有矢量算术运算和复杂广播能力的快速且节省空间的多维数组。
- 数组运算，不用编写循环
- 可以读写磁盘数据，操作内存映射
- 线性代数
- 集成c，c++等语言

python能够包装c、c++以numpy数组形式的数据。pandas提供了结构化或表格化数据的处理高级接口，
还提供了numpy不具备的时间序列处理等；

# 1.ndarray：多维数组对象
多维数组，要求所有元素的类型一致，通常说的“数组”、“Numpy数组”、“ndarray”都是指“ndarray”对象。
## 1.1 创建ndarray
函数 | 说明
- | -
array | 输入数据转换为ndarray对象，可以是python元组、列表或其他序列类型。可以自动识别dtype，或者手动指定类型
asarray | 将输入数据转换为ndarray对象
arange | 类似range，返回ndarray的一维序列数组
ones，ones_like | 创建全1数组，默认float类型。ones_like创建一个类型输入数组的全1数组
zeros，zeros_like | 与ones相同，创建全0数组
empty，empty_like | 全空数组，只分配内存空间，不填充任何值
eye、identity | 创建一个n*n的单位矩阵（阵列）


```python
In [1]: import numpy as np
In [2]: np.arange(10)
Out[2]: array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

In [3]: np.array([1,2,3,5,6,7])
Out[3]: array([1, 2, 3, 5, 6, 7])

In [4]: np.ones((3,1))
Out[4]:
array([[ 1.],
       [ 1.],
       [ 1.]])

In [5]: np.zeros((2,5))
Out[5]:
array([[ 0.,  0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.,  0.]])

In [6]: np.eye(3)
Out[6]:
array([[ 1.,  0.,  0.],
       [ 0.,  1.,  0.],
       [ 0.,  0.,  1.]])

In [7]: np.empty((2,4))
Out[7]:
array([[  0.00000000e+000,   0.00000000e+000,   2.12267575e-314,
          2.19986168e-314],
       [  2.15551710e-314,   2.19976181e-314,   2.31584192e+077,
          5.56268597e-309]])
```

## 1.2 ndarray数据类型

![image](../_data/images/python数据分析/dtype.png)


![image](../_data/images/python数据分析/dtype1.png)



# 2.通用函数

# 3.数组处理数据

# 4.文件处理

# 5.线性代数

# 6.随机数

# 7.范例：随机漫步
