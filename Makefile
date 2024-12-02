# Destination to write binaries
LIBDIR = ./lib/

#Optimisation flags
OPTFLAGS = -O3 

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
	${CC} ${CFLAGS} ${CCSHAREDFLAG} -fPIC -o ${LIBDIR}liblt_filenames.so lt_filenames.c ${DEBUG} ${CCHECKFLAG} ${OPTFLAG}

${LIBDIR}liblt_filenames.a : lt_filenames.c lt_filenames.h Makefile
	${CC} ${CFLAGS} ${CCSTATICFLAG}       -o ${LIBDIR}liblt_filenames.a lt_filenames.c   ${DEBUG}  ${CCHECKFLAG} ${OPTFLAG}




clean:
	rm -f *.o

