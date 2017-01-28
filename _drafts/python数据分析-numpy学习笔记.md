> 几乎全部引自：http://www.cnblogs.com/xinchrome/p/5043480.html

# 一、数组方法

## 1.1创建数组：
arange()创建一维数组；array()创建一维或多维数组，其参数是类似于数组的对象，如列表等

```python
import numpy as np
# arange
np.arange(10)
Out[54]: array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

np.arange(0,100,2)
Out[55]:
array([ 0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
       34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66,
       68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98])
# array
np.array([[1,2,3],[4,5,6]])
Out[56]:
array([[1, 2, 3],
       [4, 5, 6]])

np.array([[1,2,3],[4,5,6]],dtype="f8")
Out[57]:
array([[ 1.,  2.,  3.],
       [ 4.,  5.,  6.]])
np.array([[1,2,3],[4,5,6]],dtype="f4")
Out[60]:
array([[ 1.,  2.,  3.],
       [ 4.,  5.,  6.]], dtype=float32)

# 通过list转换
list1 = [1,2,3,4,5,6]
l = np.arange(list1) #失败
l = np.array(list1)
l
Out[64]: array([1, 2, 3, 4, 5, 6])
```

## 1.2反过来转换
则可以使用numpy.ndarray.tolist()函数，如a.tolist()
```python
l
Out[65]: array([1, 2, 3, 4, 5, 6])

np.ndarray.tolist(l)
Out[66]: [1, 2, 3, 4, 5, 6]

l.tolist()
Out[68]: [1, 2, 3, 4, 5, 6]
```

## 1.3创建数组
np.zeros((2,3))，或者np.ones((2,3))，参数是一个元组分别表示行数和列数
```python
np.zeros((2,3))
Out[72]:
array([[ 0.,  0.,  0.],
       [ 0.,  0.,  0.]])

np.zeros_like(np.arange(0,10,2))
Out[73]: array([0, 0, 0, 0, 0])
```
```python
np.ones((3,4))
Out[74]:
array([[ 1.,  1.,  1.,  1.],
       [ 1.,  1.,  1.,  1.],
       [ 1.,  1.,  1.,  1.]])

np.ones_like((2,2))
Out[75]: array([1, 1])

np.ones_like(np.arange(10).reshape(2,5))
Out[76]:
array([[1, 1, 1, 1, 1],
       [1, 1, 1, 1, 1]])
```
还有eye，
```python
np.eye(5) # 创建单位矩阵E，默认浮点类型
Out[77]:
array([[ 1.,  0.,  0.,  0.,  0.],
       [ 0.,  1.,  0.,  0.,  0.],
       [ 0.,  0.,  1.,  0.,  0.],
       [ 0.,  0.,  0.,  1.,  0.],
       [ 0.,  0.,  0.,  0.,  1.]])
```

## 1.4 阵列四则运算
乘法：对应元素相乘，a * b，得到一个新的阵列ndarray对象，形状要一致；但是允许a是向量而b是阵列，a的列数必须等于b的列数，a与每个行向量对应元素相乘得到行向量。
注意与矩阵乘法的区别
```python
a
Out[103]:
array([[0, 1, 2, 3, 4],
       [5, 6, 7, 8, 9]])

b
Out[105]:
array([[ 0,  2,  4,  6,  8],
       [10, 12, 14, 16, 18]])

a * b
Out[106]:
array([[  0,   2,   8,  18,  32],
       [ 50,  72,  98, 128, 162]])

a + b
Out[107]:
array([[ 0,  3,  6,  9, 12],
       [15, 18, 21, 24, 27]])

a - b
Out[108]:
array([[ 0, -1, -2, -3, -4],
       [-5, -6, -7, -8, -9]])
from __future__ import division
a / b
Out[109]:
array([[ nan,  0.5,  0.5,  0.5,  0.5],
       [ 0.5,  0.5,  0.5,  0.5,  0.5]])


```

