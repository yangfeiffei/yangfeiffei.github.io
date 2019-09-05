#!/bin/bash


echo -n "Please input the Commit Content: "
read commit_content

git add . &&
git commit -m "$commit_content" &&
git push origin master 
