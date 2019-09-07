#!/usr/bin/env python3

#
#  Python3 Scripts
#
#  Make at  2019-09-05
#  
#  create a html file  like  /calendar.html
#  can change the year number like 2020 to create a new calendar
#  every <rect> object  have a "d" attribute for date in the year, the format like this
#
#      d="2019-09-05"
#
#  the location of this cripts   BASE_DIR/scripts/xxxx.py
#  DO NOT MOVE IT ！！！
#

import arrow
 
 
def isLeapYear(years):
    '''
    通过判断闰年，获取年份years下一年的总天数
    :param years: 年份，int
    :return:days_sum，一年的总天数
    '''
    # 断言：年份不为整数时，抛出异常。
    assert isinstance(years, int), "请输入整数年，如 2018"
 
    if ((years % 4 == 0 and years % 100 != 0) or (years % 400 == 0)):  # 判断是否是闰年
        # print(years, "是闰年")
        days_sum = 366
        return days_sum
    else:
        # print(years, '不是闰年')
        days_sum = 365
        return days_sum
 
 
def getAllDayPerYear(years):
    '''
    获取一年的所有日期
    :param years:年份
    :return:全部日期列表
    '''
    start_date = '%s-1-1' % years
    a = 0
    all_date_list = []
    days_sum = isLeapYear(int(years))
    print()
    while a < days_sum:
        b = arrow.get(start_date).shift(days=a).format("YYYY-MM-DD")
        a += 1
        all_date_list.append(b)
    # print(all_date_list)
    return all_date_list
 
 
if __name__ == '__main__':

    The_YEAR = '2019'
    BOTH_DAY_COLOR = '#009a48'       # 深绿
    # POST_DAY_COLOR = '#F6A6A1'     # 浅红
    POST_DAY_COLOR = '#c3e395'     # 浅绿
    WRITE_DAY_COLOR = '#c3e395'     # 浅绿
    OTHER_DAY_COLIR = '#ebedf0'  # 浅灰色


    import os
    import sys
    import re

    all_date_list = getAllDayPerYear(The_YEAR)
    dir_name = os.path.dirname(os.path.dirname(sys.argv[0]))
    days_list_of_post = []
    days_list_of_jpg = []

    # 列举post文件名的2019年份的所有文件名前缀，如 2019-09-03
    for post_root, post_dirs, post_filename in os.walk(
            os.path.join(dir_name, '_posts')):
        for f in post_filename:
            obj_seach = re.search("%s-\d{2}-\d{2}" % The_YEAR,f)
            if obj_seach:
                days_list_of_post.append(obj_seach.group())

    # 列举 images/write-homework 下的图片前缀，
    for post_root, post_dirs, post_filename in os.walk(
        os.path.join(dir_name, 'images/write-homework')):
        for f in post_filename:
            obj_seach = re.search("%s\d{2}\d{2}" % The_YEAR,f)
            if obj_seach:
                day_search = obj_seach.group()
                day_reformat = '%s-%s-%s' % (
                    day_search[0:4], day_search[4:6],day_search[6:8])
                days_list_of_jpg.append(day_reformat)

    for d in all_date_list:
        if d in days_list_of_post and d in days_list_of_jpg:
            print("rect#id-%s{ fill: %s; opacity: 5%%; }" % (d, BOTH_DAY_COLOR))
        elif d in days_list_of_post:
            print("rect#id-%s{ fill: %s; opacity: 5%%; }" % (d, POST_DAY_COLOR))
        elif d in days_list_of_jpg:
            print("rect#id-%s{ fill: %s; opacity: 5%%; }" % (d, WRITE_DAY_COLOR))
        else:
            print("rect#id-%s{ fill: %s; opacity: 5%%;}" % (d, OTHER_DAY_COLIR))