数学上定义的矩阵乘法 np.dot(a, b)。如果形状不匹配会报错；但是允许允许a和b都是向量，返回两个向量的内积。只要有一个参数不是向量，就应用矩阵乘法。
```python
np.dot(a,b)
Traceback (most recent call last):

  File "<ipython-input-89-71281829ef00>", line 1, in <module>
    np.dot(a,b)

ValueError: shapes (2,5) and (2,5) not aligned: 5 (dim 1) != 2 (dim 0)


np.dot(a,b.T)  # b.T表示b转置矩阵
Out[90]:
matrix([[ 60, 160],
        [160, 510]])
```
（PS：总之就是，向量很特殊，在运算中可以自由转置而不会出错，运算的返回值如果维度为1，也一律用行向量[]表示）

## 1.5 读取数据元素

读取数组元素：如a[0],a[0,0]
```python
a
Out[110]:
array([[0, 1, 2, 3, 4],
       [5, 6, 7, 8, 9]])
a[0]
Out[112]: array([0, 1, 2, 3, 4])

a[0,0]
Out[113]: 0


```
```python
d = np.arange(20).reshape(4,5)  #数组变形

d
Out[120]:
array([[ 0,  1,  2,  3,  4],
       [ 5,  6,  7,  8,  9],
       [10, 11, 12, 13, 14],
       [15, 16, 17, 18, 19]])

d[0:3:2,0:2]  # 行0-3，相隔2；列0-2
Out[121]:
array([[ 0,  1],
       [10, 11]])
```
## 1.6 数组变形
如b=a.reshape(2,3,4)将得到原数组变为2*3*4的三维数组后的数组；或是a.shape=(2,3,4)或a.resize(2,3,4)直接改变数组a的形状

数组组合：水平组合hstack((a,b))或concatenate（（a,b）,axis=1）;垂直组合vstack((a,b))或concatenate（（a,b）,axis=0）;深度组合dstack((a,b))

数组分割（与数组组合相反）：分别有hsplit,vsplit,dsplit,split(split与concatenate相对应)

将np数组变为py列表：a.tolist()

数组排序（小到大）：列排列np.msort(a)，行排列np.sort(a)，np.argsort(a)排序后返回下标

复数排序：np.sort_complex(a)按先实部后虚部排序

数组的插入：np.searchsorted(a,b)将b插入原有序数组a，并返回插入元素的索引值

类型转换：如a.astype(int)，np的数据类型比py丰富，且每种类型都有转换方法

条件查找，返回满足条件的数组元素的索引值：np.where(条件)

条件查找，返回下标：np.argwhere(条件)

条件查找，返回满足条件的数组元素：np.extract([条件],a)

根据b中元素作为索引，查找a中对应元素：np.take(a,b)一维

数组中最小最大元素的索引：np.argmin(a)，np.argmax(a)

多个数组的对应位置上元素大小的比较：np.maximum(a,b,c,…..)返回每个索引位置上的最大值，np.minimum(…….)相反

将a中元素都置为b：a.fill(b)

每个数组元素的指数：np.exp(a)

生成等差行向量：如np.linspace(1,6,10)则得到1到6之间的均匀分布，总共返回10个数

求余：np.mod(a,n)相当于a%n，np.fmod(a,n)仍为求余且余数的正负由a决定

计算平均值：np.mean(a)



计算最大值：amax(a, axis=None, out=None, keepdims=False) 。Return the maximum of an array or maximum along an axis.



计算加权平均值：np.average(a,b),其中b是权重

计算数组的极差：np.pth(a)=max(a)-min(a)

计算方差（总体方差）：np.var(a)

标准差：np.std(a)

算术平方根，a为浮点数类型：np.sqrt(a)

对数：np.log(a)

修剪数组，将数组中小于x的数均换为x，大于y的数均换为y：a.clip(x,y)

所有数组元素乘积：a.prod()

数组元素的累积乘积：a.cumprod()

数组元素的符号：np.sign(a)，返回数组中各元素的正负符号，用1和-1表示

数组元素分类：np.piecewise(a,[条件]，[返回值])，分段给定取值，根据判断条件给元素分类，并返回设定的返回值。

判断两数组是否相等： np.array_equal(a,b)

