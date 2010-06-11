bin_PROGRAMS = docsis
docsis_SOURCES = docsis.c ethermac.c md5.c docsis_encode.c docsis_decode.c docsis_snmp.c docsis_yy.y docsis_lex.l hmac_md5.c

# set the include path found by configure
INCLUDES = $(all_includes) $(NETSNMP_CFLAGS)

# the library search path.
docsis_LDFLAGS = $(all_libraries) $(NETSNMP_LIBS) $(LEXLIB) $(LIBM)
noinst_HEADERS = ethermac.h md5.h docsis.h docsis_common.h docsis_decode.h docsis_encode.h docsis_symtable.h docsis_snmp.h docsis_globals.h
EXTRA_DIST=docsis_yy.h
CLEANFILES=docsis_yy.c docsis_yy.h docsis_yy.output docsis_lex.c
DISTCLEANFILES=Makefile

