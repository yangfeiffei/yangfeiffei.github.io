pandas是基于numpy创建，提供更高级的数据结构和操作工具，是一个不可获取的工具。

为了更好的阐述，约定如下：
```python
import pandas as pd
from pandas import Series,DataFrame
```
# 1 pandas数据结构
两个最重要的数据结构：Series和DataFrame，基本可以解决绝大部分问题。

## 1.1 Series
类似于一维数组的对象，由一组数据和一组索引组成。Series的表现形式一般为左边索引，
右边值。
```python
In [50]: obj = Series([4,7,-5,3])

In [51]: obj
Out[51]:
0    4
1    7
2   -5
3    3
dtype: int64

In [52]: obj.values
Out[52]: array([ 4,  7, -5,  3], dtype=int64)

In [53]: obj.index
Out[53]: RangeIndex(start=0, stop=4, step=1)
```
可以自定义索引：
```python
In [54]: obj2 = Series([4,7,-5,3],index=['d','b','a','c'])

In [55]: obj2
Out[55]:
d    4
b    7
a   -5
c    3
dtype: int64

In [56]: obj2.index
Out[56]: Index(['d', 'b', 'a', 'c'], dtype='object')
```
Series类似一个有序字典，可以进行查询和赋值等。
```python
In [5]: obj2['d']
Out[5]: 4

In [6]: obj2['d'] = 100

In [7]: obj2
Out[7]:
d    100
b      7
a     -5
c      3
dtype: int64

In [8]: obj2[['d','a']]
Out[8]:
d    100
a     -5
dtype: int64
```
同样可以对Series的值进行运算。
```python
In [9]: obj2 * 2
Out[9]:
d    200
b     14
a    -10
c      6
dtype: int64

In [10]: import numpy as np

In [11]: np.exp(obj2)
Out[11]:
d    2.688117e+43
b    1.096633e+03
a    6.737947e-03
c    2.008554e+01
dtype: float64
```
和字典一样的特性
```python
In [12]: 'a' in obj2
Out[12]: True

In [13]: 6 in obj2
Out[13]: False
```
可以通过字典直接来创建Series,字典的键就是Series的索引。
```python
In [14]: sdata = {'ohio':35000, 'Text':72134,'Uthch':5000}

In [15]: obj3 = Series(sdata)

In [16]: obj3
Out[16]:
Text     72134
Uthch     5000
ohio     35000
dtype: int64

# 在sdata中找不到的键值对，将使用NaN（Not a Number）来代替。
In [17]: obj4 = Series(sdata,index=['ohio','new_index'])

In [18]: obj4
Out[18]:
ohio         35000.0
new_index        NaN
dtype: float64

```
**pandas可以使用isnull和notnull检测是否缺失数据。**
```python
In [19]: obj3.isnull()
Out[19]:
Text     False
Uthch    False
ohio     False
dtype: bool

In [20]: obj4.notnull
Out[20]:
<bound method NDFrame.notnull of ohio         35000.0
new_index        NaN
dtype: float64>

In [21]: obj4.notnull()
Out[21]:
ohio          True
new_index    False
dtype: bool
In [22]: pd.isnull(obj4)
Out[22]:
ohio         False
new_index     True
dtype: bool
```
Series在算术运算中会自动对齐索引，并进行计算。
```python
In [24]: obj3 + obj4
Out[24]:
Text             NaN  #obj3
Uthch            NaN  #obj3
new_index        NaN  #obj4
ohio         70000.0
dtype: float64

```
Series还有一个name的属性，类似数据库表名。index也有一个name的属性，类似索引名字。
```python
In [27]: obj4.name

In [28]: obj4.name = "obj444"

In [29]: obj4
Out[29]:
ohio         35000.0
new_index        NaN
Name: obj444, dtype: float64

In [30]: obj4.index.name = 'state'

In [31]: obj4
Out[31]:
state
ohio         35000.0
new_index        NaN
Name: obj444, dtype: float64
```
可以通过赋值，就地就该index：
```python
In [39]: obj1 = Series([4,7,-5,3])

In [40]: obj1.index = ['Bob',"Steve","Jeff",'Ryan']

In [41]: obj1
Out[41]:
Bob      4
Steve    7
Jeff    -5
Ryan     3
dtype: int64
```
## 1.2 DataFrame

