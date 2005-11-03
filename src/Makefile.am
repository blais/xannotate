## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xannotate
#data_DATA = wayv.conf DEFAULT.wkey
xannotate_SOURCES = backend.c display.c process.c
# ai.c 
# aimath.c 
# gesture.c 
# setup.c 
# misc.c
CFLAGS = -Wall -O2 -DSETUPPATH=\"@datadir@\"
xannotate_LDADD = @X_LIBS@
xannotate_LDFLAGS = -I@x_includes@ -L@x_libraries@
