Docsis utility
==============

[![Travis-CI builds](https://travis-ci.org/AdrianSimionov/docsis.svg?branch=master)](https://travis-ci.org/AdrianSimionov/docsis) [![Coverage Status](https://coveralls.io/repos/AdrianSimionov/docsis/badge.png)](https://coveralls.io/r/AdrianSimionov/docsis)

This program encodes a DOCSIS binary configuration file from a human-readable text configuration file.

It was originaly developed at http://docsis.sourceforge.net and later on moved to https://github.com/rlaager/docsis

This build is an experimental one and is based on a fork from the GitHub project. Please use this build on your own risk.

Linux/Unix
----------

This was tested on Ubuntu 14.10, Fedora 21, openSUSE 13.2 (little endian) and Debian 7.5 Sparc64 (big endian) and it is known to work without issues.

Dependencies:

    Debian/Ubuntu:
    apt-get install automake libtool libsnmp-dev bison make gcc flex git libglib2.0-dev
    
    Fedore/RedHat/CentOS:
    yum install autoconf automake libtool glib2-devel bison flex net-snmp-devel
    
    openSUSE/SLES:
    zypper install autoconf automake libtool glib2-devel bison flex net-snmp-devel
    
    FreeBSD/OpenBSD/NetBSD
    pkg install automake autoconf bison flex gcc libtool glib net-snmp
    
Mac OS X
--------

Tested and working on OS X Mavericks. Dependencies should be installed using MacPorts:

    sudo port install autoconf automake libtool glib2-devel bison flex net-snmp

Windows
-------

Not tested.

Installation steps
------------------

Follow the following steps to download and install the utility.

    git clone https://github.com/AdrianSimionov/docsis.git
    cd docsis
    ./autogen.sh
    ./configure
    make
    make install (optional)

Bug reporting
-------------

Please use the GitHub system to report feature request and bugs.

Known issues
------------

Quoting the hex strings will result in a crash. Do not use something similar to "0xABCD".
