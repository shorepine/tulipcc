#!/bin/bash
# deploys web build to github pages

set -e

git rm -f ../../www/run/*
mkdir ../../www/run
cp stage/run/* ../../www/run/
git add ../../www/run/*
