noinst_LTLIBRARIES = libyahoo2.la

EXTRA_DIST = md5.h yahoo_debug.h \
	yahoo_util.h \
	yahoo2.h yahoo2_callbacks.h yahoo2_types.h \
	yahoo_httplib.h yahoo_list.h sha.h yahoo_fn.h

AM_CFLAGS = -DUSE_STRUCT_CALLBACKS
LIBS = @LIBS@ 

libyahoo2_la_SOURCES = libyahoo2.c crypt.c md5.c \
	yahoo_httplib.c yahoo_util.c yahoo_list.c sha.c yahoo_fn.c

libyahoo2_la_LDFLAGS = 

