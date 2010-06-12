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
	packetcable_mta.cfg
CONFIGS_TEXT   = $(CONFIGS_IN:.cfg=.txt)
CONFIGS_BINARY = $(CONFIGS_IN:.cfg=.bin)
CONFIGS_SCRIPT = $(CONFIGS_IN:.cfg=.sh)

examples_DATA = \
	$(CONFIGS_IN) \
	$(CONFIGS_TEXT) \
	keyfile \
	readme.txt

EXTRA_DIST = $(examples_DATA)

../src/docsis:
	(cd ../src ; $(MAKE) docsis)
.cfg.bin: ../src/docsis keyfile Makefile
	export MIBDIRS="`net-snmp-config --default-mibdirs`:../mibs" ; \
		../src/docsis -e $< keyfile $@ | \
		sed "1,/^Final content/d" > $<.1.txt ; \
		../src/docsis -d $@ > $<.2.txt ; \
		if cmp $<.1.txt $<.2.txt && cmp $(<:.cfg=.txt) $<.2.txt ; then \
			rm -f $<.1.txt $<.2.txt ; \
			true ; \
		else \
			rm -f $@ $<.1.txt $<.2.txt ; \
			false ; \
		fi
# First, the TESTS are built, then they're run.
# The .sh doesn't really depend on the .cfg, but this type of pattern rule is
# portable, where %-style rules (e.g. %.sh) are a GNU make extension.
.cfg.sh: Makefile
	(echo '#!/bin/sh' ; echo make -s $(@:.sh=.bin)) > $@
	chmod +x $@
TESTS = $(CONFIGS_SCRIPT)

clean-local:
	-rm -f $(CONFIGS_BINARY)
	-rm -f $(CONFIGS_SCRIPT)
	-rm -f $(CONFIGS_IN:.cfg=.cfg.1.txt)
	-rm -f $(CONFIGS_IN:.cfg=.cfg.2.txt)
