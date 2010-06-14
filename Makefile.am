# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

README: doc/index.html
	(echo This file is generated from doc/index.html, which is easier to read if you have ; echo a web browser available. ; echo ; w3m doc/index.html) > README

SUBDIRS = src doc examples mibs
EXTRA_DIST = build_setup.nsi.in README
MAINTAINERCLEANFILES = README
if CYGWIN_BUILD
if CYGWIN_BUILD_SETUP_EXE
bin_SCRIPTS=docsis_setup.exe
docsis_setup.exe: build_setup.nsi
	"${MAKENSIS}" build_setup.nsi
endif
endif
