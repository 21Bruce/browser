##########################################################################
#                              Configutation                             #
##########################################################################

# binary name
BIN:=booksmart

# code directories
SUBCODEDIRS:=./net ./lib ./http ./crypto

# compatibility layer (and other libraries)
INCDIRS:=./compat

# testing frameworks 
TESTDIRS:=./regress

# compiler
CC:=clang
OPT:=-O0
DEPFLAGS:=-MD -MP
CFLAGS:=-Wextra -Wall -g 

##########################################################################
#                                Automated                               #
##########################################################################

# create include flags from INCDIRS
INCFLAGS:=
.for DIR in ${INCDIRS}
INCFLAGS+=-I${DIR}
.endfor

# accumulate flags
ALLFLAGS:=${CFLAGS} ${DEPFLAGS} ${INCFLAGS} ${OPT}

# get all c files
CFILES!= find ${SUBCODEDIRS} -name "*.c"
CFILES+=main.c

# get all h files
HFILES!= find ${SUBCODEDIRS} -name "*.h"

# get residual build files 
OFILES:= ${CFILES:S/.c/.o/g}
DFILES:= ${CFILES:S/.c/.d/g}
GCHFILES:= ${HFILES:S/.h/.h.gch/g}

.SUFFIXES: .o .c

.PHONY: all clean

all: ${BIN}

.c.o:
	${CC} ${ALLFLAGS} -c $< -o $@

${BIN}: ${OFILES}
	${CC} ${OFILES} -o $@

clean:
	@rm -rf ${OFILES} ${DFILES} ${GCHFILES} 
