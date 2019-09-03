
# change jpg to 1000 width
sips -Z 1000 images/write-homework/*.jpg
if [ $? == 0 ]; 
then
	# add all jpg and push to master
	git add . &&
	COMMIT_DATE=$(date "+%Y-%m-%d-%H-%M-%S") &&
	git commit -m "add write homework at $COMMIT_DATE"  &&
	git push origin master &&
	echo "========= push seccussed. ========" ||
	echo "Push failed... Please check it..." ;
else
	echo "something wrong about change images.\nPlese check it..." ;
fi
