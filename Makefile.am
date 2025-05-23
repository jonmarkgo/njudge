# Process this file with automake to produce Makefile.in
## njudge/Makefile.am
## (C) 2002, Consolidated Braincells Inc.  Released into the public domain
#
# $Log$
# Revision 1.41  2004/09/17 19:15:28  nzmb
# Fix bug #351.
#
# Revision 1.40  2004/08/18 09:58:59  millis
# Change name of document to njudgeMapFileHOWTO
#
# Revision 1.39  2004/08/17 14:53:25  jaldhar
# Removed GNUism in date command for better portability.  Fixes bug 351.
#
# Revision 1.38  2004/05/22 08:39:57  millis
# Bug 297: Add Intimate Diplomacy
#
# Revision 1.37  2004/05/11 20:10:24  jaldhar
# Fixed duplication of datadir in upgrade-data target of Makefile.am.
# Other automake files regenerated in the process.
#
# Revision 1.36  2004/05/10 14:21:51  jaldhar
# Fixed bug 215 by ensuring that the data files were copied before we
# tried to run cmap.  Automake files regenerated in the process.
#
# Revision 1.35  2004/01/02 10:00:16  millis
# no message
#
# Revision 1.34  2003/11/18 14:42:00  millis
# Added manual.htm (Russell Blau's DJUM)
#
# Revision 1.33  2003/09/09 18:51:22  jaldhar
# Got rid of port.h and replaced with some extra configure checks.  The
# include strings.h was not carried over because it is commented out and
# likely wouldn't work anyway.
#
# Revision 1.32  2003/07/23 21:47:07  millis
# Remove delete of map files from remap target.
#
# Revision 1.31  2003/07/23 06:00:37  jaldhar
# Force calls to date in Makefile.am to use the 'C' locale.  (Fixes Bug 138)
# auxilliary files regenerated to match this change.
#
# Revision 1.30  2003/07/23 05:36:43  jaldhar
# Autogenerate dip.conf (fixes bug 154)
#
# Revision 1.29  2003/07/23 05:05:09  jaldhar
# Remake map on upgrades (Closes bug 175) and some rearrangement of targets.
#
# Revision 1.28  2003/07/19 13:13:57  millis
# Bug 196, remove pdip
#
# Revision 1.27  2003/07/16 15:04:34  millis
# Remove ign program
#
# Revision 1.26  2003/06/10 10:33:30  millis
# Bug174 fix, delete the map files first
#
# Revision 1.25  2003/05/11 16:15:16  millis
# Fixed bug 153 (make not upgrading data files correctly)
#
# Revision 1.24  2003/05/04 05:33:20  jaldhar
# Rolled back to automake 1.6/autoconf 2.50
#
# Revision 1.23  2003/04/30 00:02:00  millis
# Added use of dip.footer
#
# Revision 1.22  2003/03/29 18:20:28  christ
# Added VariantsHOWTO* to Makefile.am
#
# Revision 1.21  2003/03/29 17:55:20  christ
# Added InstallationHOWTO.* in Makefike.*.
#
# Revision 1.20  2003/03/22 09:22:57  millis
# Added plyrdata.c to ascii2rec sources
#
# Revision 1.19  2003/03/12 07:44:38  nzmb
# Fix to try to get ascii2rec.c to link correctly.
#
# Revision 1.18  2003/03/05 02:57:54  nzmb
# Small error in previous commit fixed.
#
# Revision 1.17  2003/03/05 02:42:08  nzmb
# Added ascii2rec which will convert a text plyrdata record dump (generated
# by recdump) into a binary plyrdata file.
#
# Revision 1.16  2003/02/26 16:57:24  millis
# Removed README.statistics and added README.timezone
#
# Revision 1.15  2003/02/17 12:41:40  millis
# Fixed Bug 108, make lines >=1024 terminate in '\n'
#
# Revision 1.14  2003/01/14 14:05:50  millis
# Added history
#
#

AUTOMAKE_OPTIONS=foreign dist-zip

# set the judge user
USER=@USER@

# directories
bindir=@DIR@
datadir=@DIR@/data
docdir=@DIR@
mandir=@DIR@

# compiler/linker options
AM_CFLAGS = -pipe -Wall 

# locations of some support programs
HEAD=@HEAD@
MAIL=@_MAIL_@
AWK=@AWK@
LN_S=@LN_S@
PERL=@PERL@

