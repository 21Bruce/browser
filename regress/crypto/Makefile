# compiler
CC:=clang
OPT:=-O0
DEPFLAGS:=-MD -MP
CFLAGS:=-Wextra -Wall -g 

# accumulate flags
ALLFLAGS:=${CFLAGS} ${DEPFLAGS} ${OPT}

PWD != pwd

TOPDIR := ${PWD}/../ 

#
## get all c files
#TCFILES!= find ./ -name "*.c"
#CFILES!= find ${WD} -name "*.c"
#HFILES!= find ${WD} -name "*.h"
#
## get residual build files 
#TCOFILES:= ${TCFILES:S/.c/.o/g}
#COFILES:= ${CFILES:S/.c/.o/g}
#TCDFILES:= ${TCFILES:S/.c/.d/g}
#CDFILES:= ${CFILES:S/.c/.d/g}
#
#OFILES:= ${TCOFILES} ${COFILES}
#GCHFILES:= ${HFILES:S/.h/.h.gch/g}

.SUFFIXES: .o .c

.PHONY: all clean

all:
	@echo "${PWD}"
#
#.c.o:
#	${CC} ${ALLFLAGS} -c $< -o $@
#
#${BIN}: ${OFILES}
#	${CC} ${OFILES} -o $@
#
#clean:
#	@rm -rf ${OFILES} ${DFILES} ${GCHFILES} 
