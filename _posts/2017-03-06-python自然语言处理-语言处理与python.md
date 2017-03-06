---
layout: post
title: 语言处理与python
date: 2017-03-06 12:00
author: felo
---


## 1.1 语言计算：文本和单词

### python入门

略

### NLTK入门

```python
In [1]: import nltk
In [10]: nltk.download()  # 下载书中所需数据

```

![](/images/python_nltk/nltk_download.png)

```python
In [49]: from nltk import book
*** Introductory Examples for the NLTK Book ***
Loading text1, ..., text9 and sent1, ..., sent9
Type the name of the text or sentence to view it.
Type: 'texts()' or 'sents()' to list the materials.
text1: Moby Dick by Herman Melville 1851
text2: Sense and Sensibility by Jane Austen 1811
text3: The Book of Genesis
text4: Inaugural Address Corpus
text5: Chat Corpus
text6: Monty Python and the Holy Grail
text7: Wall Street Journal
text8: Personals Corpus
text9: The Man Who Was Thursday by G . K . Chesterton 1908

In [50]: book.text1
Out[50]: <Text: Moby Dick by Herman Melville 1851>

In [51]: book.text2
Out[51]: <Text: Sense and Sensibility by Jane Austen 1811>
```

### 搜索文本

```python
# 精确匹配单词
In [52]: book.text1.concordance("monstrous")
Displaying 11 of 11 matches:
ong the former , one was of a most monstrous size . ... This came towards us ,
ON OF THE PSALMS . " Touching that monstrous bulk of the whale or ork we have r
ll over with a heathenish array of monstrous clubs and spears . Some were thick
d as you gazed , and wondered what monstrous cannibal and savage could ever hav
that has survived the flood ; most monstrous and most mountainous ! That Himmal
they might scout at Moby Dick as a monstrous fable , or still worse and more de
th of Radney .'" CHAPTER 55 Of the Monstrous Pictures of Whales . I shall ere l
ing Scenes . In connexion with the monstrous pictures of whales , I am strongly
ere to enter upon those still more monstrous stories of them which are to be fo
ght have been rummaged out of this monstrous cabinet there is no telling . But
of Whale - Bones ; for Whales of a monstrous size are oftentimes cast up dead u

# 模糊匹配上下文
In [53]: book.text1.similar("monstrous")
imperial subtly impalpable pitiable curious abundant perilous
trustworthy untoward singular lamentable few determined maddens
horrible tyrannical lazy mystifying christian exasperate

In [54]: book.text2.similar("monstrous")
very exceedingly so heartily a great good amazingly as sweet
remarkably extremely vast

In [55]: book.text2.common_contexts(["monstrous","very"])
a_pretty is_pretty a_lucky am_glad be_glad
```


```python
# 分散作图
In [56]: book.text4.dispersion_plot(["citizens","democracy","freedom","duties","
    ...: America"])
```

![](/images/python_nltk/dispersion_plot.png)



### 计数词汇

```python
In [5]: len(book.text3)  # 计算所有词数
Out[5]: 44764

In [6]: len(sorted(set(book.text3)))  # 去重
Out[6]: 2789

In [7]: from __future__ import division

In [9]: len(book.text3)/len(set(book.text3))  # 每个词平均出现16次
Out[9]: 16.050197203298673

In [10]: book.text3.count('smote')
Out[10]: 5

In [11]: 100 * book.text4.count("a")/len(book.text4)  # a出现的比例
Out[11]: 1.4643016433938312

```


## 1.2 将文本当做词链表

### 链表(list)

```python
# 相加
In [17]: book.sent3 + book.sent1
Out[17]:
['In',
 'the',
 'beginning',
 'God',
 'created',
 'the',
 'heaven',
 'and',
 'the',
 'earth',
 '.',
 'Call',
 'me',
 'Ishmael',
 '.']

#追加
In [18]: book.sent1.append('Some')

In [19]: book.sent1
Out[19]: ['Call', 'me', 'Ishmael', '.', 'Some']

```

### 索引列表

```python
In [20]: book.text5[173] # 索引
Out[20]: u'to'

In [21]: book.text5[16715:16725]  # 切片
Out[21]:
[u'U86',
 u'thats',
 u'why',
 u'something',
 u'like',
 u'gamefly',
 u'is',
 u'so',
 u'good',
 u'because']

# list其他属性，略
```

### 变量

```python
In [22]: vocab = set(book.text1)

In [23]: len(vocab)
Out[23]: 19317

```

### 字符串

```python
In [24]: name = 'Monty'

In [25]: name*2
Out[25]: 'MontyMonty'

In [26]: name[2]
Out[26]: 'n'

In [27]: ''.join(['Monty','!'])
Out[27]: 'Monty!'

```


## 1.3 简单统计

### 频率分布

手动统计词频很费劲，不过计算机处理就更好一些。

```python
# 统计词频
In [28]: fdist1 = book.FreqDist(book.text1)

In [29]: vocabulary1 = fdist1.keys()

In [30]: vocabulary1[:50]
Out[30]:
[u'funereal',
 u'unscientific',
 u'divinely',
 u'foul',
 u'four',
 u'gag',
...
]
In [6]: fdist1.most_common()[:10]
Out[6]:
[(u',', 18713),
 (u'the', 13721),
 (u'.', 6862),
 (u'of', 6536),
 (u'and', 6024),
 (u'a', 4569),
 (u'to', 4542),
 (u';', 4072),
 (u'in', 3916),
 (u'that', 2982)]

```

使用`fdist1.plot(20)`可以画出前20的词频统计图。

![](/images/python_nltk/freqdist_plot.png)

### 细粒度的选择词

















end
