---

layout: post
title: python数据分析-Pandas数据规整学习笔记
date:
author: felo

---


摘要：
Pandas数据规整相关的学习笔记，记录了《利用python进行数据分析》学习过程和笔记。

**清理、转换、合并、重塑**

# 1 合并数据集

pandas有一些内置的合并数据集方法：
- pandas.merge：根据一个或多个键将多个DataFrame连接起来，类似数据库连接；
- pandas.concat：可以沿着一个轴将多个对象堆叠起来；
- 实例方法combine_first可以将重复数据编制在一起，用以填充另一个对象的缺失值。

## 1.1 数据库风格的DataFrame合并

数据库合并（merge）或连接（join）运行将一个或多个键将行链接起来。来个例子：

```python
In [1]: import pandas as pd

In [2]: from pandas import Series,DataFrame

In [3]: df1 = DataFrame({'key':list('bbacaab'),'data1':range(7)})

In [4]: df2 = DataFrame({'key':list('abd'),'data2':range(3)})

In [5]: df1
Out[5]:
   data1 key
0      0   b
1      1   b
2      2   a
3      3   c
4      4   a
5      5   a
6      6   b

In [6]: df2
Out[6]:
   data2 key
0      0   a
1      1   b
2      2   d

# merge 合并数据集，相同列的共同值。
In [7]: pd.merge(df1, df2)    #不指定列名，默认会选择列名相同的key列。
Out[7]:
   data1 key  data2
0      0   b      1
1      1   b      1
2      6   b      1
3      2   a      0
4      4   a      0
5      5   a      0

In [8]: pd.merge(df1, df2, on='key') # 指定列名
Out[8]:
   data1 key  data2
0      0   b      1
1      1   b      1
2      6   b      1
3      2   a      0
4      4   a      0
5      5   a      0

# 可以将不同列名进行链接。

In [9]: df3 = DataFrame({'lkey':list('bbacaab'),'data1':range(7)})

In [10]: df4 = DataFrame({'rkey':list('abd'),'data2':range(3)})

In [11]: pd.merge(df3,df4,left_on='lkey',right_on='rkey')
Out[11]:
   data1 lkey  data2 rkey
0      0    b      1    b
1      1    b      1    b
2      6    b      1    b
3      2    a      0    a
4      4    a      0    a
5      5    a      0    a

# 默认inner，通过how可以指定outer
In [12]: pd.merge(df1,df2,how='outer')
Out[12]:
   data1 key  data2
0    0.0   b    1.0
1    1.0   b    1.0
2    6.0   b    1.0
3    2.0   a    0.0
4    4.0   a    0.0
5    5.0   a    0.0
6    3.0   c    NaN
7    NaN   d    2.0

In [13]: pd.merge(df1,df2,how='left')
Out[13]:
   data1 key  data2
0      0   b    1.0
1      1   b    1.0
2      2   a    0.0
3      3   c    NaN
4      4   a    0.0
5      5   a    0.0
6      6   b    1.0

In [14]: pd.merge(df1,df2,how='right',on='key')
Out[14]:
   data1 key  data2
0    0.0   b      1
1    1.0   b      1
2    6.0   b      1
3    2.0   a      0
4    4.0   a      0
5    5.0   a      0
6    NaN   d      2

# 多个键 合并

In [15]: left = DataFrame({'key1':['foo','foo','bar'],'key2':['one','two','one']
    ...: ,'lval':[1,2,3]})

In [16]: right = DataFrame({'key1':['foo','foo','bar','bar'],'key2':['one','one'
    ...: ,'one','two'],'rval':[4,5,6,7]})

In [17]: pd.merge(left,right,on=['key1','key2'],how='outer')
Out[17]:
  key1 key2  lval  rval
0  foo  one   1.0   4.0
1  foo  one   1.0   5.0
2  foo  two   2.0   NaN
3  bar  one   3.0   6.0
4  bar  two   NaN   7.0

# 如果使用的on选项后，还有重复列名，怎么办？
# merge有一个suffixes选项，可以处理重复列名

In [18]: pd.merge(left,right,on='key1')
Out[18]:
  key1 key2_x  lval key2_y  rval
0  foo    one     1    one     4
1  foo    one     1    one     5
2  foo    two     2    one     4
3  foo    two     2    one     5
4  bar    one     3    one     6
5  bar    one     3    two     7

In [19]: pd.merge(left,right,on='key1',suffixes=('_left','_right'))
Out[19]:
  key1 key2_left  lval key2_right  rval
0  foo       one     1        one     4
1  foo       one     1        one     5
2  foo       two     2        one     4
3  foo       two     2        one     5
4  bar       one     3        one     6
5  bar       one     3        two     7

```

