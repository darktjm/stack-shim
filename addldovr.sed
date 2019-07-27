# Process C files for LD_PRELOAD wrappers
# e.g. /*+ int open(const char *path, int flags, int mode) */
# will have function libc_open() added/replaced using libc_dll to call libc
# function is locked using global mutex "lock"

# GENERATED FILE:  DO NOT EDIT OR READ THIS FILE
# Instead, read or edit the NoWeb file(s) from which this was generated,
# listed below.  Copyright notice and license terms can be found there.
# $Id: build.nw 197 2012-12-06 05:02:53Z darktjm $
# $Id: ldpreload.nw 199 2012-12-07 05:48:21Z darktjm $

/^\/\*+.*(/{
  p
  s/... *//;s/ *\*\/.*//
  h
  # Find next blank line
  :b
    N
    s/.*\n//
    /^..*$/bb
  # spit out prototype
  g
  p
  i{
  i \  static void *sym = NULL;
  i \  pthread_mutex_lock(&lock);
  g
  # can't handle function pointer returns
  s/[^(]* \**\([^( ]*\) *(.*/  if(!sym \&\& !(sym = dlsym(RTLD_NEXT, "\1"))) {/
  p
  i \    pthread_mutex_unlock(&lock);
  g
  s/[^(]* \**\([^( ]*\) *(.*/    fprintf(stderr, "Can't find bind \1\\n");/
  p
  i \    exit(1);
  i \  }
  i \  pthread_mutex_unlock(&lock);
  g
  s/\([^(]* [^A-Za-z_]*\)[^( ]* *\((.*)\)/  return ((\1(*)\2)sym)(/
  s/return ((void (/((void (/
  x
  # can't handle function pointer args, so try to strip them
  s/( *\* *\([A-Za-z0-9_]*\) *) *([^)]*)/\1/g
  s/[^(]*(//;s/).*/)/
  s/[^,]* [^A-Za-z0-9_, ]*\([a-zA-Z0-9_]* *[,)]\)/\1 /g
  s/^void)/)/
  # paste it all together and tack on a semicolon
  x
  G
  s/\n//
  s/ *$/;/
  a}
  a
}
