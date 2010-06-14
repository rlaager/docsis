# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

README: doc/index.html
	(echo This file is generated from doc/index.html, which is easier to read if you have ; echo a web browser available. ; echo ; w3m doc/index.html) > README

SUBDIRS = src doc examples mibs
EXTRA_DIST = build_setup.nsi.in README
MAINTAINERCLEANFILES = README

version-check:
# We don't want to release development versions.
	test x`echo $(PACKAGE_VERSION) | grep dev` = x

# For all releases, check the ChangeLogs.
	head ChangeLog | grep "^version $(PACKAGE_VERSION) (`date +%m/%d/%Y`):$$" >/dev/null

# Ensure we're working from a tag...
	HEAD=`git rev-parse --verify HEAD` && test `git tag -l v$(VERSION) --contains $$HEAD | wc -l` -gt 0
# ... and have no changes in the working copy.
	git status | grep -q "working directory clean"

release: version-check distcheck packages

packages:
	gpg -ab $(PACKAGE)-$(PACKAGE_VERSION).tar.gz
	gpg -ab $(PACKAGE)-$(PACKAGE_VERSION).tar.bz2
	gpg --verify $(PACKAGE)-$(PACKAGE_VERSION).tar.gz.asc $(PACKAGE)-$(PACKAGE_VERSION).tar.gz
	gpg --verify $(PACKAGE)-$(PACKAGE_VERSION).tar.bz2.asc $(PACKAGE)-$(PACKAGE_VERSION).tar.bz2

if CYGWIN_BUILD
if CYGWIN_BUILD_SETUP_EXE
bin_SCRIPTS=docsis_setup.exe
docsis_setup.exe: build_setup.nsi
	"${MAKENSIS}" build_setup.nsi
endif
endif
