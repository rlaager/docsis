Docsis utility
==============

This program encodes a DOCSIS binary configuration file from a human-readable text configuration file.

It was originaly developed at http://docsis.sourceforge.net and later on moved to https://github.com/rlaager/docsis

This build is an experimental one and is based on a fork from the GitHub project. Please use this build on your own risk.

Linux/Unix
----------

This was tested on Ubuntu 14.04 amd64 (little endian) and Debian 7.5 Sparc64 (big endian) and it is known to work without issues.

Dependencies:

.. code:: bash

  apt-get install automake libsnmp-dev bison make gcc flex git

Installation steps for Debian based systems:

.. code:: bash

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
