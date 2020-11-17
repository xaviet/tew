#!/bin/bash

git checkout --orphan latest_branch
git add -A
commit_date=`date +%Y%m%d`
hn=`hostname`
git commit -am "${commit_date}@$hn"
git branch -D master
git branch -m master
git push -f origin master

