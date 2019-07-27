# Process C files for LD_PRELOAD wrappers
# e.g. /*+ int open(const char *path, int flags, int mode) */
# will have function libc_open() added/replaced using libc_dll to call libc
# function is locked using global mutex "lock"

# GENERATED FILE:  DO NOT EDIT OR READ THIS FILE
# Instead, read or edit the NoWeb file(s) from which this was generated,
# listed below.  Copyright notice and license terms can be found there.
# $Id: build.nw 183 2012-11-26 07:59:32Z darktjm $
# $Id: ldpreload.nw 182 2012-11-26 07:23:23Z darktjm $

/^\/\*+.*(/{
  p
  s/... *//;s/ *\*\/.*//
  h
  # Find next blank line
  :b
    N
    s/.*\n//
    /^..*$/bb
}
