# Diplomacy Adjudicator.
#
# $Log$
# Revision 1.24  2002/04/15 12:55:40  miller
# Multiple changes for blind & Colonial & setup from USTV
#
# Revision 1.23  2002/04/15 10:55:14  miller
# Added size_check to dependencies
#
# Revision 1.22  2002/04/12 18:20:50  miller
# Fixed small error
#
# Revision 1.21  2002/04/12 12:10:01  miller
# Added blind and check executables, removed bgreet
#
# Revision 1.20  2002/04/12 01:26:41  miller
# Better attempt for HOME directory
#
# Revision 1.19  2002/02/18 20:18:14  miller
# Include document subdirectory
#
# Revision 1.18  2002/01/06 22:49:45  miller
# depend target now addes Makefile dependency
#
# Revision 1.17  2002/01/05 17:09:36  miller
# Moved version to another file (Makefile.version) and removed mapit
#
# Revision 1.16  2001/12/29 20:38:03  nzmb
#
# Added infoplayer, record commands. Put judge version to 1.0.0 as we think it is
# stable.
#
# Revision 1.15  2001/10/20 12:11:10  miller
# Merged in changes from DEMA and USTV 
#
# Revision 1.14.2.2  2001/10/20 00:51:07  dedo
# Added INSTALLCMD default
#
# Revision 1.14.2.1  2001/10/15 00:25:25  ustv
# Check before installing to prevent judge over-write
#
#
#  Copyright 1987, Lowe.
#
#  Diplomacy is a trademark of the Avalon Hill Game Company, Baltimore,
#  Maryland, all rights reserved; used with permission.
#
#  Redistribution and use in source and binary forms are permitted
#  provided that it is for non-profit purposes, that this and the 
#  above notices are preserved and that due credit is given to Mr.
#  Lowe.

#Include installation-specific settings
# A macro similar to this will install base settings
#	@if [ ! -f Makefile.defines ]; then \
#		cp Makefile.defines.base Makefile.defines
#		echo "Default Makefile.defines created - please check." ; \
#	fi;

#default, over-ridden by user Makefile.defines if needed
INSTALLCMD=install
include Makefile.defines

#set the judge version
#Now in a separate file
include Makefile.version

# Programs, Sources, and Objects.

SRCS = 	assign.c \
		bailout.c \
		common.c \
		conf.c \
		dip.c \
		dipent.c \
		diplog.c \
		dipstats.c \
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
		st_setup.c \
		st_movement.c \
		st_porder.c \
		st_retreat.c \
		st_status.c \
		strcasecmp.c \
		strdup.c \
		users.c \
		variant.c \
		version.c

EXTRAS = size_check.c cmap.c summary.c deddump.c delgame.c \
	 ascii_to_ded.c ded_to_ascii.c flock.c fmtwho.c ign.c pdip.c recdump.c \
	 rdip.c zpblind.c zparseb.c

OBJS = $(SRCS:%.c=%.o)

