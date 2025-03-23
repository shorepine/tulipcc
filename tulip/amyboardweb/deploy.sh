#!/bin/bash
# deploys web build to github pages

set -e

git rm -f ../../www/amy/*
mkdir ../../www/amy
cp stage/run/* ../../www/amy/
git add ../../www/amy/*
git commit -a -m "deploying AMYboard Web"
git push -u origin HEAD
