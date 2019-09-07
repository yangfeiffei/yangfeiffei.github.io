#!/bin/bash

YEAR="2019"
python scripts/make-one-year-calendar-css.py  > custom/css/commit-day-$YEAR.css

echo -n "Please input the Commit Content: "
read commit_content

git add . &&
git commit -m "$commit_content" &&
git push origin master 
