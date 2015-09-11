#!/bin/sh
OUTDIR=$1
MODDIR=$2
COMPILER=$4
LINK=

CFLAGS="$3 -lSemphella -fpermissive -Wall -Wno-sign-compare  -lgomp -fopenmp -std=c++11 -fpermissive \
	-Wall -D__LINUX__ -I$(pwd)/../../  -L$OUTDIR"
for file in $(ls | grep .cpp | grep -v \~) ;
	do
		LINK=$LINK $OUTDIR/$file.o
		$4 $CFLAGS -c -o $OUTDIR/$file.o $file 
	done
mkdir -p $MODDIR	
cd $MODDIR
mkdir -p $OUTDIR/mod

for file in $(ls | grep .cpp | grep -v \~);
	do
		LINK=$LINK $OUTDIR/$file.mod.o
		$4 $CFLAGS -c -o $OUTDIR/mod/$file.mod.o $file
	done
	
$4 -o $OUTDIR/TAHITI $LINK