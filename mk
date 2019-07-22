#!/bin/sh
#
# Sample script to run make without having to retype the long path each time
# This will work if you built the environment using our ~/bin/build-snap script

case $1 in
"-l")
	make -C ../../../BUILD/contrib/snapdev 2>&1 | less -SR
	;;

"-d")
	rm -rf ../../../BUILD/contrib/snapdev/doc/snapdev-doc-2.0.tar.gz \
	       ../../../BUILD/contrib/snapdev/doc/snapdev-doc-2.0
	make -C ../../../BUILD/contrib/snapdev
	;;

"-t")
	(
		if make -C ../../../BUILD/contrib/snapdev
		then
			../../../BUILD/contrib/snapdev/tests/snapdevtests --progress
		fi
	) 2>&1 | less -SR
	;;

"-i")
	make -C ../../../BUILD/contrib/snapdev install
	;;

*)
	make -C ../../../BUILD/contrib/snapdev
	;;

esac
