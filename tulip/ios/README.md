# tulip for iOS

Work in progress

```
cd tulip/ios
make lib # creates build-standard/tulip/obj/libmicropython.a
```

Get the SDL release zip file and put it in ~/ioslib/ (TODO, put it here instead)

Then go into the xcode project and build, which links the .a to a normal iOS .app

We could do this all from the command line (my preferred) with `xcrun` or `xcodebuild`. 

TODOs:
 - download/move SDL into here -- or share SDL among the ports?
 - get simulator/real device build as a makefile parameter
 - debug/release setting as well
 - pass off makefile build to xcode via `./build.sh`
 - soft keyboard is not returning symbols to Tulip, like `(` etc  (hard keyboard works)
 - screen size is not using hidpi -- for phone this is required
 - load example filesystem on first boot to Documents folder
 - MIDI


