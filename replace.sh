#!/bin/sh
find -type f | grep proj$ | while read line ; do
		sed -i 's/<WindowsTargetPlatformVersion>..\..\......\..<\/WindowsTargetPlatformVersion>/<WindowsTargetPlatformVersion>10.0.16299.0<\/WindowsTargetPlatformVersion>/g' $line
		unix2dos $line
done