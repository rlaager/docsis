## Process this file with automake to generate Makefile.in
examplesdir = $(prefix)/share/docsis/examples

CONFIGS_IN = \
	docsis1.0_2class_non-rfc2669.cfg \
	docsis1.0_2class_rfc2669.cfg \
	docsis1.0_basic.cfg \
	docsis1.0_basic_bpi.cfg \
	docsis1.0_basic_upgrade.cfg \
	docsis1.0_example.cfg \
	docsis1.0_filters2_oidnum.cfg \
	docsis1.0_filters2_oidtext.cfg \
	docsis1.0_filters_NmAccess.cfg \
	docsis1.0_filters_oidnum.cfg \
	docsis1.0_filters_oidtext.cfg \
	docsis1.1_classifiers.cfg \
	docsis1.1_classifiers2.cfg \
	docsis1.1_oss_v3coex.cfg \
	docsis1.1_simple.cfg \
	docsis1.1_stress_test.cfg \
	docsis20.cfg \
	docsis20_no_snmp.cfg \
	docsis20_stresstest.cfg \
	docsis_ipv6.cfg \
	packetcable_mta.cfg
CONFIGS_TEXT   = $(CONFIGS_IN:.cfg=.txt)
CONFIGS_BINARY = $(CONFIGS_IN:.cfg=.bin)
CONFIGS_SCRIPT = $(CONFIGS_IN:.cfg=.sh)

examples_DATA = \
	$(CONFIGS_IN) \
	$(CONFIGS_TEXT) \
	keyfile \
	README.txt

EXTRA_DIST = $(examples_DATA)

../src/docsis:
	(cd ../src ; $(MAKE) docsis)
.cfg.bin: ../src/docsis $(srcdir)/keyfile Makefile
	export MIBDIRS="`net-snmp-config --default-mibdirs`:$(top_srcdir)/mibs" ; \
		../src/docsis -e $< $(srcdir)/keyfile $@ | \
		sed "1,/^Final content/d" > $@.1.txt ; \
		../src/docsis -d $@ > $@.2.txt ; \
		if cmp $@.1.txt $@.2.txt && cmp $(<:.cfg=.txt) $@.2.txt ; then \
			rm -f $@.1.txt $@.2.txt ; \
			true ; \
		else \
			rm -f $@ $@.1.txt $@.2.txt ; \
			false ; \
		fi
# First, the TESTS are built, then they're run.
# The .sh doesn't really depend on the .cfg, but this type of pattern rule is
# portable, where %-style rules (e.g. %.sh) are a GNU make extension.
.cfg.sh: Makefile
# This test is a hack to avoid encoding docsis20_stresstest.cfg, which will
# fail because it references external CVC files and the docsis program looks
# for them relative to the CWD instead of the directory containing the input
# configuration file.
	if [ "`basename $@`" = "docsis20_stresstest.sh" ] && \
	   [ "$(srcdir)" != "$(builddir)" ] ; then \
		echo '#!/bin/sh' > $@ ; \
	else \
		(echo '#!/bin/sh' ; echo make -s $(@:.sh=.bin)) > $@ ; \
	fi
	chmod +x $@
TESTS = $(CONFIGS_SCRIPT)

clean-local:
	-rm -f $(CONFIGS_BINARY)
	-rm -f $(CONFIGS_SCRIPT)
	-rm -f $(CONFIGS_IN:.cfg=.bin.1.txt)
	-rm -f $(CONFIGS_IN:.cfg=.bin.2.txt)