判断数组元素是否为实数： np.isreal(a)

去除数组中首尾为0的元素：np.trim_zeros(a)

对浮点数取整，但不改变浮点数类型：np.rint(a)



二、数组属性

1.获取数组每一维度的大小：a.shape

2.获取数组维度：a.ndim

3.元素个数：a.size

4.数组元素在内存中的字节数：a.itemsize

5.数组字节数：a.nbytes==a.size*a.itemsize

6.数组元素覆盖：a.flat=1，则a中数组元素都被1覆盖

7.数组转置：a.T

不能求逆、求协方差、迹等，不适用于复杂科学计算，可以将array转换成matrix。



三、矩阵方法

创建矩阵：np.mat(‘…’)通过字符串格式创建，np.mat(a)通过array数组创建，也可用matrix或bmat函数创建

matrix不会自动转换行列向量。matrix的所有运算默认都是数学上定义的矩阵运算，除非用mutiply函数实现点乘。

创建复合矩阵：np.bmat(‘A B’,’AB’)，用A和B创建复合矩阵AB（字符串格式）

创建n*n维单位矩阵：np.eye(n)

矩阵的转置：A.T

矩阵的逆矩阵：A.I

计算协方差矩阵：np.cov(x)，np.cov(x,y)

计算矩阵的迹（对角线元素和）：a.trace()

相关系数：np.corrcoef(x,y)

给出对角线元素：a.diagonal()



四、线性代数

估计线性模型中的系数：a=np.linalg.lstsq(x,b),有b=a*x

求方阵的逆矩阵：np.linalg.inv(A)

求广义逆矩阵：np.linalg.pinv(A)

求矩阵的行列式：np.linalg.det(A)

解形如AX=b的线性方程组：np.linalg.solve(A,b)

求矩阵的特征值：np.linalg.eigvals（A）

求特征值和特征向量：np.linalg.eig(A)

Svd分解：np.linalg.svd(A)



五、概率分布

产生二项分布的随机数：np.random.binomial(n,p,size=…)，其中n,p,size分别是每轮试验次数、概率、轮数

产生超几何分布随机数：np.random.hypergeometric(n1,n2,n,size=…)，其中参数意义分别是物件1总量、物件2总量、每次采样数、试验次数

产生N个正态分布的随机数：np.random.normal(均值，标准差，N)

产生N个对数正态分布的随机数：np.random.lognormal(mean,sigma,N)



六、多项式

多项式拟合：poly= np.polyfit(x,a,n),拟合点集a得到n级多项式，其中x为横轴长度，返回多项式的系数

多项式求导函数：np.polyder(poly),返回导函数的系数

得到多项式的n阶导函数：多项式.deriv(m = n)

多项式求根：np.roots(poly)

多项式在某点上的值：np.polyval(poly,x[n]),返回poly多项式在横轴点上x[n]上的值

两个多项式做差运算： np.polysub(a,b)



Matpoltlib简单绘图方法

引入简单绘图的包import matplotlib.pyplot as plt，最后用plt.show()显示图像

基本画图方法：plt.plot(x,y)，plt.xlabel(‘x’)，plt.ylabel(‘y’)，plt.title(‘…’)

子图：plt.subplot(abc)，其中abc分别表示子图行数、列数、序号

创建绘图组件的顶层容器：fig = plt.figure()

添加子图：ax = fig.add_subplot(abc)

设置横轴上的主定位器：ax.xaxis.set_major_locator(…)

绘制方图：plt.hist(a,b)，a为长方形的左横坐标值，b为柱高

绘制散点图：plt.scatter(x,y,c = ‘..’,s = ..)，c表示颜色，s表示大小

添加网格线：plt.grid(True)

添加注释：如ax.annotate('x', xy=xpoint, textcoords='offsetpoints',xytext=(-50, 30), arrowprops=dict(arrowstyle="->"))



增加图例：如plt.legend(loc='best', fancybox=True)

对坐标取对数：横坐标plt.semilogx()，纵坐标plt.semilogy()，横纵同时plt.loglog()
