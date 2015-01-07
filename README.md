Docsis utility
==============

[![Travis-CI builds](https://travis-ci.org/AdrianSimionov/docsis.svg?branch=master)](https://travis-ci.org/AdrianSimionov/docsis) [![Coverage Status](https://coveralls.io/repos/AdrianSimionov/docsis/badge.png)](https://coveralls.io/r/AdrianSimionov/docsis)

This program encodes a DOCSIS binary configuration file from a human-readable text configuration file.

It was originaly developed at http://docsis.sourceforge.net and later on moved to https://github.com/rlaager/docsis

This build is an experimental one and is based on a fork from the GitHub project. Please use this build on your own risk.

Linux/Unix
----------

This was tested on Ubuntu 14.04 amd64 (little endian) ~~and Debian 7.5 Sparc64 (big endian)~~ and it is known to work without issues. It is known to have issues on Sparc64 (big endian) CPU.

Dependencies:

    apt-get install automake libtool libsnmp-dev bison make gcc flex git

Installation steps for Debian based systems:

    git clone https://github.com/AdrianSimionov/docsis.git
    cd docsis
    ./autogen.sh
    ./configure
    make
    make install (optional)

Windows and Mac OS X
--------------------

Not tested. If someone has the resources to do it, please post your findings.

Bug reporting
-------------

Please use the GitHub system to report feature request and bugs.

Known issues
------------

CMTS and CM MIC have the wrong value when calculated on a big endian CPU. Using the MIC values and a big endian CPU should be avoided in production systems.

Quoting the hex strings will result in a crash. Do not use something similar to "0xABCD".

Following TLVs can only be decoded but not encoded: SNMPv1v2cTransportAddress, SNMPv1v2cTransportAddressMask.

