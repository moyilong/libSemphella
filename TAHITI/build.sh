#!/bin/sh
OUTDIR=$1
MODDIR=$2
CFLAGS=$3
COMPILER=$4
LINK=
for file in $(ls | grep .cpp | grep -v \~) ;
	do
		LINK=$LINK $OUTDIR/$file.o
		$4 $3 -c -o $OUTDIR/$file.o $file 
	done
mkdir -p $MODDIR	
cd $MODDIR
mkdir -p $OUTDIR/mod

for file in $(ls | grep .cpp | grep -v \~);
	do
		LINK=$LINK $OUTDIR/$file.mod.o
		$4 $3 -c -o $OUTDIR/mod/$file.mod.o $file
	done
	
$4 -o $OUTDIR/TAHITI $LINK