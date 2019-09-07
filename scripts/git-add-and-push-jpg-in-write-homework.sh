#!/bin/bash


#  文件只能在 主目录执行
#  依赖   scripts/make-one-year-calendar-css.py  脚本
#  创建时间： 2019-09-07
#

YEAR="2019"

echo -n "[Step 1] 修改jpg文件的分辨率......"

# change jpg to 1000 width
/usr/bin/sips -Z 1000 images/write-homework/*.jpg
if [ $? == 0 ];
then
    # 修改css文件
    echo "seccussed."
    echo -n "[Step 2] css change......"
    python scripts/make-one-year-calendar-css.py  > custom/css/commit-day-$YEAR.css
    echo "seccussed."
	# add all jpg and push to master
	echo -n "[Step 3] ADD and PUSH to master ......"
	git add . &&
	COMMIT_DATE=$(date "+%Y-%m-%d-%H-%M-%S") &&
	git commit -m "add write homework at $COMMIT_DATE"  &&
	git push origin master &&
	echo "seccussed." ||
	echo "Push failed... Please check it..." ;
else
	echo "something wrong about change images.\nPlese check it..." ;
fi
