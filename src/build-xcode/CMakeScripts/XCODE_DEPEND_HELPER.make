# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.main.Debug:
/Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/Debug/main:\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-athena.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-core.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-crt-cpp.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-mqtt.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-event-stream.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-s3.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-auth.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-http.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-io.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-compression.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-cal.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-sdkutils.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-checksums.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-common.1.0.0.dylib
	/bin/rm -f /Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/Debug/main


PostBuild.main.Release:
/Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/Release/main:\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-athena.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-core.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-crt-cpp.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-mqtt.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-event-stream.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-s3.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-auth.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-http.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-io.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-compression.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-cal.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-sdkutils.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-checksums.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-common.1.0.0.dylib
	/bin/rm -f /Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/Release/main


PostBuild.main.MinSizeRel:
/Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/MinSizeRel/main:\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-athena.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-core.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-crt-cpp.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-mqtt.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-event-stream.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-s3.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-auth.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-http.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-io.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-compression.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-cal.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-sdkutils.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-checksums.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-common.1.0.0.dylib
	/bin/rm -f /Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/MinSizeRel/main


PostBuild.main.RelWithDebInfo:
/Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/RelWithDebInfo/main:\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-athena.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-core.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-crt-cpp.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-mqtt.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-event-stream.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-s3.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-auth.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-http.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-io.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-compression.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-cal.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-sdkutils.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-checksums.1.0.0.dylib\
	/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-common.1.0.0.dylib
	/bin/rm -f /Users/ryanhamby/Documents/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/InvertedYieldCurveTrader/src/build-xcode/RelWithDebInfo/main




# For each target create a dummy ruleso the target does not have to exist
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-auth.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-cal.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-common.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-compression.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-event-stream.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-http.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-io.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-mqtt.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-s3.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-c-sdkutils.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-checksums.1.0.0.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-athena.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-cpp-sdk-core.dylib:
/Users/ryanhamby/Desktop/aws-sdk-cpp-install/lib/libaws-crt-cpp.dylib:
