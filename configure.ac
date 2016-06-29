AC_INIT([docsis], [0.9.9-dev])
AC_CONFIG_SRCDIR([src/docsis.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([dist-bzip2])

AC_CANONICAL_HOST

AC_LANG([C])
LT_INIT
dnl AC_PROG_YACC
AC_PROG_LEX
if test "x$LEX" != xflex; then
    AC_MSG_ERROR([flex not found])
fi

AC_CHECK_LIB(resolv, inet_aton)
LT_LIB_M
AC_SUBST(LIBM)

AC_PATH_PROGS([M4], [gm4 gnum4 m4], [notfound],[/usr/local/bin:/usr/freeware/bin:/usr/gnu/bin:${prefix}/bin:${exec_prefix}/bin:$PATH] )
if test "x$M4" = "xnotfound"; then
  AC_MSG_ERROR([M4 is required])
else
  ac_m4_vers=`$M4 --version 2>/dev/null | head -1` ;
  if echo "$ac_m4_vers" | grep -q GNU; then
	AC_MSG_RESULT([   found $ac_m4_vers at $M4 ])
  else
	AC_MSG_ERROR([ GNU M4 is needed	])
  fi
fi

dnl check for bison, yacc won't help;

AC_PATH_PROG([BISON], [bison], [notfound],
                   [$PATH:${prefix}/bin:${exec_prefix}/bin:/usr/freeware/bin:/usr/gnu/bin:/usr/local/bin:/usr/bin])
if test "x$BISON" != "xnotfound" ; then
  ac_bison_vers=`$BISON --version 2>/dev/null | head -1 | cut -d' ' -f4| awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  if test -n "$ac_bison_vers" && test "$ac_bison_vers" -ge 1028000
  then
     YACC="${BISON} -t -d -y"
     BISON="${YACC}"
     AC_MSG_RESULT([   found bison version $ac_bison_vers, using $YACC])
     AC_DEFINE_UNQUOTED(HAVE_BISON, 1, [Defines if your system has GNU bison])
  else
     AC_MSG_ERROR([bison version >= 1.28 not found])
  fi
else
  AC_MSG_ERROR([bison not found])
fi

AC_SUBST(YACC)
AC_SUBST(BISON)

dnl NET-SNMP check by Cornel Ciocirlan.

AC_PATH_PROG([NETSNMP_CONFIG], [net-snmp-config], [/usr/libexec/net-snmp-config],
                   [$PATH:${prefix}/bin:${exec_prefix}/bin:/opt/net-snmp/bin:/usr/local/bin:/usr/bin])
if test -n "$NETSNMP_CONFIG"; then
  vers=`$NETSNMP_CONFIG --version 2>/dev/null | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
  if test -n "$vers" && test "$vers" -ge 5000007
  then
     NETSNMP_LIBS="`$NETSNMP_CONFIG --libs`"
     NETSNMP_PREFIX="`$NETSNMP_CONFIG --prefix`"
     # Hack to allow distcheck to succeed.
     if test "x$prefix" = "xNONE" || echo "$NETSNMP_PREFIX" | grep -q "^$prefix"; then
         NETSNMP_MIBPATH="$NETSNMP_PREFIX/share/snmp/mibs"
     else
         NETSNMP_MIBPATH="${prefix}${NETSNMP_PREFIX}/share/snmp/mibs"
     fi
     NETSNMP_RPATH=
     for args in $NETSNMP_LIBS; do
          case $args in
            -L*)
               NETSNMP_RPATH="$NETSNMP_CONFIG $args"
               ;;
          esac
     done
     NETSNMP_RPATH=`echo $NETSNMP_RPATH | sed -e "s/-L/-R/g"`
     NETSNMP_CFLAGS="`$NETSNMP_CONFIG --cflags`"

     # AC_CHECK_HEADERS(net-snmp/net-snmp-config.h, [
          AC_MSG_RESULT([   found net-snmp version $vers])
          AC_DEFINE_UNQUOTED(HAVE_NETSNMP, 1, [Defines if your system has the net-snmp library])
     # ], [
     #      AC_MSG_ERROR([You need at least net-snmp 5.0.7, get it at http://www.net-snmp.org])
     # ])
  else
     AC_MSG_ERROR([You need at least net-snmp 5.0.7, get it at http://www.net-snmp.org])
  fi
fi

AC_SUBST(NETSNMP_LIBS)
AC_SUBST(NETSNMP_CFLAGS)
AC_SUBST(NETSNMP_RPATH)
AC_SUBST(NETSNMP_PREFIX)

AC_MSG_CHECKING([for netsnmp_init_mib])
orig_CFLAGS="$CFLAGS"
orig_LIBS="$LIBS"
CFLAGS="$CFLAGS $NETSNMP_CFLAGS"
LIBS="$LIBS $NETSNMP_LIBS"
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([[#include <net-snmp/net-snmp-config.h>
                     #include <net-snmp/net-snmp-includes.h>]],
                   [[netsnmp_init_mib();]])],
  [AC_MSG_RESULT(yes)
   AC_DEFINE_UNQUOTED(HAVE_NETSNMP_INIT_MIB, 1, [Defines if your net-snmp has the netsnmp_init_mib function])],
  [AC_MSG_RESULT(no)]
)
CFLAGS="$orig_CFLAGS"
LIBS="$orig_LIBS"

dnl end libnetsnmp check

dnl check for Cygwin & NSIS to build docsis_setup.exe

AC_MSG_CHECKING([for Cygwin])
if test "x$host_os" = "xcygwin" ; then
	AC_MSG_RESULT([ found Cygwin, trying to use NSIS to build setup.exe ])
dnl save current path in DOS format so we can substitute it in in NSIS setup script
	CYGWIN_BUILDROOT=`cygpath -w $PWD`
        NETSNMP_MIBPATH="`cygpath -d ${NETSNMP_PREFIX}/share/snmp/mibs`"
	AC_MSG_RESULT([   using build root $CYGWIN_BUILDROOT])
	AC_SUBST(CYGWIN_BUILDROOT)
	AC_SUBST(NETSNMP_MIBPATH)

	AC_PATH_PROG([MAKENSIS], [makensis], [notfound],[$PATH:$prefix/bin:/cygdrive/c/Program Files/NSIS])
	if test "x$MAKENSIS" = "xnotfound"; then
		AC_MSG_RESULT([makensis not found, no setup.exe...])
	else
		AC_MSG_RESULT([found makensis at $MAKENSIS ... ])
	fi
else
	AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(CYGWIN_BUILD, test "x$host_os" = "xcygwin")
AM_CONDITIONAL(CYGWIN_BUILD_SETUP_EXE, test "x$host_os" = "xcygwin" -a "x$MAKENSIS" != "xnotfound" )

dnl end check for NSIS

AC_PATH_PROG(sedpath, sed)
if test "x$GCC" = "xyes"; then
	dnl We enable -Wall later.
	dnl If it's set after the warning CFLAGS in the compiler invocation, it counteracts the -Wno... flags.
	dnl This leads to warnings we don't want.
	CFLAGS=`echo $CFLAGS |$sedpath 's/-Wall//'`

	dnl ENABLE WARNINGS SUPPORTED BY THE VERSION OF GCC IN USE
	for newflag in \
			"-Waggregate-return" \
			"-Wbad-function-cast" \
			"-Wcast-align" \
			"-Wdeclaration-after-statement" \
			"-Wendif-labels" \
			"-Werror-implicit-function-declaration" \
			"-Wextra -Wno-unused-parameter" \
			"-Wfloat-equal" \
			"-Wformat-security" \
			"-Werror=format-security" \
			"-Winit-self" \
			"-Wmissing-declarations" \
			"-Wmissing-noreturn" \
			"-Wmissing-prototypes" \
			"-Wpointer-arith" \
			"-Wundef" \
	; do
		orig_CFLAGS="$CFLAGS"
		CFLAGS="$CFLAGS $newflag"
		AC_MSG_CHECKING(for $newflag option to gcc)
		AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[
			int main() {return 0;}
		]])], [
			AC_MSG_RESULT(yes)
			CFLAGS="$orig_CFLAGS"
			DEBUG_CFLAGS="$DEBUG_CFLAGS $newflag"
		], [
			AC_MSG_RESULT(no)
			CFLAGS="$orig_CFLAGS"
		])
	done

	if test "x$enable_fortify" = "xyes"; then
		AC_MSG_CHECKING(for FORTIFY_SOURCE support)
		AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <features.h>]], [[
			int main() {
			#if !(__GNUC_PREREQ (4, 1) \
				|| (defined __GNUC_RH_RELEASE__ && __GNUC_PREREQ (4, 0)) \
				|| (defined __GNUC_RH_RELEASE__ && __GNUC_PREREQ (3, 4) \
					&& __GNUC_MINOR__ == 4 \
					&& (__GNUC_PATCHLEVEL__ > 2 \
						|| (__GNUC_PATCHLEVEL__ == 2 && __GNUC_RH_RELEASE__ >= 8))))
			#error No FORTIFY_SOURCE support
			#endif
				return 0;
			}
		]])], [
			AC_MSG_RESULT(yes)
			DEBUG_CFLAGS="$DEBUG_CFLAGS -Wp,-D_FORTIFY_SOURCE=2"
		], [
			AC_MSG_RESULT(no)
		])
	fi
	
	case $host in
        *cygwin*)
                CFLAGS="$CFLAGS -fno-stack-protector"
                ;;
        esac

	CFLAGS="-Wall $DEBUG_CFLAGS -g $CFLAGS"
fi

AC_CONFIG_FILES([build_setup.nsi Makefile src/Makefile doc/Makefile examples/Makefile mibs/Makefile])
AC_OUTPUT
