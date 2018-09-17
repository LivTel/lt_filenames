#HOSTTYPE = i386-linux

#cfitsio is here, but these are default paths that should no longer need specifying explicitly.
#CFITSIOINCDIR was a feature of ltdevsrv
#CFITSIOINCDIR = $(LT_SRC_HOME)/cfitsio3310/include
#./usr/lib64/libcfitsio.so
#./usr/include/cfitsio/fitsio2.h
#./usr/include/cfitsio/fitsio.h

#LT_INC_HOME		= ${LT_SRC_HOME}/include 

# eSTAR_SRC_HOME is set in lt_environent.csh
# eSTAR_INC_HOME is set in lt_environent.csh
# eSTAR_LIB_HOME is set in lt_environent.csh but needs addition of the HOSTTYPE
#eSTAR_CONF_HOME		= config
#eSTAR_CONF_INC_HOME	= ${eSTAR_SRC_HOME}/${eSTAR_CONF_HOME}/include
#eSTAR_COMMON_HOME	= common
#eSTAR_COMMON_INC_HOME	= ${eSTAR_SRC_HOME}/${eSTAR_COMMON_HOME}/include

# Destination to write binaries
BINDIR = ./bin/
LIBDIR = ./lib/

#Optimisation flags
# Heavy
OPTFLAGS = -O3 
# Standard
#OPTFLAGS = -O
# None 
#OPTFLAGS = -O0

#Produce dbx output 
#CFLAGS = -g
#CFLAGS = -pg
CFLAGS = 

#DEBUG = -DARCHIVE_DEBUG -g 

#Compile as shared/static library (Gets set in lt_environment anyway)
CCSHAREDFLAG = -shared
CCSTATICFLAG = -static

#Obsessive compiler check flags
CCHECKFLAG = -ansi -pedantic -Wall -W -Warray-bounds

#CC = gcc

shared: ${LIBDIR}liblt_filenames.so

static: ${LIBDIR}liblt_filenames.a

all: lt_filenames.o ${LIBDIR}liblt_filenames.so ${LIBDIR}liblt_filenames.a

install:
	cp lt_filenames.h /usr/local/include/.
	cp ${LIBDIR}liblt_filenames.so /usr/local/lib/.
	cp ${LIBDIR}liblt_filenames.a /usr/local/lib/.

#
# Object
#
lt_filenames.o : lt_filenames.c lt_filenames.h
	${CC} ${CFLAGS} -c -o lt_filenames.o lt_filenames.c ${DEBUG} ${CCHECKFLAG} ${OPTFLAG}

#
# Libraries
#
${LIBDIR}liblt_filenames.so : lt_filenames.c lt_filenames.h Makefile
	${CC} ${CFLAGS} -c ${CCSHAREDFLAG} -o ${LIBDIR}liblt_filenames.so lt_filenames.c ${DEBUG} ${CCHECKFLAG} ${OPTFLAG}

${LIBDIR}liblt_filenames.a : lt_filenames.c lt_filenames.h Makefile
	${CC} ${CFLAGS} -c ${CCSTATICFLAG} -o ${LIBDIR}liblt_filenames.a lt_filenames.c   ${DEBUG}  ${CCHECKFLAG} ${OPTFLAG}




clean:
	rm -f *.o

