[![Build Status][CFUtilities-travis-svg]][CFUtilities-travis]
[![Coverage Status][CFUtilities-codecov-svg]][CFUtilities-codecov]

[CFUtilities-travis]: https://travis-ci.com/Nuovations/CFUtilities
[CFUtilities-travis-svg]: https://travis-ci.com/Nuovations/CFUtilities.svg?branch=main
[CFUtilities-codecov]: https://codecov.io/gh/Nuovations/CFUtilities
[CFUtilities-codecov-svg]: https://codecov.io/gh/Nuovations/CFUtilities/branch/main/graph/badge.svg

Nuovations CoreFoundation Utilities
===================================

# Introduction

This package defines utility interfaces and objects for interacting
and working with Apple's CoreFoundation framework.

In particular, emphasis is placed on making interwork with POSIX, C,
and, in particular, C++ intrinsic types easier and/or more type-safe.

Select interfaces and/or objects for working with the following
CoreFoundation objects and facilities are available:

  * Reference counting and types
  * Booleans
  * Dates and Time
  * Dictionaries
  * Numbers
  * Property Lists
  * Sets
  * Strings
  * Trees

# Getting Started with CFUtilities

## Building CFUtilities

If you are not using a prebuilt distribution of CFUtilities, building CFUtilities
should be a straightforward, two- or three-step process. If you are building from
the main branch, start with:

    % ./bootstrap

If you are building from the main branch, a release branch, or a distribution
package, continue (main branch) or start with:

    % ./configure
    % make

The first `bootstrap` step creates the `configure` script and `Makefile.in` files
from `configure.ac` and `Makefile.am` respectively and only needs to be done once
unless those input files have changed.

The second `configure` step generates `Makefile` files from `Makefile.in` files and only
needs to be done once unless those input files have changed.

Although not strictly necessary, the additional step of sanity checking
the build results is recommended:

    % make check

### Dependencies

CFUtilities depends on the Apple CoreFoundation framework or library. The
framework may be provided on the iOS or macOS (or other) platforms as a
native system framework. Alternatively, CoreFoundation may be provided as a
static or shared library via either the [Apple CFLite](https://opensource.apple.com/source/CF/)
project or via the [OpenCFLite](https://opencflite.sourceforge.io/) project fork
thereof.

The `configure` step recognizes and accepts provisions for working with either
the native framework or the library and attempts to automatically detect and use
the appropriate instance based on the target platform.

Regardless of the CoreFoundation source, CFUtilities has been proven to work
with CoreFoundation versions between and 476.0 and 1258.1, inclusive.

Additionally, CFUtilities depends on:

  * Boost
  * CppUnit

However, CppUnit is only required when building and running the CFUtilities
unit test suite.

If you want to modify or otherwise maintain the CFUtilities build
system, see "Maintaining CFUtilities" below for more information.

## Installing CFUtilities

To install CFUtilities for your use simply invoke:

    % make install

to install CFUtilities in the location indicated by the --prefix `configure`
option (default "/usr/local"). If you intended an arbitrarily
relocatable CFUtilities installation and passed `--prefix=/` to `configure`,
then you might use DESTDIR to, for example install CFUtilities in your user
directory:

    % make DESTIDIR="${HOME}" install

## Maintaining CFUtilities

If you want to maintain, enhance, extend, or otherwise modify CFUtilities, it
is likely you will need to change its build system, based on GNU
autotools, in some circumstances.

After any change to the CFUtilities build system, including any *Makefile.am*
files or the *configure.ac* file, you must run the `bootstrap` or
`bootstrap-configure` (which runs both `bootstrap` and `configure` in
one shot) script to update the build system.

### Dependencies

Due to its leverage of GNU autotools, if you want to modify or
otherwise maintain the CFUtilities build system, the following
additional packages are required and are invoked by `bootstrap`:

  * autoconf
  * automake
  * libtool

#### Linux

On Debian-based Linux distributions such as Ubuntu, these dependencies
can be satisfied with the following:

    % sudo apt-get install autoconf automake libtool

#### Mac OS X

On Mac OS X, these dependencies can be installed and satisfied using
[Brew](https://brew.sh/):

    % brew install autoconf automake libtool

# Interact

There are numerous avenues for CFUtilities support:

  * Bugs and feature requests — [submit to the Issue Tracker](https://github.com/Nuovations/CFUtilities/issues)

# Versioning

CFUtilities follows the [Semantic Versioning guidelines](http://semver.org/)
for release cycle transparency and to maintain backwards compatibility.

# License

CFUtilities is released under the [Apache License, Version 2.0 license](https://opensource.org/licenses/Apache-2.0).
See the `LICENSE` file for more information.
