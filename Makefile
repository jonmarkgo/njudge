# Diplomacy Adjudicator.
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


include Makefile.defines

#set the judge version
JVERSION=0.8.10

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
		st_movement.c \
		st_porder.c \
		st_retreat.c \
		st_status.c \
		strcasecmp.c \
		strdup.c \
		users.c \
		variant.c \
		version.c

EXTRAS = cmap.c summary.c bgreet.c deddump.c delgame.c \
         flock.c fmtwho.c ign.c pdip.c recdump.c rdip.c
 
OBJS = $(SRCS:%.c=%.o)

FILES=	${SRCDIR}/Makefile ${SRCDIR}/README ${SRCDIR}/dip.template \
	${SRCDIR}/*.h ${SRCDIR}/*.c ${SRCDIR}/makedata ${SRCDIR}/newvers \
	${SRCDIR}/diprun ${SRCDIR}/smail ${SRCDIR}/dipclean ${SRCDIR}/atrun \
        ${SRCDIR}/rundipmap ${SRCDIR}/runlistmap ${SRCDIR}/newlogs \
	${SRCDIR}/data/* ${SRCDIR}/starter.flist ${SRCDIR}/mapit/* \
	${SRCDIR}/rundipmap ${SRCDIR}/README.* ${SRCDIR}/dip.conf ${SRCDIR}/defaults.inc.base \
	${SRCDIR}/smail ${SRCDIR}/Makefile.defines.base

.SUFFIXES: .c .o .h

.c.o:
	${CC} ${CFLAGS} ${INCPATH} -c $*.c

all:	dip rdip pdip cmap summary bgreet deddump delgame flock fmtwho ign

dip: ${OBJS}
	${CC} ${LDFLAGS} -o dip ${OBJS} ${LIBS}

cmap: jm.o cmap.o lib.o bailout.o conf.o strdup.o variant.o hashtable.o global.o diplog.o
	${CC} ${LDFLAGS} -o cmap jm.o cmap.o lib.o bailout.o conf.o strdup.o diplog.o \
	                    variant.o hashtable.o global.o ${LIBS}

summary: summary.o dipent.o diplog.o jm.o lib.o params.o po_get.o bailout.o conf.o variant.o hashtable.o global.o strdup.o
	${CC} ${LDFLAGS} -o summary summary.o dipent.o diplog.o jm.o lib.o hashtable.o \
						params.o po_get.o bailout.o conf.o strdup.o variant.o \
						global.o ${LIBS}

bgreet: bgreet.o
	${CC} ${LDFLAGS} -o bgreet bgreet.o ${LIBS}

deddump: deddump.o conf.o strdup.o hashtable.o global.o
	${CC} ${LDFLAGS} -o deddump deddump.o conf.o strdup.o hashtable.o global.o ${LIBS}

recdump: recdump.o plyrdata.o
	${CC} ${LDFLAGS} -o recdump recdump.o plyrdata.o ${LIBS}

delgame: delgame.o
	${CC} ${LDFLAGS} -o delgame delgame.o ${LIBS}

flock: flock.o
	${CC} ${LDFLAGS} -o flock flock.o ${LIBS}

fmtwho: fmtwho.o
	${CC} ${LDFLAGS} -o fmtwho fmtwho.o ${LIBS}

ign: ign.o strcasecmp.o
	${CC} ${LDFLAGS} -o ign ign.o strcasecmp.o ${LIBS}

pdip: pdip.o
	${CC} ${LDFLAGS} -o pdip pdip.o ${LIBS}

rdip: rdip.o diplog.o bailout.o conf.o strdup.o hashtable.o global.o
	${CC} ${LDFLAGS} -o rdip rdip.o diplog.o bailout.o conf.o strdup.o hashtable.o global.o ${LIBS}

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
	 summary bgreet deddump delgame flock fmtwho ign pdip rdip recdump magic \
	 defaults.inc
	install ${INSFLAGS} dip ${INSTALLDIR}/newprg
	mv ${INSTALLDIR}/newprg ${INSTALLDIR}/dip
	install ${INSFLAGS} summary ${INSTALLDIR}/summary
	install ${INSFLAGS} bgreet ${INSTALLDIR}/bgreet
	install ${INSFLAGS} deddump ${INSTALLDIR}/deddump
	install ${INSFLAGS} delgame ${INSTALLDIR}/delgame
	install ${INSFLAGS} flock ${INSTALLDIR}/flock
	install ${INSFLAGS} fmtwho ${INSTALLDIR}/fmtwho
	install ${INSFLAGS} ign ${INSTALLDIR}/ign
	install ${INSFLAGS} pdip ${INSTALLDIR}/pdip
	install ${INSFLAGS} rdip ${INSTALLDIR}/rdip
	install ${INSFLAGS} recdump ${INSTALLDIR}/recdump
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
		ln -f -s ${INSTALLDIR}/bgreet ${DESTDIR}; \
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
	@echo ",\"| ${DESTDIR}/rdip -b -d ${DESTDIR} \"" > /home/${USER}/forward
	-@chown ${USER} ${DESTDIR}/*
	@echo
	@echo "you must now edit ${INSTALLDIR}/smail, ${INSTALLDIR}/atrun,"
	@echo "${DESTDIR}/dip.conf and ${DESTDIR}/dip.msg (which last may be left empty)"
	@echo "To start the judge, rename /home/${USER}/forward to .forward"
	@echo

remap: cmap
	./cmap ${INSTALLDIR} >> ${INSTALLDIR}/install.log
#	@if [ ${DESTDIR} != ${INSTALLDIR} ] ; then \
#	@-ln -f -s  ${INSTALLDIR}/install.log ${DESTDIR} \
#	fi; 

upgrade: Datamake dip diprun dipclean rundipmap runlistmap bgreet fmtwho \
	 remap pdip rdip summary deddump recdump delgame flock ign flist magic
	@make -f Datamake
	install ${INSFLAGS} dip ${INSTALLDIR}/newprg
	mv ${INSTALLDIR}/newprg ${INSTALLDIR}/dip
	install ${INSFLAGS} summary ${INSTALLDIR}/summary
	install ${INSFLAGS} bgreet ${INSTALLDIR}/bgreet
	install ${INSFLAGS} deddump ${INSTALLDIR}/deddump
	install ${INSFLAGS} delgame ${INSTALLDIR}/delgame
	install ${INSFLAGS} flock ${INSTALLDIR}/flock
	install ${INSFLAGS} fmtwho ${INSTALLDIR}/fmtwho
	install ${INSFLAGS} ign ${INSTALLDIR}/ign
	install ${INSFLAGS} pdip ${INSTALLDIR}/pdip
	install ${INSFLAGS} rdip ${INSTALLDIR}/rdip
	install ${INSFLAGS} recdump ${INSTALLDIR}/recdump
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
	install ${INSFLAGS} dip ${INSTALLDIR}/newprg
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

tarZ: ../njudge-${JVERSION}.tar.Z ../mapit.tar.Z

targz: ../njudge-${JVERSION}.tar.gz ../mapit.tar.gz

tarbz2: ../njudge-${JVERSION}.tar.bz2 ../mapit.tar.bz2

tar: ../njudge-${JVERSION}.tar ../mapit.tar

../njudge-${JVERSION}.tar: ${FILES} 
#	newvers VERMAJ
#	@touch .log/oldlog
	rm -f ../njudge-${JVERSION}.tar
	tar -cf ../njudge-${JVERSION}.tar ${FILES}

../mapit.tar:
	rm -f ../mapit.tar 
	tar -cf ../mapit.tar mapit

../njudge-${JVERSION}.tar.Z: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	compress -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar

../mapit.tar.Z: ../mapit.tar
	cp -p ../mapit.tar ../mapit.tar.old
	compress -f ../mapit.tar
	mv ../mapit.tar.old ../mapit.tar

../njudge-${JVERSION}.tar.gz: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	gzip -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar

../mapit.tar.gz: ../mapit.tar
	cp -p ../mapit.tar ../mapit.tar.old
	gzip -f ../mapit.tar
	mv ../mapit.tar.old ../mapit.tar

../njudge-${JVERSION}.tar.bz2: ../njudge-${JVERSION}.tar
	cp -p ../njudge-${JVERSION}.tar ../njudge-${JVERSION}.tar.old
	bzip2 -f ../njudge-${JVERSION}.tar
	mv ../njudge-${JVERSION}.tar.old ../njudge-${JVERSION}.tar

../mapit.tar.bz2: ../mapit.tar
	cp -p ../mapit.tar ../mapit.tar.old
	bzip2 -f ../mapit.tar
	mv ../mapit.tar.old ../mapit.tar


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
	rm -f cmap bgreet fmtwho pdip rdip summary
	rm -f deddump delgame flock ign makedep eddep recdump
	
lclint:
	lint ${SRCS}
lint: 
	lint ${SRCS}

depend: 
	${CC} -MM ${INCPATH} ${SRCS} ${EXTRAS} > makedep 
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	# rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
assign.o: assign.c dip.h conf.h port.h variant.h
bailout.o: bailout.c dip.h conf.h port.h variant.h diplog.h
common.o: common.c dip.h conf.h port.h variant.h porder.h
conf.o: conf.c conf.h hashtable.h
dip.o: dip.c dip.h conf.h port.h variant.h mail.h functions.h diplog.h \
 plyrdata.h
dipent.o: dipent.c dip.h conf.h port.h variant.h defaults.h \
 defaults.inc functions.h diplog.h
diplog.o: diplog.c diplog.h functions.h dip.h conf.h port.h variant.h
dipstats.o: dipstats.c conf.h dipstats.h
draw.o: draw.c dip.h conf.h port.h variant.h functions.h mail.h
global.o: global.c dip.h conf.h port.h variant.h mach.h porder.h
hashtable.o: hashtable.c hashtable.h
history.o: history.c dip.h conf.h port.h variant.h functions.h mail.h
jm.o: jm.c functions.h dip.h conf.h port.h variant.h
lib.o: lib.c dip.h conf.h port.h variant.h functions.h porder.h mail.h
ma_build.o: ma_build.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h
ma_build_basic.o: ma_build_basic.c dip.h conf.h port.h variant.h \
 functions.h porder.h
ma_expenses.o: ma_expenses.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_famplag.o: ma_famplag.c dip.h conf.h port.h variant.h mail.h \
 porder.h mach.h functions.h
ma_movement.o: ma_movement.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_porder.o: ma_porder.c dip.h conf.h port.h variant.h mail.h porder.h \
 mach.h functions.h
ma_retreat.o: ma_retreat.c dip.h conf.h port.h variant.h porder.h \
 mach.h functions.h
ma_stats.o: ma_stats.c functions.h dip.h conf.h port.h variant.h \
 mail.h porder.h mach.h
machlib.o: machlib.c dip.h conf.h port.h variant.h porder.h mach.h
mail.o: mail.c dip.h conf.h port.h variant.h mail.h functions.h \
 dipstats.h diplog.h
ml_short.o: ml_short.c dip.h conf.h port.h variant.h mail.h \
 functions.h porder.h
mfprintf.o: mfprintf.c mail.h variant.h
ml_date.o: ml_date.c port.h
ml_getaddr.o: ml_getaddr.c dip.h conf.h port.h variant.h mail.h \
 functions.h
ml_list.o: ml_list.c functions.h dip.h conf.h port.h variant.h mail.h
ml_press.o: ml_press.c dip.h conf.h port.h variant.h mail.h \
 functions.h
ml_set.o: ml_set.c dip.h conf.h port.h variant.h mail.h functions.h \
 dipstats.h diplog.h plyrdata.h
ml_signon.o: ml_signon.c dip.h conf.h port.h variant.h mail.h \
 ml_signon.h functions.h dipstats.h plyrdata.h
params.o: params.c dip.h conf.h port.h variant.h functions.h
plyrdata.o: plyrdata.c plyrdata.h
phase.o: phase.c dip.h conf.h port.h variant.h porder.h functions.h
po_condition.o: po_condition.c dip.h conf.h port.h variant.h \
 functions.h porder.h
po_errmsg.o: po_errmsg.c dip.h conf.h port.h variant.h functions.h \
 porder.h
po_get.o: po_get.c dip.h conf.h port.h variant.h functions.h porder.h
po_init.o: po_init.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h
po_mastrpt.o: po_mastrpt.c dip.h conf.h port.h variant.h
porder.o: porder.c dip.h conf.h port.h variant.h porder.h functions.h
st_build.o: st_build.c dip.h conf.h port.h variant.h functions.h \
 porder.h
st_movement.o: st_movement.c functions.h dip.h conf.h port.h variant.h \
 porder.h
st_porder.o: st_porder.c dip.h conf.h port.h variant.h functions.h \
 mail.h porder.h
st_retreat.o: st_retreat.c dip.h conf.h port.h variant.h functions.h \
 porder.h
st_status.o: st_status.c dip.h conf.h port.h variant.h functions.h \
 porder.h
strcasecmp.o: strcasecmp.c
strdup.o: strdup.c
users.o: users.c dip.h conf.h port.h variant.h mail.h functions.h \
 plyrdata.h
variant.o: variant.c dip.h conf.h port.h variant.h
version.o: version.c dip.h conf.h port.h variant.h functions.h
cmap.o: cmap.c dip.h conf.h port.h variant.h functions.h porder.h \
 mach.h
summary.o: summary.c dip.h conf.h port.h variant.h porder.h mach.h \
 functions.h diplog.h
bgreet.o: bgreet.c dip.h conf.h port.h variant.h functions.h
deddump.o: deddump.c dip.h conf.h port.h variant.h
delgame.o: delgame.c port.h dip.h conf.h variant.h
flock.o: flock.c port.h
fmtwho.o: fmtwho.c functions.h dip.h conf.h port.h variant.h
ign.o: ign.c
pdip.o: pdip.c
recdump.o: recdump.c plyrdata.h
rdip.o: rdip.c functions.h dip.h conf.h port.h variant.h diplog.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
