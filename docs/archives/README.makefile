The following targets exist in the makefile, with their use:

all:		Rebuild all programs
dip:		Rebuild dip program
cmap:		Rebuild cmap program
summary:	Rebuild summary program
bgreet:		Rebuild bgreet program (obsolete, used to send
		"Happy birthday" to registered players)
deddump:	Rebuild dedump program
delgame:	Rebuild delgame program (to erase permanentely a game)
flock:		Rebuild flock program (used for locking of dip.log)
fmtwho:		Rebuild fmtwho program
ign:		rebuild ign program
pdip:		Rebuild pdip program (no longer used?)
rdip:		Rebuild rdip program
Datamake:	Rebuild the data files
install:	build and install a judge (Note: this will DELETE
		any existing judge data files, so do NOT use on an
		existing judge installation)
remap:		Force binary map files to be regenerated if required
upgrade:	Rebuild all the programs and install them in correct
		directory (used principally for updating a running 
		judge)
flist:		Rebuild the help file "flist" 
dist:		Build an update distribution (calls tarZ, targz and tarbz2)
tarZ:		Build a tar.Z file of judge files for updates
targz:		Build a tar.gz file of judge files for updates
tarbz2:		Build a tar.bz2 files of judge files for updates
tar:		Build a tar file of judge files for updates
loglink:	Links DESTDIR/data/log to SRCDIR/.log/log
more:		Copy every useful file to one big file called "morefil"
list:		Beatify all useful files to one big file called "listing"
clean:		Remove all output files
lclint:		Run "lclint" on all source files
lint:		Run "lint" on all source files
depend:		Update Makefile itself automatically for source dependcies.


-Millis Miller 24th October 2001