FILES=	${SRCDIR}/Makefile ${SRCDIR}/Makefile.version ${SRCDIR}/README ${SRCDIR}/dip.template \
	${SRCDIR}/*.h ${SRCDIR}/*.c ${SRCDIR}/makedata ${SRCDIR}/newvers \
	${SRCDIR}/diprun ${SRCDIR}/smail ${SRCDIR}/dipclean ${SRCDIR}/atrun \
	${SRCDIR}/rundipmap ${SRCDIR}/runlistmap ${SRCDIR}/newlogs \
	${SRCDIR}/data/* ${SRCDIR}/starter.flist \
	${SRCDIR}/rundipmap ${SRCDIR}/docs/* ${SRCDIR}/dip.conf ${SRCDIR}/defaults.inc.base \
	${SRCDIR}/smail ${SRCDIR}/Makefile.defines.base

SIZE_FILE= ${DESTDIR}/.size.dat

.SUFFIXES: .c .o .h

.c.o:
	${CC} ${CFLAGS} ${INCPATH} -c $*.c

all:    dip rdip pdip size_check cmap summary deddump delgame flock \
	fmtwho ign ded_to_ascii ascii_to_ded zpblind

dip: ${OBJS}
	${CC} ${LDFLAGS} -o dip ${OBJS} ${LIBS}

size_check: size_check.o
	${CC} ${LDFLAGS} -o size_check size_check.o ${LIBS}

cmap: jm.o cmap.o lib.o bailout.o conf.o strdup.o variant.o hashtable.o global.o diplog.o
	${CC} ${LDFLAGS} -o cmap jm.o cmap.o lib.o bailout.o conf.o strdup.o diplog.o \
	                    variant.o hashtable.o global.o ${LIBS}

summary: summary.o dipent.o diplog.o jm.o lib.o params.o po_get.o bailout.o conf.o variant.o hashtable.o global.o strdup.o
	${CC} ${LDFLAGS} -o summary summary.o dipent.o diplog.o jm.o lib.o hashtable.o \
						params.o po_get.o bailout.o conf.o strdup.o variant.o \
						global.o ${LIBS}

deddump: deddump.o conf.o strdup.o hashtable.o global.o
	${CC} ${LDFLAGS} -o deddump deddump.o conf.o strdup.o hashtable.o global.o ${LIBS}

recdump: recdump.o plyrdata.o
	${CC} ${LDFLAGS} -o recdump recdump.o plyrdata.o ${LIBS}

delgame: delgame.o
	${CC} ${LDFLAGS} -o delgame delgame.o ${LIBS}

flock: flock.o
	${CC} ${LDFLAGS} -o flock flock.o ${LIBS}

ascii_to_ded: ascii_to_ded.o
	${CC} ${LDFLAGS} -o ascii_to_ded ascii_to_ded.o ${LIBS}

ded_to_ascii: ded_to_ascii.o
	${CC} ${LDFLAGS} -o ded_to_ascii ded_to_ascii.o ${LIBS}

fmtwho: fmtwho.o
	${CC} ${LDFLAGS} -o fmtwho fmtwho.o ${LIBS}

ign: ign.o strcasecmp.o
	${CC} ${LDFLAGS} -o ign ign.o strcasecmp.o ${LIBS}

pdip: pdip.o
	${CC} ${LDFLAGS} -o pdip pdip.o ${LIBS}

rdip: rdip.o diplog.o bailout.o conf.o strdup.o hashtable.o global.o
	${CC} ${LDFLAGS} -o rdip rdip.o diplog.o bailout.o conf.o strdup.o hashtable.o global.o ${LIBS}

zpblind: zpblind.o zparseb.o
	${CC} ${LDFLAGS} -o zpblind zpblind.o zparseb.o ${LIBS}


defaults.inc:
	@echo "Generating defaults.inc file from base version."
	@echo "You should change this if you wish a different policy for your judge."
	@echo
	@cp -p defaults.inc.base defaults.inc

Datamake: data/*
	./makedata ${INSTALLDIR} > Datamake

install: ${INSTALLDIR} ${DESTDIR} ${INSTALLDIR}/data dip ${INSTALLDIR}/data/flist \
	 ${INSTALLDIR}/diprun ${INSTALLDIR}/smail ${INSTALLDIR}/dipclean ${INSTALLDIR}/atrun \
	 ${INSTALLDIR}/rundipmap ${INSTALLDIR}/runlistmap ${INSTALLDIR}/newlogs cmap \
	 summary deddump delgame flock fmtwho ign pdip rdip recdump magic \
	 size_check defaults.inc ascii_to_ded ded_to_ascii zpblind
	@if [ -f ${INSTALLDIR}/dip.master ]; then \
	    echo; echo "Error: ${INSTALLDIR}/dip.master exists." ;echo; \
	    echo "Cannot install over existing judge."; \
	    echo "Perhaps you meant to do 'make upgrade'."; echo; echo; \
	    exit 1; \
	fi;
	-rm -f ${SIZE_FILE}
	./size_check ${SIZE_FILE}
	${INSTALLCMD} ${INSFLAGS} dip ${INSTALLDIR}/newprg
	mv ${INSTALLDIR}/newprg ${INSTALLDIR}/dip
	${INSTALLCMD} ${INSFLAGS} summary ${INSTALLDIR}/summary
	${INSTALLCMD} ${INSFLAGS} deddump ${INSTALLDIR}/deddump
	${INSTALLCMD} ${INSFLAGS} delgame ${INSTALLDIR}/delgame
	${INSTALLCMD} ${INSFLAGS} flock ${INSTALLDIR}/flock
	${INSTALLCMD} ${INSFLAGS} fmtwho ${INSTALLDIR}/fmtwho
	${INSTALLCMD} ${INSFLAGS} ign ${INSTALLDIR}/ign
	${INSTALLCMD} ${INSFLAGS} pdip ${INSTALLDIR}/pdip
	${INSTALLCMD} ${INSFLAGS} rdip ${INSTALLDIR}/rdip
	${INSTALLCMD} ${INSFLAGS} recdump ${INSTALLDIR}/recdump
	${INSTALLCMD} ${INSFLAGS} zpblind ${INSTALLDIR}/zpblind
	${INSTALLCMD} ${INSFLAGS} ascii_to_ded ${INSTALLDIR}/ascii_to_ded
	${INSTALLCMD} ${INSFLAGS} ded_to_ascii ${INSTALLDIR}/ded_to_ascii
	@if [ ${DESTDIR} != ${INSTALLDIR} ] ; then \
		ln -f -s ${INSTALLDIR}/rdip ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/dip ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/summary ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/deddump ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/recdump ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/flock ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/fmtwho ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/ign ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/pdip ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/delgame ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/atrun ${DESTDIR}; \
		ln -f -s ${INSTALLDIR}/smail ${DESTDIR}; \
	fi;
	-rm -f data/RCS
	-cp -p data/* ${INSTALLDIR}/data
#FIXME
#	@if [ ${DESTDIR} != ${INSTALLDIR} ] ; then \
#	-@ln -f -s  ${INSTALLDIR}/data ${DESTDIR} \
#	fi; 
	./cmap ${INSTALLDIR} >> ${INSTALLDIR}/install.log
#FIXME
#	@if [ ${DESTDIR} != ${INSTALLDIR} ] ; then \
#	@-ln -f -s  ${INSTALLDIR}/install.log ${DESTDIR} \  
#	fi;
	cp dip.conf $(DESTDIR)/dip.conf
	touch ${DESTDIR}/dip.msg
	@chmod 640 ${DESTDIR}/dip.msg
	touch ${DESTDIR}/dip.addr
	@chmod 640 ${DESTDIR}/dip.addr
	touch ${DESTDIR}/dip.whois
	@chmod 640 ${DESTDIR}/dip.whois
	-mkdir ${DESTDIR}
	-mkdir ${DESTDIR}/Dcontrol
	-chmod 700 ${DESTDIR}
	-chown $(USER) ${DESTDIR}
	-chown $(USER) ${DESTDIR}/Dcontrol
	@echo "control   001       S1801MX   1 1 1 0 2 18 7" > ${DESTDIR}/dip.master
	@echo "Process   " `date '+%a %b %d %X %Y' --date '1 year'` >> ${DESTDIR}/dip.master
	@echo "Start     " `date '+%a %b %d %X %Y'` >> ${DESTDIR}/dip.master
	@echo "-" >> ${DESTDIR}/dip.master
	@echo ",\"| ${DESTDIR}/rdip -b -d ${DESTDIR} \"" > ${HOME}/../${USER}/forward
	-@chown ${USER} ${DESTDIR}/*
	@echo
	@echo "you must now edit ${INSTALLDIR}/smail, ${INSTALLDIR}/atrun,"
	@echo "${DESTDIR}/dip.conf and ${DESTDIR}/dip.msg (which last may be left empty)"
	@echo "To start the judge, rename ${HOME}/../${USER}/forward to .forward"
	@echo


remap: cmap
	@make -f Datamake
	./cmap ${INSTALLDIR} >> ${INSTALLDIR}/install.log
#	@if [ ${DESTDIR} != ${INSTALLDIR} ] ; then \
#	@-ln -f -s  ${INSTALLDIR}/install.log ${DESTDIR} \
#	fi; 

upgrade: Datamake dip diprun dipclean rundipmap runlistmap fmtwho \
	 remap pdip rdip summary deddump recdump delgame flock ign flist magic \
	 size_check ascii_to_ded ded_to_ascii zpblind
	${INSTALLCMD} ${INSFLAGS} dip ${INSTALLDIR}/newprg
	mv ${INSTALLDIR}/newprg ${INSTALLDIR}/dip
	${INSTALLCMD} ${INSFLAGS} summary ${INSTALLDIR}/summary
	${INSTALLCMD} ${INSFLAGS} zpblind ${INSTALLDIR}/zpblind
	${INSTALLCMD} ${INSFLAGS} deddump ${INSTALLDIR}/deddump
	${INSTALLCMD} ${INSFLAGS} delgame ${INSTALLDIR}/delgame
	${INSTALLCMD} ${INSFLAGS} flock ${INSTALLDIR}/flock
	${INSTALLCMD} ${INSFLAGS} fmtwho ${INSTALLDIR}/fmtwho
	${INSTALLCMD} ${INSFLAGS} ign ${INSTALLDIR}/ign
	${INSTALLCMD} ${INSFLAGS} pdip ${INSTALLDIR}/pdip
	${INSTALLCMD} ${INSFLAGS} rdip ${INSTALLDIR}/rdip
	${INSTALLCMD} ${INSFLAGS} recdump ${INSTALLDIR}/recdump
	${INSTALLCMD} ${INSFLAGS} ascii_to_ded ${INSTALLDIR}/ascii_to_ded
	${INSTALLCMD} ${INSFLAGS} ded_to_ascii ${INSTALLDIR}/ded_to_ascii

magic:
	@if [ -f .magic.h ]; then \
		cat .magic.h  | grep DIE_MAGIC | cut -d' ' -f3 >  ${DESTDIR}/.magic.dat; \
		echo ".magic.dat created from .magic.h" ; \
		rm -f .magic.h; \
	fi;	
	

${DESTDIR}: 
	mkdir ${DESTDIR}
	mkdir ${DESTDIR}/Dcontrol
	cp dip.template ${DESTDIR}/dip.master
	cp /dev/null    ${DESTDIR}/dip.addr
	cp /dev/null    ${DESTDIR}/dip.whois
	cp /dev/null	${DESTDIR}/dip.blist
	chmod 700 ${DESTDIR}
	-chown $(USER) ${DESTDIR}
	-chown $(USER) ${DESTDIR}/Dcontrol

${DESTDIR}/data: 
	mkdir ${DESTDIR}/data
	ln -f -s ../dip.whois ${DESTDIR}/data/whois
	chmod 700 ${DESTDIR}/data
	-chown $(USER) ${DESTDIR}/data

 ${INSTALLDIR}/data:
 	mkdir ${INSTALLDIR}/data

${INSTALLDIR}:
	mkdir ${INSTALLDIR}

flist.info:  starter.flist
	cp starter.flist flist.info

# flist: Available file list.  
#
#   1) First generate a header on temp.flist1
#   2) Generate a list of all files in the format  "name size date +"
#   3) Sort and merge with the flist.info which is "name %	   = comment"
#   4) Edit the resulting file to:
#	a) Insert a # at the beginning of each comment line (those with an =).
#	b) Join the comment lines to the end of the preceeding lines.
#	c) Replace the first # with a %.
#	d) Delete anything after a second # (represents comment with no file)
#	e) Delete the text from the + to the =.
#	f) Delete any extra + signs (files without comments).
#	g) Delete any entries with a comment of NOLIST.
#   5) Merge the header and the resulting file list.

flist: ${INSTALLDIR}/data/flist starter.flist 


${INSTALLDIR}/data/flist: ${INSTALLDIR}/data Datamake flist.info
	@make -f Datamake
	rm -f ${INSTALLDIR}/data/flist.info
	cp flist.info ${INSTALLDIR}/data
	echo >  temp.flist1 "Files and sizes in bytes as of `date`"
	echo >> temp.flist1 ""
	echo >> temp.flist1 \
"Name                   Size  Last Change    Description"
	echo >> temp.flist1 \
"------------------   ------ -------------   ---------------------------------"
	(cd ${INSTALLDIR}/data; \
	  /bin/ls -lL | grep -v "^total" | \
	  awk '{printf("%-20.20s%7d %3s %3s %5s  +\n",$$9,$$5,$$6,$$7,$$8)}' \
	) | sort - flist.info > temp.flist2
	(echo g/=/s/'^'/'#'/;           \
	 echo g/=/.-1,.j;               \
	 echo g/'#'/s/'#'/'\%'/;        \
	 echo g/'#'/s/'#'.'*'//;        \
	 echo g/=/s/+.'*'=//;           \
	 echo g/+/s/+//;                \
	 echo g/NOLIST/d;               \
	 echo w) | \
	  ed  - temp.flist2
	-rm ${INSTALLDIR}/data/flist
	cat temp.flist1 temp.flist2 > ${INSTALLDIR}/data/flist
	rm  temp.flist1 temp.flist2

${INSTALLDIR}/dip: dip
	${INSTALLCMD} ${INSFLAGS} dip ${INSTALLDIR}/newprg
	mv ${INSTALLDIR}/newprg ${INSTALLDIR}/dip

${INSTALLDIR}/diprun: diprun
	cp diprun ${INSTALLDIR}/diprun

${INSTALLDIR}/smail: smail
	cp smail ${INSTALLDIR}/smail

${INSTALLDIR}/dipclean: dipclean
	cp dipclean ${INSTALLDIR}/dipclean

${INSTALLDIR}/atrun: atrun
	cp atrun ${INSTALLDIR}/atrun

${INSTALLDIR}/rundipmap: rundipmap
	cp rundipmap ${INSTALLDIR}/rundipmap

${INSTALLDIR}/runlistmap: runlistmap
	cp runlistmap ${INSTALLDIR}/runlistmap

${INSTALLDIR}/newlogs: newlogs
	cp newlogs ${INSTALLDIR}/newlogs

dist: tarZ targz tarbz2

tarZ: ../njudge-${JVERSION}.tar.Z 

targz: ../njudge-${JVERSION}.tar.gz 

tarbz2: ../njudge-${JVERSION}.tar.bz2 

tar: ../njudge-${JVERSION}.tar 

../njudge-${JVERSION}.tar: ${FILES} 
#	newvers VERMAJ
#	@touch .log/oldlog
	rm -f ../njudge-${JVERSION}.tar
#	tar --exclude data/CVS/ -cf ../njudge-${JVERSION}.tar ${FILES}
#	tar --exclude ../njudge/CVS ../njudge/data/CVS ../njudge/docs/CVS -cf ../njudge-${JVERSION}.tar ../njudge/

#	tar --exclude ../njudge/CVS \
#	    --exclude ../njudge/data/CVS \
#	    --exclude ../njudge/docs/CVS \
#	    -cf ../njudge-${JVERSION}.tar ../njudge/

	cd ..;\
	tar -X njudge/xfile \
	    -cf njudge-${JVERSION}.tar njudge/


../njudge-${JVERSION}.tar.Z: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	compress -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar

../njudge-${JVERSION}.tar.gz: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	gzip -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar

../njudge-${JVERSION}.tar.bz2: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	bzip2 -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar


loglink: 
	rm -f ${DESTDIR}/data/log
	ln -f -s `pwd`/data/log ${DESTDIR}/data/log

more: 
	rm -f morefil
	(more ${FILES} > morefil)

list: 
	rm -f listing
	-for i in ${FILES}; do \
		expand $$i | pr -h $$i | sed 's/^/       /' >> listing; \
	done

clean: 
	rm -f a.out core dip Datamake *.o *~
	rm -f cmap fmtwho pdip rdip summary
	rm -f deddump delgame flock ign makedep eddep recdump
	rm -f size_check zpblind ascii_to_ded ded_to_ascii	
lclint:
	lint ${SRCS}
lint: 
	lint ${SRCS}

depend: 
	${CC} -MM ${INCPATH} ${SRCS} ${EXTRAS} > makedep 
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo ',s/version.o\:/version.o\: Makefile.version' > edv
	echo ',s/\:/\: Makefile Makefile.defines/g' >> edv
	echo 'w' >> edv
	ed - makedep < edv
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm edv eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
assign.o: Makefile Makefile.defines assign.c dip.h conf.h port.h variant.h
bailout.o: Makefile Makefile.defines bailout.c dip.h conf.h port.h variant.h diplog.h
common.o: Makefile Makefile.defines common.c dip.h conf.h port.h variant.h porder.h
conf.o: Makefile Makefile.defines conf.c conf.h hashtable.h
dip.o: Makefile Makefile.defines dip.c dip.h conf.h port.h variant.h mail.h functions.h diplog.h \
 plyrdata.h
dipent.o: Makefile Makefile.defines dipent.c dip.h conf.h port.h variant.h defaults.h \
 defaults.inc functions.h diplog.h
diplog.o: Makefile Makefile.defines diplog.c diplog.h functions.h dip.h conf.h port.h variant.h
dipstats.o: Makefile Makefile.defines dipstats.c conf.h dipstats.h
draw.o: Makefile Makefile.defines draw.c dip.h conf.h port.h variant.h functions.h mail.h
global.o: Makefile Makefile.defines global.c dip.h conf.h port.h variant.h mach.h porder.h
hashtable.o: Makefile Makefile.defines hashtable.c hashtable.h
history.o: Makefile Makefile.defines history.c dip.h conf.h port.h variant.h functions.h mail.h
jm.o: Makefile Makefile.defines jm.c functions.h dip.h conf.h port.h variant.h
lib.o: Makefile Makefile.defines lib.c dip.h conf.h port.h variant.h functions.h porder.h mail.h
ma_build.o: Makefile Makefile.defines ma_build.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h
ma_build_basic.o: Makefile Makefile.defines ma_build_basic.c dip.h conf.h port.h variant.h \
 functions.h porder.h mach.h
ma_expenses.o: Makefile Makefile.defines ma_expenses.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_famplag.o: Makefile Makefile.defines ma_famplag.c dip.h conf.h port.h variant.h mail.h \
 porder.h mach.h functions.h
ma_movement.o: Makefile Makefile.defines ma_movement.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_porder.o: Makefile Makefile.defines ma_porder.c dip.h conf.h port.h variant.h mail.h porder.h \
 mach.h functions.h
ma_retreat.o: Makefile Makefile.defines ma_retreat.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_stats.o: Makefile Makefile.defines ma_stats.c functions.h dip.h conf.h port.h variant.h \
 mail.h porder.h mach.h
machlib.o: Makefile Makefile.defines machlib.c dip.h conf.h port.h variant.h porder.h mach.h
mail.o: Makefile Makefile.defines mail.c dip.h conf.h port.h variant.h mail.h functions.h \
 dipstats.h diplog.h plyrdata.h
ml_short.o: Makefile Makefile.defines ml_short.c dip.h conf.h port.h variant.h mail.h \
 functions.h porder.h
mfprintf.o: Makefile Makefile.defines mfprintf.c mail.h variant.h
ml_date.o: Makefile Makefile.defines ml_date.c port.h
ml_getaddr.o: Makefile Makefile.defines ml_getaddr.c dip.h conf.h port.h variant.h mail.h \
 functions.h
ml_list.o: Makefile Makefile.defines ml_list.c functions.h dip.h conf.h port.h variant.h mail.h
ml_press.o: Makefile Makefile.defines ml_press.c dip.h conf.h port.h variant.h mail.h \
 functions.h
ml_set.o: Makefile Makefile.defines ml_set.c dip.h conf.h port.h variant.h mail.h functions.h \
 dipstats.h diplog.h plyrdata.h
ml_signon.o: Makefile Makefile.defines ml_signon.c dip.h conf.h port.h variant.h mail.h \
 ml_signon.h functions.h dipstats.h plyrdata.h
params.o: Makefile Makefile.defines params.c dip.h conf.h port.h variant.h functions.h
plyrdata.o: Makefile Makefile.defines plyrdata.c plyrdata.h
phase.o: Makefile Makefile.defines phase.c dip.h conf.h port.h variant.h porder.h functions.h
po_condition.o: Makefile Makefile.defines po_condition.c dip.h conf.h port.h variant.h \
 functions.h porder.h
po_errmsg.o: Makefile Makefile.defines po_errmsg.c dip.h conf.h port.h variant.h functions.h \
 porder.h
po_get.o: Makefile Makefile.defines po_get.c dip.h conf.h port.h variant.h functions.h porder.h
po_init.o: Makefile Makefile.defines po_init.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h
po_mastrpt.o: Makefile Makefile.defines po_mastrpt.c dip.h conf.h port.h variant.h
porder.o: Makefile Makefile.defines porder.c dip.h conf.h port.h variant.h porder.h functions.h
st_build.o: Makefile Makefile.defines st_build.c dip.h conf.h port.h variant.h functions.h \
 porder.h
st_setup.o: Makefile Makefile.defines st_setup.c dip.h conf.h port.h variant.h functions.h \
 porder.h
st_movement.o: Makefile Makefile.defines st_movement.c functions.h dip.h conf.h port.h variant.h \
 porder.h
st_porder.o: Makefile Makefile.defines st_porder.c dip.h conf.h port.h variant.h functions.h \
 mail.h porder.h
st_retreat.o: Makefile Makefile.defines st_retreat.c dip.h conf.h port.h variant.h functions.h \
 porder.h
st_status.o: Makefile Makefile.defines st_status.c dip.h conf.h port.h variant.h functions.h \
 porder.h
strcasecmp.o: Makefile Makefile.defines strcasecmp.c
strdup.o: Makefile Makefile.defines strdup.c
users.o: Makefile Makefile.defines users.c dip.h conf.h port.h variant.h mail.h functions.h \
 plyrdata.h
variant.o: Makefile Makefile.defines variant.c dip.h conf.h port.h variant.h
version.o: Makefile Makefile.defines Makefile.version version.c dip.h conf.h port.h variant.h functions.h
size_check.o: Makefile Makefile.defines size_check.c
cmap.o: Makefile Makefile.defines cmap.c dip.h conf.h port.h variant.h functions.h porder.h \
 mach.h
summary.o: Makefile Makefile.defines summary.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h diplog.h
deddump.o: Makefile Makefile.defines deddump.c dip.h conf.h port.h variant.h
delgame.o: Makefile Makefile.defines delgame.c port.h dip.h conf.h variant.h
ascii_to_ded.o: Makefile Makefile.defines ascii_to_ded.c dip.h conf.h port.h variant.h
ded_to_ascii.o: Makefile Makefile.defines ded_to_ascii.c dip.h conf.h port.h variant.h
flock.o: Makefile Makefile.defines flock.c port.h
fmtwho.o: Makefile Makefile.defines fmtwho.c functions.h dip.h conf.h port.h variant.h
ign.o: Makefile Makefile.defines ign.c
pdip.o: Makefile Makefile.defines pdip.c
recdump.o: Makefile Makefile.defines recdump.c plyrdata.h
rdip.o: Makefile Makefile.defines rdip.c functions.h dip.h conf.h port.h variant.h diplog.h
zpblind.o: Makefile Makefile.defines zpblind.c zparseb.h zmacro.h
zparseb.o: Makefile Makefile.defines zparseb.c zparseb.h zmacro.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
