# Process this file with automake to produce Makefile.in
## njudge/Makefile.am
## (C) 2002, Consolidated Braincells Inc.  Released into the public domain
#
# $Log$
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

bin_PROGRAMS= dip rdip pdip size_check cmap summary deddump delgame flock \
	fmtwho ign ded_to_ascii ascii_to_ded zpblind recdump lenlimit \
        ascii2rec

bin_SCRIPTS= atrun dipclean diprun newlogs rundipmap runlistmap smail makeflist

# all headers except defaults.h this one has a special make rule below

noinst_HEADERS=	conf.h  \
		config.h \
		dip.h   \
		diplog.h   \
		functions.h \
		hashtable.h \
		mach.h \
		mail.h \
		ml_signon.h  \
		plyrdata.h \
		porder.h \
		port.h \
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

ign_SOURCES=ign.c \
		strcasecmp.c
ign.o: ign.c
	${CC} ${DEFS} ${INCLUDES} ${AM_CFLAGS} ${CFLAGS} -DMAIL_PROG=\"${MAIL}\" -c $<

pdip_SOURCES=	pdip.c

rdip_SOURCES=	rdip.c \
		diplog.c \
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

dist_data_DATA=	data/*

dist_doc_DATA=	docs/InstallationHOWTO \
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
		docs/njudgemapdatahowto.htm \
		docs/njudgemapdatahowto.txt \
		AUTHORS \
		COPYING \
		ChangeLog \
		README

upgrade: defaults.h ${bin_PROGRAMS}
	$(mkinstalldirs) ${DESTDIR}${bindir}
	@for i in ${bin_PROGRAMS}; \
	do ${INSTALL_PROGRAM} $$i ${DESTDIR}$(bindir)/$$i; \
	done;

upgrade-strip: defaults.h ${bin_PROGRAMS}
	$(mkinstalldirs) ${DESTDIR}${bindir}
	@for i in ${bin_PROGRAMS}; \
	do ${INSTALL_PROGRAM} -s $$i ${DESTDIR}$(bindir)/$$i; \
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

remap: ${DESTDIR}${datadir}
	./cmap ${DESTDIR}${datadir} >> ${DESTDIR}${datadir}/install.log

install: install-check install-am

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

install-data-hook: flist magic
	$(mkinstalldirs) ${DESTDIR}$(datadir)
	./cmap ${DESTDIR}${datadir} >> ${DESTDIR}${datadir}/install.log
	${INSTALL_DATA} dip.conf ${DESTDIR}${bindir}/dip.conf
	touch ${DESTDIR}${bindir}/dip.msg
	touch ${DESTDIR}${bindir}/dip.addr
	touch ${DESTDIR}${bindir}/dip.whois
	touch ${DESTDIR}${bindir}/dip.blist
	${LN_S} -f ../dip.whois ${DESTDIR}${datadir}/whois
	$(mkinstalldirs) ${DESTDIR}${bindir}/Dcontrol
	@echo "control   001       S1801MX   1 1 1 0 2 18 7" > ${DESTDIR}${bindir}/dip.master
	@echo "Process   " `date '+%a %b %d %X %Y' --date '1 year'` >> ${DESTDIR}${bindir}/dip.master
	@echo "Start     " `date '+%a %b %d %X %Y'` >> ${DESTDIR}${bindir}/dip.master
	@echo "-" >> ${DESTDIR}${bindir}/dip.master
	@echo "\"| ${bindir}/rdip -b -d ${bindir} \"" > ${DESTDIR}${bindir}/${USER}-forward
	-@chown ${USER} ${DESTDIR}${bindir}
	-@chown ${USER} ${DESTDIR}${bindir}/Dcontrol
	-@chown ${USER} ${DESTDIR}${datadir}
	-@chown ${USER} ${DESTDIR}${bindir}/*
	-@chown ${USER} ${DESTDIR}${datadir}/*
	@echo 
	@echo "you must now edit ${bindir}/dip.conf and "
	@echo "${bindir}/dip.msg (which last may be left empty)"
	@echo "To start the judge, move ${bindir}/${USER}-forward to ${HOME}/../${USER}/.forward"
	@echo

# smail.in and dipclean.in are  already automatically included as are 
# top-level docs like COPYING etc.
distclean-local:
	-rm -rf autom4te.cache

EXTRA_DIST= config.h.in defaults.inc.in dip.conf dip.template starter.flist \
	debian njudge.spec $(man_MANS) $(bin_SCRIPTS)

CLEANFILES=flist.info flist
DISTCLEANFILES=defaults.inc

