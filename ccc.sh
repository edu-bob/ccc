#!/bin/sh
#
# Top level concurrent C driver
#
LIBDIR=$(LIBDIR)
LIB=libccc.a
debug=false
keep=false
link=true
trace=false
while test $# -gt 0; do
	case "$1" in
	-G)	debug=true ;;
	-x)	trace=true ;;
	-K)	keep=true ;;
	-g)	cc1args="$cc1args $1"
		ccargs="$ccargs $1" ;;
	-O)	cc1args="$cc1args $1 -i"
		ccargs="$ccargs $1 -i" ;;
	*.cc)	cccfiles="$cccfiles $1"
		ccargs="$ccargs `basename $1 .cc`.o" ;;
	-c)	link=false ;;
	-D*)	cppargs="$cppargs $1" ;;
	*)	ccargs="$ccargs $1" ;;
	esac
	shift
done
okay=true
$trace && set -x
$debug && cccargs=-G && LIB=libccd.a
for i in $cccfiles; do
	dotsee=/tmp/cc$$.c
	/lib/cpp $cppargs $i | $LIBDIR/cccom $cccargs > "$dotsee"
	if test $? -ne 0
	then
		echo Concurrent C errors detected
		okay=false
	else
		$keep && cb < $dotsee | cat -s > `basename $i .cc`.c 2>/dev/null
		if cc $cc1args -c $dotsee ;then
			mv cc$$.o `basename $i .cc`.o 
		else
			okay=false
		fi
	fi
	rm -f $dotsee
done
$okay && $link && cc $ccargs $LIBDIR/$LIB -lpps
$okay
