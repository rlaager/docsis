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
	test x`echo $(VERSION) | grep dev` = x

# For all releases, check the ChangeLogs.
	head ChangeLog | grep "^version $(VERSION) (`date +%m/%d/%Y`):$$" >/dev/null

# Ensure we're working from a tag...
	HEAD=`git rev-parse --verify HEAD` && test `git tag -l v$(VERSION) --contains $$HEAD | wc -l` -gt 0
# ... and have no changes in the working copy.
	git status | grep -q "working directory clean"

packages:
	gpg -ab $(PACKAGE)-$(VERSION).tar.gz
	gpg -ab $(PACKAGE)-$(VERSION).tar.bz2
	gpg --verify $(PACKAGE)-$(VERSION).tar.gz.asc \
		$(PACKAGE)-$(VERSION).tar.gz
	gpg --verify $(PACKAGE)-$(VERSION).tar.bz2.asc \
		$(PACKAGE)-$(VERSION).tar.bz2

release: version-check distcheck packages

ONE=$(shell echo $(PACKAGE) | cut -c1)
TWO=$(shell echo $(PACKAGE) | cut -c-2)
FRS=/home/frs/project/$(ONE)/$(TWO)/$(PACKAGE)/$(PACKAGE)/
upload:
	if test -e $(PACKAGE)-$(VERSION)/$(PACKAGE)-$(VERSION).tar.bz2.asc ; \
	then \
		rsync -e ssh -av $(PACKAGE)-$(VERSION) \
			$(USER),$(PACKAGE)@frs.sourceforge.net:$(FRS) ; \
	else \
		$(MAKE) release && \
		mkdir $(PACKAGE)-$(VERSION) && \
		mv $(PACKAGE)-$(VERSION).tar.gz      $(PACKAGE)-$(VERSION) && \
		mv $(PACKAGE)-$(VERSION).tar.gz.asc  $(PACKAGE)-$(VERSION) && \
		mv $(PACKAGE)-$(VERSION).tar.bz2     $(PACKAGE)-$(VERSION) && \
		mv $(PACKAGE)-$(VERSION).tar.bz2.asc $(PACKAGE)-$(VERSION) && \
		cp ChangeLog                         $(PACKAGE)-$(VERSION) && \
		$(MAKE) upload ; \
	fi

if CYGWIN_BUILD
if CYGWIN_BUILD_SETUP_EXE
bin_SCRIPTS=docsis_setup.exe
docsis_setup.exe: build_setup.nsi
	"${MAKENSIS}" build_setup.nsi
endif
endif