merge选项：

![](/images/python数据分析/pandas_merge_opt1.png)

![](/images/python数据分析/pandas_merge_opt2.png)


## 1.2 索引上的合并

传入left_index=True或者right_index=True，可以将索引作为链接键使用。

```python
In [20]: left1 = DataFrame({'key':list('abaabc'),'value':range(6)})

In [21]: left2 = DataFrame({'group_val':[3.5,7]},index=['a','b'])

In [22]: left1
Out[22]:
  key  value
0   a      0
1   b      1
2   a      2
3   a      3
4   b      4
5   c      5

In [23]: left2
Out[23]:
   group_val
a        3.5
b        7.0

# 将右DataFrame的index作为链接键；
In [24]: pd.merge(left1, left2 ,left_on='key', right_index=True)
Out[24]:
  key  value  group_val
0   a      0        3.5
2   a      2        3.5
3   a      3        3.5
1   b      1        7.0
4   b      4        7.0

In [25]: pd.merge(left1, left2 ,left_on='key', right_index=True, how='outer')
Out[25]:
  key  value  group_val
0   a      0        3.5
2   a      2        3.5
3   a      3        3.5
1   b      1        7.0
4   b      4        7.0
5   c      5        NaN

# 对于层次化索引，比较复杂
In [28]: lefth = DataFrame({'key1':['Ohio','Ohio','Ohio','Nevada','Nevada'],'key
    ...: 2':[2000,2001,2002,2001,2002],'data':np.arange(5.)})

In [29]: righth = DataFrame(np.arange(12).reshape(6,2),index=[['Nevada','Nevada'
    ...: ,'Ohio','Ohio','Ohio','Ohio'],[2001,2000,2000,2000,2001,2002]],columns=
    ...: ['event1','event2'])

In [30]: lefth
Out[30]:
   data    key1  key2
0   0.0    Ohio  2000
1   1.0    Ohio  2001
2   2.0    Ohio  2002
3   3.0  Nevada  2001
4   4.0  Nevada  2002

In [31]: righth
Out[31]:
             event1  event2
Nevada 2001       0       1
       2000       2       3
Ohio   2000       4       5
       2000       6       7
       2001       8       9
       2002      10      11

# 使用列表的形式，指明多个列作为合并键
In [32]: pd.merge(lefth, righth, left_on=['key1','key2'],right_index=True)
Out[32]:
   data    key1  key2  event1  event2
0   0.0    Ohio  2000       4       5
0   0.0    Ohio  2000       6       7
1   1.0    Ohio  2001       8       9
2   2.0    Ohio  2002      10      11
3   3.0  Nevada  2001       0       1

In [33]: pd.merge(lefth, righth, left_on=['key1','key2'],right_index=True, how='
    ...: outer')
Out[33]:
   data    key1    key2  event1  event2
0   0.0    Ohio  2000.0     4.0     5.0
0   0.0    Ohio  2000.0     6.0     7.0
1   1.0    Ohio  2001.0     8.0     9.0
2   2.0    Ohio  2002.0    10.0    11.0
3   3.0  Nevada  2001.0     0.0     1.0
4   4.0  Nevada  2002.0     NaN     NaN
4   NaN  Nevada  2000.0     2.0     3.0

In [34]: left2 = DataFrame([[1,2],[3,4],[5,6]],index=['a','c','e'],columns=['Ohi
    ...: o','Nevada'])

In [35]: right2 = DataFrame([[7,8],[9,10],[11,12],[13,14]],index=list('bcde'),co
    ...: lumns=['Missouri','Alabama'])

In [36]: left2
Out[36]:
   Ohio  Nevada
a     1       2
c     3       4
e     5       6

In [37]: right2
Out[37]:
   Missouri  Alabama
b         7        8
c         9       10
d        11       12
e        13       14

# 同时根据两边索引合并
In [38]: pd.merge(left2, right2, how='outer', left_index=True, right_index=True)
    ...:
Out[38]:
   Ohio  Nevada  Missouri  Alabama
a   1.0     2.0       NaN      NaN
b   NaN     NaN       7.0      8.0
c   3.0     4.0       9.0     10.0
d   NaN     NaN      11.0     12.0
e   5.0     6.0      13.0     14.0
```

DataFrame还有join方法，更好的合并索引。

