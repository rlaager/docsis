# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = src doc examples mibs
EXTRA_DIST = README.txt License.txt build_setup_template.nsi
if CYGWIN_BUILD
if CYGWIN_BUILD_SETUP_EXE
bin_SCRIPTS=docsis_setup.exe
docsis_setup.exe: build_setup.nsi
	"${MAKENSIS}" build_setup.nsi

build_setup.nsi: build_setup_template.nsi
	echo "s/@@buildroot@@/${CYGWIN_BUILDROOT}/g" | sed 's/\\/\\\\/g' > config_build_sedscript
	echo "s/@@netsnmproot@@/${NETSNMP_MIBPATH}/g" | sed 's/\\/\\\\/g' >> config_build_sedscript
	sed -f config_build_sedscript < build_setup_template.nsi > build_setup.nsi

CLEANFILES=config_build_sedscript build_setup.nsi
endif
endif
