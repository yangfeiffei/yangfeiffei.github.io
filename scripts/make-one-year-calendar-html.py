#!/usr/bin/env python

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
    # 获取一年的所有日期
    all_date_list = getAllDayPerYear("2019")

    # create a svg like github contribute 
    # 
    #  sun mon tue th tur fri sat
    #  0    1   2  3   4  5   6
    first_day = 2
    week_counts = 53
    day_counts = 365
    html_txt = ""

    step_of_rect = 13
    day_of_year = 0
    for week in range(week_counts):
        html_txt = html_txt +  '<g transform="translate('+ str((week+1)*step_of_rect) + ', 0)">' + "\n"
        for day in range(7):
            if first_day > 0:
                first_day = first_day -1
                continue
            if day_of_year < 365:
                html_txt = html_txt + '\t<rect class="day" width="11" height="11" y="'+ str(day * step_of_rect) + \
                           '" d="' + all_date_list[day_of_year] + '"></rect>\n'
                day_of_year += 1
        html_txt += "</g>\n"


    html_txt = '''---

layout: default
title: "Calendar"

---

<div class="box-body">
    <!-- CALENDAR -->
    <div id="contributions-calendar">
        <div class="js-graph js-calendar-graph graph-canvas calendar-graph">
            <svg width="721" height="110" id="calendar-graph">
                %s
            </svg>
        </div>
        <p class="text-center">
            ....
        </p>
    </div>
</div>''' % html_txt

    print(html_txt)
