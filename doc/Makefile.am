## Process this file with automake to generate Makefile.in
htmldocdir = $(prefix)/share/docsis
      htmldoc_DATA = config-settings.html index.html config-format.html

EXTRA_DIST=generate-settings.sh index.html config-format.html
CLEANFILES=config-settings.html

config-settings.html: ../src/docsis_symtable.h
	/bin/sh generate-settings.sh config-settings.html