bin_PROGRAMS= dip rdip size_check cmap summary deddump delgame flock \
	fmtwho ded_to_ascii ascii_to_ded zpblind recdump lenlimit \
        ascii2rec

bin_SCRIPTS= atrun dipclean diprun newlogs rundipmap runlistmap smail makeflist

# all headers except defaults.h this one has a special make rule below

noinst_HEADERS=	acconfig.h \
		conf.h  \
		config.h \
		dip.h   \
		diplog.h   \
		diptime.h  \
		functions.h \
		hashtable.h \
		mach.h \
		mail.h \
		ml_signon.h  \
		plyrdata.h \
		porder.h \
		variant.h \
		zmacro.h \
		zparseb.h

BUILT_SOURCES= defaults.inc

dip_SOURCES=	assign.c \
		bailout.c \
		common.c \
		conf.c \
		dip.c \
		dipent.c \
		diplog.c \
		diptime.c \
		draw.c \
		global.c \
		hashtable.c \
		history.c \
		jm.c \
		lib.c \
		ma_build.c \
		ma_build_basic.c \
		ma_expenses.c \
		ma_famplag.c \
		ma_movement.c \
		ma_porder.c \
		ma_retreat.c \
		ma_stats.c \
		machlib.c \
		mail.c \
		ml_short.c \
		mfprintf.c \
		ml_date.c \
		ml_getaddr.c \
		ml_list.c \
		ml_press.c \
		ml_set.c \
		ml_signon.c \
		params.c \
		plyrdata.c \
		phase.c \
		po_condition.c \
		po_errmsg.c \
		po_get.c \
		po_init.c \
		po_mastrpt.c \
		porder.c \
		st_bid.c \
		st_build.c \
		st_movement.c \
		st_porder.c \
		st_retreat.c \
		st_setup.c \
		st_status.c \
		strcasecmp.c \
		strdup.c \
		tm_xpress.c \
		users.c \
		variant.c \
		version.c \
		defaults.h

ascii_to_ded_SOURCES=	ascii_to_ded.c \
			plyrdata.c

ascii2rec_SOURCES=   ascii2rec.c plyrdata.c

cmap_SOURCES=	cmap.c \
		diptime.c \
		jm.c \
		lib.c \
		bailout.c \
		conf.c \
		strdup.c \
		variant.c \
		hashtable.c \
		global.c \
		diplog.c 

lenlimit_SOURCES = lenlimit.c

ded_to_ascii_SOURCES=	ded_to_ascii.c

deddump_SOURCES=deddump.c \
		conf.c \
		strdup.c \
		hashtable.c \
		global.c

delgame_SOURCES=delgame.c

flock_SOURCES=	flock.c

fmtwho_SOURCES=	fmtwho.c

rdip_SOURCES=	rdip.c \
		diplog.c \
		diptime.c \
		bailout.c \
		conf.c \
		strdup.c \
		hashtable.c \
		global.c

recdump_SOURCES=recdump.c \
		plyrdata.c

size_check_SOURCES=size_check.c

summary_SOURCES=summary.c \
		dipent.c \
		diplog.c \
		diptime.c \
		jm.c \
		lib.c \
		params.c \
		po_get.c \
		bailout.c \
		conf.c	\
		variant.c \
		hashtable.c \
		global.c \
		strdup.c

zpblind_SOURCES=zpblind.c \
		zparseb.c

man_MANS=	judge.6

