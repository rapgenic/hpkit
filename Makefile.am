bin_PROGRAMS = hplot hplisten hptalk hprologix #test

hplot_SOURCES = \
	src/common.c	     \
	src/aderror.c        \
	src/adapters.c       \
	src/adapters_lowlevel.c\
	src/hplot.c

hplisten_SOURCES = \
	src/common.c	     \
	src/aderror.c        \
	src/adapters.c       \
	src/adapters_lowlevel.c\
	src/hplisten.c
	
hptalk_SOURCES = \
	src/common.c	     \
	src/aderror.c        \
	src/adapters.c       \
	src/adapters_lowlevel.c\
	src/hptalk.c
	
hprologix_SOURCES = \
	src/common.c	     \
	src/aderror.c        \
	src/adapters.c       \
	src/adapters_lowlevel.c\
	src/hprologix.c
	
#test_SOURCES = \
	src/aderror.c        \
	src/adapters.c       \
	src/adapters_lowlevel.c\
	src/test.c

sysconfdir = $(prefix)/etc/hpkit.d/
sysconf_DATA = \
	src/adapters/prologix.conf.xml
	
LDADD = $(XML_LIBS)
	
AM_CPPFLAGS = $(XML_CFLAGS)
AM_CPPFLAGS += -DDATA_PATH=\"$(sysconfdir)\"