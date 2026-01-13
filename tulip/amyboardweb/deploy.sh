#!/bin/bash
# deploys web build to vercel

set -e

# Just vercel deploy from stage...
vercel deploy stage --prod


