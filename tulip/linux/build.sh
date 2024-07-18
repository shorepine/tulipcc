#!/bin/bash
# build.sh
# Just builds locally
set -e

source ../shared/grab_submodules.sh
make DEBUG=1
mkdir -p dev
cp build-standard/tulip/obj/tulip dev/

tulip_home="${HOME}/.local/share/tulipcc"

mkdir -p "${tulip_home}"
mkdir -p "${tulip_home}/user"

tulip_sys="${tulip_home}/sys"

if [ -d ${tulip_sys} ]
then
  echo "refreshing ${tulip_sys}"

  # safely delete ${tulip_sys}
  find "${tulip_sys}" ! -type d -delete
  find "${tulip_sys}" -type d -delete
fi

mkdir -p "${tulip_sys}"
cp -Rf ../fs/* "${tulip_sys}"