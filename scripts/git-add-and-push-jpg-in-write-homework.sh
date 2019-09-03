sips -Z 1000 images/write-homework/*.jpg

git add .
git commit -m `date "+%Y-%m-%d-%H-%M-%S"`
git push origin master
