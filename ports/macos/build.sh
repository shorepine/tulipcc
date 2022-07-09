# build.sh
# Just builds locally

make 
mkdir -p dev
rm -rf dev/Tulip\ CC.app
mkdir -p dev/Tulip\ CC.app/Contents/{MacOS,Resources,Frameworks,libs}
cp tulip.$(arch) dev/Tulip\ CC.app/Contents/MacOS/tulip
cp Info.plist dev/Tulip\ CC.app/Contents/
cp -rf ../../tulip_home dev/Tulip\ CC.app/Contents/Resources/
cp -a SDL2.framework dev/Tulip\ CC.app/Contents/Frameworks/
cp libs/libsoundio.2.0.0.dylib dev/Tulip\ CC.app/Contents/libs/
install_name_tool -add_rpath @executable_path/../Frameworks dev/Tulip\ CC.app/Contents/MacOS/tulip
install_name_tool -change "@@HOMEBREW_PREFIX@@/opt/libsoundio/lib/libsoundio.2.dylib" "@executable_path/../libs/libsoundio.2.0.0.dylib" "dev/Tulip CC.app/Contents/MacOS/tulip"



