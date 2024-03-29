# Copyright (C) 2006, 2007 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: configure.ac 1133 2008-08-22 18:11:18Z stefan $

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([ThirdPartyBlas],[1.0.13],[http://projects.coin-or.org/BuildTools/newticket])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Common Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(dgemm.f)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

AC_COIN_PROJECTDIR_INIT

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

#turn off warnings and pedantic mode since they could make trouble on some platforms,
#and we couldn't fix them anyway
coin_skip_warn_fflags=yes

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(Blas)

# Get the name of the Fortran compiler and appropriate compiler options
AC_COIN_PROG_F77

# Initialize automake
AC_COIN_INIT_AUTO_TOOLS

# Check if Blas is already available on the system or provided by user
AC_COIN_HAS_BLAS
MAKEOKFILE=.MakeOk
if test $coin_has_blas = yes && test "$use_blas" != BUILD; then
  rm -f $MAKEOKFILE
  AC_MSG_NOTICE([The Blas sources don't need to be compiled])
  MAKEOKFILE=
else
  touch $MAKEOKFILE
fi
AC_SUBST(MAKEOKFILE)
AM_CONDITIONAL(SKIPMAKE, test -z "$MAKEOKFILE")

AC_CONFIG_FILES([Makefile])

AC_COIN_FINALIZE
