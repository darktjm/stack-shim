This is a copy of my sed script for LD_PRELOAD libraries (available on
this site as well) modified to just relay the call to the original
library.  If you compile this with the appropriate options, it will
force 16-byte stack alignment.

While the sed script generates most of the code, you still have to
identify fucntions to override, find their prototypes, and add them to
the code.  I have done this for the game Eschalon Book I in
`stack-fix.c`: at first I added functions that were crashing, but when
gdb was no longer able to identify these (due to missing unwind
information), I went ahead and added all symbols in glx or GL directly
referred to by the binary. This also includes glXGetProcAddressARB,
which I treat differently in order to find out what addresses are
being queried and adding them manually (in this case, only one
function).

To generate the code, run sed:

    sed -i -f addldovr.sed stack-fix.c

To clean the source for further editing, run sed:

    sed -i -f delldovr.sed stack-fix.c

To compile, make sure and set the stack alignment option.  The
following assumes that `-mpreferred-stack-boundary=4` is the default;
otherwise add that as well.

    gcc -m32 -mincoming-stack-boundary=2 -shared -o stack-fix.{so,c} -ldl -lX11 -lGL

To use, just add stack-fix.so to the LD_PRELOAD environment variable,
using the full path if not in your LD_LIBRARY_PATH.

This cures the mysterious crashing on the Eschalon game series.  It
may cure others, as well, if you add enough prototypes.
