#!/usr/bin/env python3
import tkinter as tk
# import re
# import time
import http.client as httplib
import hashlib
import urllib
import random
import json
# import config file
import baidu_config_file

appid = baidu_config_file.appid
secretKey = baidu_config_file.secretKey

# 公共参数：
LAST_SOURCE_TXT=''
SAME_SOURCE_FLAG=False

# 2.判断是否与上一次的一样，如果一样就返回一个SAME_SOURCE_FLAG=True
# 3.如果不一样，更新LAST_SOURCE_TXT
# 处理前后的字符串，去除回车符号
# 4.最后返回处理好的source_txt
def parse_source_txt(source_txt):
    global SAME_SOURCE_FLAG
    global LAST_SOURCE_TXT
    if source_txt == LAST_SOURCE_TXT:
        pass
        SAME_SOURCE_FLAG = True
    else:
        LAST_SOURCE_TXT = source_txt
        SAME_SOURCE_FLAG = False
        source_txt = source_txt.replace("\n","")
        return source_txt

def trans(source_txt):
    if SAME_SOURCE_FLAG == True:
        return ''
    httpClient = None
    myurl = '/api/trans/vip/translate'
    q = source_txt
    fromLang = 'auto'
    toLang = 'zh'
    salt = random.randint(32768, 65536)

    sign = appid + q + str(salt) + secretKey
    m1 = hashlib.md5()
    m1.update(sign.encode(encoding='UTF-8'))
    sign = m1.hexdigest()
    myurl = myurl + '?appid=' + appid + '&q=' + urllib.parse.quote(q) + '&from=' + fromLang + \
            '&to=' + toLang + '&salt=' + str(salt) + '&sign=' + sign

    try:
        httpClient = httplib.HTTPConnection('api.fanyi.baidu.com')
        httpClient.request('GET', myurl)

        # response是HTTPResponse对象
        response = httpClient.getresponse()
        ret = response.read().decode()
        ret = json.loads(ret)
        target_txt = ''
        for s in ret['trans_result']:
            target_txt = target_txt + s['dst'] + '\n'

        return target_txt

    except Exception as e:
        return e
    finally:
        if httpClient:
            httpClient.close()

def update_text():
    source_txt = parse_source_txt(root.clipboard_get())
    if SAME_SOURCE_FLAG == False:
        target_text = trans(source_txt)
        t.delete('0.0','end')
        t.insert('end', target_text)
        root.update()
    else:
        pass
    t.after(1000, update_text)

if __name__ == '__main__':
    root = tk.Tk()
    root.wm_attributes('-topmost', 1)
    root.title("mytrans")
    t = tk.Text(root, height=10, width=80, font=('', 12))
    t.pack()
    source_txt = parse_source_txt(root.clipboard_get())
    # if source_txt:
    #     target_text = trans(source_txt)
    # t.after(1000, update_text)

    target_txt = trans(source_txt)
    t.insert('end', target_txt)
    root.update()
    t.after(1000, update_text)

    root.mainloop()