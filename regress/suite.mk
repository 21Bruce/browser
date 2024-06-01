# compiler
CC:=clang
OPT:=-O0
DEPFLAGS:=-MD -MP
CFLAGS:=-Wextra -Wall -g 

# if no DEBUG level set, assume none
.ifndef DEBUG
DEBUG := 0
.endif

# if no topdir level set, assume this is topdir
.ifndef TOPDIR 
TOPDIR != pwd 
.endif

# accumulate flags
ALLFLAGS:=${CFLAGS} ${DEPFLAGS} ${OPT}

# get all top-level sub-dirs 
SUBDIRS != ls -d ${TOPDIR}/*/

# remove regress, giving us all code-dirs 
SUBCDIRS:= ${SUBDIRS:N*/regress/}

# get all c files
TCFILES!= find ./ -name "*.c"
TRUNS:= ${TCFILES:S/.c/-run/g:.//%=%}
TCNAMES:= ${TCFILES:S/.c//g:.//%=%}

# get all code o files --- should be already compiled by harness
CCFILES!= find ${SUBCDIRS} -name "*.c"
HFILES!= find ${SUBCDIRS} -name "*.h"
COFILES:= ${CCFILES:S/.c/.o/g}
CDFILES:= ${CCFILES:S/.c/.d/g}

GCHFILES:= ${HFILES:S/.h/.h.gch/g}

.SUFFIXES: .o .c

.PHONY: all 

all: ${COFILES} _start ${TRUNS} _end

_start:
	@echo "\"tests\": ["

_end:
	@echo "],"
.ifndef HARNESS
	-@rm -rf ${COFILES} ${CDFILES} ${GCHFILES}
.endif

.c.o:
.if ${DEBUG} >= 2
	${CC} ${ALLFLAGS} -c $< -o $@ 
.else
	@${CC} ${ALLFLAGS} -c $< -o $@ > /dev/null 2>&1
.endif

.for TCNAME in ${TCNAMES}
TCOFILE-${TCNAME} = ${TCNAME}.o
TCDFILE-${TCNAME} = ${TCNAME}.d
${TCNAME}-run: ${TCOFILE-${TCNAME}} ${COFILES}
.if ${DEBUG} >= 2
	${CC} ${COFILES} ${TCOFILE-${TCNAME}} -o ${TCNAME} 
.else 
	@${CC} ${COFILES} ${TCOFILE-${TCNAME}} -o ${TCNAME} > /dev/null 2>&1
.endif
	@echo "\t{"
	@echo "\t\t\"name\": \"${TCNAME}\","
	-@./${TCNAME} > /dev/null 2>&1 ; echo "\t\t\"status\": "$$?"," 
.if ${DEBUG} >= 1
	@echo "\t\t\"debug\": ["
	-@./${TCNAME} 2>&1 | awk ' BEGIN { } { print ("\t\t\t\"" $$0  "\",")} END { } ' 
	@echo "\t\t],"
.endif
	@echo "\t},"
	-@rm -rf ${TCOFILE-${TCNAME}} ${TCDFILE-${TCNAME}} ${TCNAME} 
.if ${.TARGETS} == ${TCNAME}-run
	-@rm -rf ${COFILES} ${CDFILES} ${GCHFILES}
.endif
.endfor
