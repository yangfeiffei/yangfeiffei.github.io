# -*- coding: utf-8 -*-
#! env /Users/yangfeilong/.pyenv/shims/python
"""
Spyder Editor

This is a temporary script file.
"""

import matplotlib.pyplot as plt  
from wordcloud import WordCloud  
import jieba  
import os

wc = ''

for r,d,f in os.walk('/Users/yangfeilong/GitHub/yangfeiffei.github.io/_posts/'):
    file_list = f
    file_root = r
for f in file_list:    
    text_from_file_with_apath = open(file_root+f).read()  
      
    wordlist_after_jieba = jieba.cut(text_from_file_with_apath,cut_all = True) 
    wl_space_split = " ".join(wordlist_after_jieba)  
    wc += wl_space_split

my_wordcloud=WordCloud().generate(wl_space_split)  
  
plt.imshow(my_wordcloud)  
plt.axis("off")  
plt.savefig('/Users/yangfeilong/GitHub/yangfeiffei.github.io/images/wordcloud.png', dpi=400, bbox_inches='tight')