一个表格型数据结构，包含一组有序的列。典型的创建DataFrame如下：
```python
In [42]: data = {'state':['Ohio',"Ohio",'Ohio',"Nevada",'Nevada'],}

In [43]: data = {'state':['Ohio',"Ohio",'Ohio',"Nevada",'Nevada'],
    ...:          'year':[2000,2001,2002,2001,2002],
    ...:          'pop':[1.5,1.7,3.6,2.4,2.9]}

In [44]: frame = DataFrame(data)

In [45]: frame  #自动增加索引,列顺序是随机的。
Out[45]:
   pop   state  year
0  1.5    Ohio  2000
1  1.7    Ohio  2001
2  3.6    Ohio  2002
3  2.4  Nevada  2001
4  2.9  Nevada  2002
```
可以指定列进行创建：
```python
In [46]: DataFrame(data, columns=['year','state','pop'])
Out[46]:
   year   state  pop
0  2000    Ohio  1.5
1  2001    Ohio  1.7
2  2002    Ohio  3.6
3  2001  Nevada  2.4
4  2002  Nevada  2.9
In [47]: frame2 = DataFrame(data,columns=['year','state','pop','debt'],
    ...: index=['one','two','three','four','five'])

In [48]: frame2
Out[48]:
       year   state  pop debt
one    2000    Ohio  1.5  NaN
two    2001    Ohio  1.7  NaN
three  2002    Ohio  3.6  NaN
four   2001  Nevada  2.4  NaN
five   2002  Nevada  2.9  NaN
In [49]: frame2.columns
Out[49]: Index(['year', 'state', 'pop', 'debt'], dtype='object')
# 可以通过类似字典方式获取列数据，获取一列即为一个Series.
# 注意返回的是一个name被自动设置为state，索引与DataFrame相同。
In [50]: frame2['state']
Out[50]:
one        Ohio
two        Ohio
three      Ohio
four     Nevada
five     Nevada
Name: state, dtype: object

# 也可以获取行数据，通过ix
# 获取的内容与列类似
In [52]: frame2.ix['two']
Out[52]:
year     2001
state    Ohio
pop       1.7
debt      NaN
Name: two, dtype: object

# 可以给debt列赋值
In [53]: frame2['debt'] = 16.5  #可以是标量，自动全部填充

In [54]: frame2
Out[54]:
       year   state  pop  debt
one    2000    Ohio  1.5  16.5
two    2001    Ohio  1.7  16.5
three  2002    Ohio  3.6  16.5
four   2001  Nevada  2.4  16.5
five   2002  Nevada  2.9  16.5

In [55]: frame2['debt'] = np.arange(5.)  # 赋值长度必须与debt长度一直

In [56]: frame2
Out[56]:
       year   state  pop  debt
one    2000    Ohio  1.5   0.0
two    2001    Ohio  1.7   1.0
three  2002    Ohio  3.6   2.0
four   2001  Nevada  2.4   3.0
five   2002  Nevada  2.9   4.0

## 也可以用一个Series来赋值，缺失值NaN
In [59]: frame2['debt'] = Series([-1,-2,-4],index=['one','three','five'])

In [60]: frame2
Out[60]:
       year   state  pop  debt
one    2000    Ohio  1.5  -1.0
two    2001    Ohio  1.7   NaN
three  2002    Ohio  3.6  -2.0
four   2001  Nevada  2.4   NaN
five   2002  Nevada  2.9  -4.0

# 不存在列的赋值，会创建新列
In [61]: frame2['eastern'] = frame2.state == 'Ohio'

In [62]: frame2
Out[62]:
       year   state  pop  debt eastern
one    2000    Ohio  1.5  -1.0    True
two    2001    Ohio  1.7   NaN    True
three  2002    Ohio  3.6  -2.0    True
four   2001  Nevada  2.4   NaN   False
five   2002  Nevada  2.9  -4.0   False

# del 会删除列

In [63]: del frame2['eastern']

In [64]: frame2.columns
Out[64]: Index(['year', 'state', 'pop', 'debt'], dtype='object')

```
**注意：通过索引获取的数据，均为原始数据的视图，所有修改都会反应到源DataFrame上。
需要复制使用Series的copy方法。**

可以使用嵌套字典创建DataFrame。
```python
In [68]: pop = {'Nevada':{2001:2.4,2002:2.9},
    ...: 'Ohio':{2000:1.5,2001:1.7,2002:3.6}}

In [69]: frame3 = DataFrame(pop)

In [70]: frame3
Out[70]:
      Nevada  Ohio
2000     NaN   1.5
2001     2.4   1.7
2002     2.9   3.6

# 还可以转置
In [72]: frame3.T
Out[72]:
        2000  2001  2002
Nevada   NaN   2.4   2.9
Ohio     1.5   1.7   3.6

# 排序合并索引
In [73]: DataFrame(pop,index=[2001,2002,2003])
Out[73]:
      Nevada  Ohio
2001     2.4   1.7
2002     2.9   3.6
2003     NaN   NaN

In [75]: pdata = {'Ohio':frame3['Ohio'][:-1],'Nevada':frame3['Nevada'][:2]}

In [76]: DataFrame(pdata)
Out[76]:
      Nevada  Ohio
2000     NaN   1.5
2001     2.4   1.7

```
下面是所有DataFrame能接受的数据类型：