```python
In [39]: left2.join(right2, how='outer')
Out[39]:
   Ohio  Nevada  Missouri  Alabama
a   1.0     2.0       NaN      NaN
b   NaN     NaN       7.0      8.0
c   3.0     4.0       9.0     10.0
d   NaN     NaN      11.0     12.0
e   5.0     6.0      13.0     14.0
In [44]: right1
Out[44]:
   group_val
a        3.5
b        7.0

In [45]: left1
Out[45]:
  key  value
0   a      0
1   b      1
2   a      2
3   a      3
4   b      4
5   c      5

# left1 使用key来合并，right1使用索引。
In [46]: left1.join(right1, on='key')
Out[46]:
  key  value  group_val
0   a      0        3.5
1   b      1        7.0
2   a      2        3.5
3   a      3        3.5
4   b      4        7.0
5   c      5        NaN

# 对于简单的索引合并，可以在join传入一个DataFrame组。

In [47]: another = DataFrame([[7,8],[9,10],[11,12],[16,17]],index=list('acef'),c
    ...: olumns=['NewYork','Oregon'])

In [48]: left2.join([right2, another])  # 传入list
Out[48]:
   Ohio  Nevada  Missouri  Alabama  NewYork  Oregon
a     1       2       NaN      NaN        7       8
c     3       4       9.0     10.0        9      10
e     5       6      13.0     14.0       11      12
In [50]: left2.join([right2, another],how='outer')
Out[50]:
   Ohio  Nevada  Missouri  Alabama  NewYork  Oregon
a   1.0     2.0       NaN      NaN      7.0     8.0
b   NaN     NaN       7.0      8.0      NaN     NaN
c   3.0     4.0       9.0     10.0      9.0    10.0
d   NaN     NaN      11.0     12.0      NaN     NaN
e   5.0     6.0      13.0     14.0     11.0    12.0
f   NaN     NaN       NaN      NaN     16.0    17.0
```

concat函数也能实现类似功能，后面介绍。

## 1.3 轴向连接

这是另一种连接，也叫作：连接、绑定或堆叠。

```python
# numpy的函数：concatenation
In [51]: arr = np.arange(12).reshape(3,4)

In [52]: np.concatenate([arr,arr],axis=1)
Out[52]:
array([[ 0,  1,  2,  3,  0,  1,  2,  3],
       [ 4,  5,  6,  7,  4,  5,  6,  7],
       [ 8,  9, 10, 11,  8,  9, 10, 11]])

In [53]: np.concatenate([arr,arr],axis=0)
Out[53]:
array([[ 0,  1,  2,  3],
       [ 4,  5,  6,  7],
       [ 8,  9, 10, 11],
       [ 0,  1,  2,  3],
       [ 4,  5,  6,  7],
       [ 8,  9, 10, 11]])

```

对于pandas也能推广，

```python
In [54]: s1 = Series([0,1],index=list('ab'))

In [55]: s2 = Series([2,3,4],index=list('cde'))

In [56]: s3 = Series([5,6],index=list('fg'))

In [57]: pd.concat([s1,s2,s3])
Out[57]:
a    0
b    1
c    2
d    3
e    4
f    5
g    6
dtype: int64

# 做一个DataFrame出来。
In [58]: pd.concat([s1,s2,s3],axis=1)
Out[58]:
     0    1    2
a  0.0  NaN  NaN
b  1.0  NaN  NaN
c  NaN  2.0  NaN
d  NaN  3.0  NaN
e  NaN  4.0  NaN
f  NaN  NaN  5.0
g  NaN  NaN  6.0


# 使用inner选项，求并集
In [59]: s4 = pd.concat([s1*5,s3])

In [60]: s4
Out[60]:
a    0
b    5
f    5
g    6
dtype: int64

In [61]: pd.concat([s1,s4],axis=1)
Out[61]:
     0  1
a  0.0  0
b  1.0  5
f  NaN  5
g  NaN  6

In [62]: pd.concat([s1,s4],axis=1,join='inner')
Out[62]:
   0  1
a  0  0
b  1  5


# 指定求那些索引的并集
In [63]: pd.concat([s1,s4],axis=1,join_axes=[list('acbe')])
Out[63]:
     0    1
a  0.0  0.0
c  NaN  NaN
b  1.0  5.0
e  NaN  NaN

In [64]: pd.concat([s1,s4],axis=1,join_axes=[list('acbef')])
Out[64]:
     0    1
a  0.0  0.0
c  NaN  NaN
b  1.0  5.0
e  NaN  NaN
f  NaN  5.0

# 用keys创建一个自己的层次化索引
In [65]: result = pd.concat([s1,s2,s3],keys=['one','two','three'])

In [66]: result
Out[66]:
one    a    0
       b    1
two    c    2
       d    3
       e    4
three  f    5
       g    6
dtype: int64

# 在axis=1轴上连接的话，keys就成了列
In [70]: result = pd.concat([s1,s2,s3],keys=['one','two','three'],axis=1)

In [71]: result
Out[71]:
   one  two  three
a  0.0  NaN    NaN
b  1.0  NaN    NaN
c  NaN  2.0    NaN
d  NaN  3.0    NaN
e  NaN  4.0    NaN
f  NaN  NaN    5.0
g  NaN  NaN    6.0
```

