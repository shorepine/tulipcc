#!/bin/bash
set -e 
# package.sh
# packages the Tulip Deskop binary into a macOS app, universal binary

# Note : on mac you need both types of homebrew, intel and AS
# https://medium.com/mkdir-awesome/how-to-install-x86-64-homebrew-packages-on-apple-m1-macbook-54ba295230f
# arch -x86_64 /usr/local/homebrew/bin/brew install libffi 

# I also first stored my Apple credentials with this
# xcrun notarytool store-credentials --apple-id "brian@variogram.com" --team-id "Y6CQ3JU8G4"


DEV_ID="Developer ID Application: Brian Whitman (Y6CQ3JU8G4)"
# Build for arm then intel
rm -rf build-standard
make WHICH_ARCH=arm64
cp build-standard/tulip/obj/tulip.arm64 .
rm -rf build-standard
make WHICH_ARCH=x86_64
cp build-standard/tulip/obj/tulip.x86_64 .
rm -rf build-standard

# lipo them together
lipo -create -output tulip tulip.x86_64 tulip.arm64
rm tulip.x86_64
rm tulip.arm64
rm -rf dist
mkdir -p dist

# Now make the app bundle
mkdir -p dist/Tulip\ Desktop.app/Contents/{MacOS,Resources,Frameworks,libs}
cp tulip dist/Tulip\ Desktop.app/Contents/MacOS
cp Info.plist dist/Tulip\ Desktop.app/Contents/
cp -rf ../fs dist/Tulip\ Desktop.app/Contents/Resources/
cp -a SDL2.framework dist/Tulip\ Desktop.app/Contents/Frameworks/
install_name_tool -add_rpath @executable_path/../Frameworks dist/Tulip\ Desktop.app/Contents/MacOS/tulip
cp tulip.icns dist/Tulip\ Desktop.app/Contents/Resources/
# I then run SD Notary2 on this .app file. It creates a .dmg file

# Yikes, but https://stackoverflow.com/questions/60608887/what-is-the-most-efficient-way-to-notarize-and-staple-a-zip-containing-a-app
codesign -s "${DEV_ID}" dist/Tulip\ Desktop.app/Contents/Frameworks/SDL2.framework/Versions/A/SDL2 -f
codesign -s "${DEV_ID}" dist/Tulip\ Desktop.app/Contents/MacOS/tulip -f
codesign --force --options runtime --timestamp --sign "${DEV_ID}" dist/Tulip\ Desktop.app
cd dist
/usr/bin/ditto -c -k --sequesterRsrc --keepParent Tulip\ Desktop.app Tulip_Desktop.zip
xcrun notarytool submit Tulip_Desktop.zip --keychain-profile "baw" --wait
rm -rf Tulip\ Desktop.app
unzip Tulip_Desktop.zip
xcrun stapler staple Tulip\ Desktop.app
/usr/bin/ditto -c -k --sequesterRsrc --keepParent Tulip\ Desktop.app Tulip_Desktop.zip
cd ..

