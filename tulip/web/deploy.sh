#!/bin/bash
# deploys web build to github pages


git rm -f ../../www/run/*
set -e
cp stage/run/* ../../www/run/
git add ../../www/run/*