同样的逻辑，对DataFrame也是一样，

```python
In [72]: df1 = DataFrame(np.arange(6).reshape(3,2),index=list('abc'),columns=['o
    ...: ne','two'])

In [73]: df2 = DataFrame(np.arange(4).reshape(2,2),index=list('ac'),columns=['th
    ...: ree','four'])

In [74]: pd.concat([df1,df2],axis=1,keys=['level1','level2'])
Out[74]:
  level1     level2     
     one two  three four
a      0   1    0.0  1.0
b      2   3    NaN  NaN
c      4   5    2.0  3.0

In [75]: pd.concat([df1,df2],axis=1)
Out[75]:
   one  two  three  four
a    0    1    0.0   1.0
b    2    3    NaN   NaN
c    4    5    2.0   3.0

In [76]: pd.concat([df1,df2])
Out[76]:
   four  one  three  two
a   NaN  0.0    NaN  1.0
b   NaN  2.0    NaN  3.0
c   NaN  4.0    NaN  5.0
a   1.0  NaN    0.0  NaN
c   3.0  NaN    2.0  NaN


# 还可以传入字典
In [77]: pd.concat({'level1':df1,'level2':df2},axis=1)
Out[77]:
  level1     level2     
     one two  three four
a      0   1    0.0  1.0
b      2   3    NaN  NaN
c      4   5    2.0  3.0

# 没用的索引怎么处理？
In [78]: df3 = DataFrame(np.random.randn(3,4),columns=list('abcd'))

In [79]: df4 = DataFrame(np.random.randn(2,3),columns=list('bda'))

  ## 产生新的一列索引，顺序从0开始
In [80]: pd.concat([df3,df4],ignore_index=True)
Out[80]:
          a         b         c         d
0  0.322805 -1.495024  0.971860 -0.904791
1  0.332766 -2.320841 -0.903456  0.026713
2 -0.163670  0.387144 -0.746927 -0.360291
3 -0.074411 -0.557447       NaN  0.704929
4  2.673221 -1.143143       NaN  0.592930
```

![](/images/python数据分析/pandas_concat_opt.png)

## 1.4 合并重叠数据

合并重叠数据不能使用merge和concat来实现，使用np.where函数。

```python
In [81]: a = Series([np.nan,2.5,np.nan,3.5,4.5,np.nan],index=list('fedcba'))

In [85]: b = Series([0,1,2,3,4,np.nan],index=list('fedcba'))

In [86]: a
Out[86]:
f    NaN
e    2.5
d    NaN
c    3.5
b    4.5
a    NaN
dtype: float64

In [87]: b
Out[87]:
f    0.0
e    1.0
d    2.0
c    3.0
b    4.0
a    NaN
dtype: float64

# 实现如果a为空，就是用b
In [88]: np.where(pd.isnull(a), b, a)
Out[88]: array([ 0. ,  2.5,  2. ,  3.5,  4.5,  nan])

# 相同的目标
In [95]: a.combine_first(b)
Out[95]:
f    0.0
e    2.5
d    2.0
c    3.5
b    4.5
a    NaN
dtype: float64

```

针对DataFrame：

```python
In [96]: df1 = DataFrame({'a':[1,np.nan,5,np.nan],'b':[np.nan,2,np.nan,6],'c':ra
    ...: nge(2,18,4)})

In [97]: df2 = DataFrame({'a':[5,4,np.nan,3,7],'b':[np.nan,3,4,6,8]})

In [98]: df1.combine_first(df2)
Out[98]:
     a    b     c
0  1.0  NaN   2.0
1  4.0  2.0   6.0
2  5.0  4.0  10.0
3  3.0  6.0  14.0
4  7.0  8.0   NaN
```















# 2 重塑和轴向旋转

# 3 数据转换

# 4 字符串操作

# 5 示例：usda食品数据库