dist_data_DATA=data/*

dist_doc_DATA=docs/InstallationHOWTO.txt \
		docs/InstallationHOWTO.html \
		docs/VariantsHOWTO.txt \
		docs/VariantsHOWTO.html \
		docs/README.bailout \
		docs/README.flags \
		docs/README.flist \
		docs/README.hackers \
		docs/README.magic \
		docs/README.master \
		docs/README.plyrdata \
		docs/README.resign \
		docs/README.size \
		docs/README.syslog \
		docs/README.timewarp \
		docs/README.timezone \
		docs/njudgeMapFileHOWTO.htm \
		docs/njudgeMapFileHOWTO.txt \
		docs/manual-all.htm \
		AUTHORS \
		COPYING \
		ChangeLog \
		README

dirs:
	$(mkinstalldirs) ${DESTDIR}${bindir}
	$(mkinstalldirs) ${DESTDIR}$(datadir)

upgrade: upgrade-progs upgrade-data remap
	

upgrade-strip: upgrade-progs-strip upgrade-data remap
	

upgrade-progs: defaults.h ${bin_PROGRAMS} dirs
	@for i in ${bin_PROGRAMS}; \
	do ${INSTALL_PROGRAM} $$i ${DESTDIR}$(bindir)/$$i; \
	done;

upgrade-progs-strip: defaults.h ${bin_PROGRAMS} dirs
	@for i in ${bin_PROGRAMS}; \
	do ${INSTALL_PROGRAM} -s $$i ${DESTDIR}$(bindir)/$$i; \
	done;

upgrade-data: ${dist_data_DATA} dirs
	@for i in ${dist_data_DATA}; \
	do ${INSTALL_DATA} $$i ${DESTDIR}$(datadir)/`basename $$i`; \
	done;

defaults.h: defaults.inc
	
defaults.inc:
	@echo "Generating defaults.inc file from base version."
	@echo "You should change this if you wish a different policy for your judge."
	@echo
	@cp -p defaults.inc.in defaults.inc

flist:
	cp starter.flist flist.info
	${PERL} -w ./makeflist ${DESTDIR}${datadir} > flist
	${INSTALL_DATA} flist ${DESTDIR}${datadir}

magic:
	@if [ -f .magic.h ]; then \
		cat .magic.h  | grep DIE_MAGIC | cut -d' ' -f3 >  ${DESTDIR}${bindir}/.magic.dat; \
		echo ".magic.dat created from .magic.h" ; \
		rm -f .magic.h; \
	fi;

remap: dirs
	./cmap ${DESTDIR}${datadir} >> ${DESTDIR}${datadir}/install.log

install: install-check upgrade-data install-am

install-check:
	@if [ -f ${DESTDIR}$(bindir)/dip.master ]; then \
		echo ; \
		echo "Error: ${DESTDIR}$(bindir)/dip.master exists." ; \
		echo ; \
		echo "Cannot install over existing judge." ; \
		echo "Perhaps you meant to do 'make upgrade?'." ; \
		echo ; \
		echo ; \
		exit 1 ; \
	fi

install-data-hook: upgrade-data dirs flist magic
	./cmap ${DESTDIR}${datadir} >> ${DESTDIR}${datadir}/install.log
	${INSTALL_DATA} dip.conf ${DESTDIR}${bindir}/dip.conf
	touch ${DESTDIR}${bindir}/dip.msg
	touch ${DESTDIR}${bindir}/dip.footer
	touch ${DESTDIR}${bindir}/dip.addr
	touch ${DESTDIR}${bindir}/dip.whois
	touch ${DESTDIR}${bindir}/dip.blist
	${LN_S} -f ../dip.whois ${DESTDIR}${datadir}/whois
	$(mkinstalldirs) ${DESTDIR}${bindir}/Dcontrol
	@echo "control   001       S1801MX   1 1 1 0 2 18 7" > ${DESTDIR}${bindir}/dip.master
	@echo "Process   " `LC_TIME=C date '+%a %b %d %X '` `date '+%Y' | awk '{print 1+$$1}'` >> ${DESTDIR}${bindir}/dip.master
	@echo "Start     " `LC_TIME=C date '+%a %b %d %X %Y'` >> ${DESTDIR}${bindir}/dip.master
	@echo "-" >> ${DESTDIR}${bindir}/dip.master
	@echo "\"| ${bindir}/rdip -b -d ${bindir} \"" > ${DESTDIR}${bindir}/${USER}-forward
	-@chown ${USER} ${DESTDIR}${bindir}
	-@chown ${USER} ${DESTDIR}${bindir}/Dcontrol
	-@chown ${USER} ${DESTDIR}${datadir}
	-@chown ${USER} ${DESTDIR}${bindir}/*
	-@chown ${USER} ${DESTDIR}${datadir}/*
	@echo 
	@echo "you must now edit ${bindir}/dip.conf, ${bindir}/dip.msg "
	@echo "${bindir}/dip.footer (which last two may be left empty)"
	@echo "To start the judge, move ${bindir}/${USER}-forward to ${HOME}/../${USER}/.forward"
	@echo

# smail.in, dip.conf.in, and dipclean.in are  already automatically included as
# are top-level docs like COPYING etc.
distclean-local:
	-rm -rf autom4te.cache

EXTRA_DIST= config.h.in defaults.inc.in dip.template starter.flist \
	debian njudge.spec $(man_MANS) $(bin_SCRIPTS)

CLEANFILES=flist.info flist
DISTCLEANFILES=defaults.inc