![](../_data/images/python数据分析/data_to_dataframe.PNG)


```python
In [78]: DataFrame(np.arange(10).reshape(2,5))
Out[78]:
   0  1  2  3  4
0  0  1  2  3  4
1  5  6  7  8  9
In [80]: frame3
Out[80]:
      Nevada  Ohio
2000     NaN   1.5
2001     2.4   1.7
2002     2.9   3.6

In [81]: frame3.name = 'states';frame3.index.name = 'year'

In [82]: frame3
Out[82]:
      Nevada  Ohio
year
2000     NaN   1.5
2001     2.4   1.7
2002     2.9   3.6

In [83]: frame3.columns.name = 'state'

In [84]: frame3
Out[84]:
state  Nevada  Ohio
year
2000      NaN   1.5
2001      2.4   1.7
2002      2.9   3.6

In [86]: frame3.values
Out[86]:
array([[ nan,  1.5],
       [ 2.4,  1.7],
       [ 2.9,  3.6]])

In [87]: frame2.values
Out[87]:
array([[2000, 'Ohio', 1.5, -1.0],
       [2001, 'Ohio', 1.7, nan],
       [2002, 'Ohio', 3.6, -2.0],
       [2001, 'Nevada', 2.4, nan],
       [2002, 'Nevada', 2.9, -4.0]], dtype=object)

```

## 1.3 索引

索引对象负责管理轴标签和其他元数据，没有指定的话会自动生成。一旦生成，索引对象不能修改。
```python
In [88]: obj = Series(range(3),index=['a','b','c'])

In [89]: obj.index
Out[89]: Index(['a', 'b', 'c'], dtype='object')

In [90]: index = obj.index

In [91]: index[1:]
Out[91]: Index(['b', 'c'], dtype='object')

In [92]: index[1] = 'd'
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-92-676fdeb26a68> in <module>()
----> 1 index[1] = 'd'

C:\Users\yangfl\Anaconda3\lib\site-packages\pandas\indexes\base.py in __setitem_
_(self, key, value)
   1243
   1244     def __setitem__(self, key, value):
-> 1245         raise TypeError("Index does not support mutable operations")
   1246
   1247     def __getitem__(self, key):

TypeError: Index does not support mutable operations

```
因为不可修改，因此可以在多个数据结构中间传递index对象。

```python
In [94]: index = pd.Index(np.arange(3))

In [95]: obj2 = Series([-0.2,-1,2],index=index)

In [96]: obj2.index == index
Out[96]: array([ True,  True,  True], dtype=bool)
```

pandas中主要的index对象：


![](../_data/images/python数据分析/pandas_index.PNG)



index对象很像数组，也是一个固定的集合。
```python

In [97]: frame3
Out[97]:
state  Nevada  Ohio
year
2000      NaN   1.5
2001      2.4   1.7
2002      2.9   3.6

In [98]: 2001 in frame3.index
Out[98]: True

In [99]: 2003 in frame3.index
Out[99]: False

```
因此，索引也有自己的方法和属性：

![](../_data/images/python数据分析/pandas_index_method.PNG)


















# 2 基本功能

只是一些基本功能，更深奥的内容用到再摸索。

## 2.1 重新索引

reindex是pandas的重要方法，举个例子：
```python
In [101]: obj = Series([4,7,-5,3.4],index=['c','a','b','d'])

In [102]: obj
Out[102]:
c    4.0
a    7.0
b   -5.0
d    3.4
dtype: float64

In [103]: obj2 = obj.reindex(['a','b','c','d','e'])

In [104]: obj2
Out[104]:
a    7.0
b   -5.0
c    4.0
d    3.4
e    NaN
dtype: float64

# 缺失值可以自定义

In [105]: obj.reindex(['a','b','c','d','e'],fill_value=0)
Out[105]:
a    7.0
b   -5.0
c    4.0
d    3.4
e    0.0
dtype: float64


```




















# 3 汇总和计算描述统计



# 4 处理缺失数据


# 5 层次化索引


# 6 其他
