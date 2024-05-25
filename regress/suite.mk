# compiler
CC:=clang
OPT:=-O0
DEPFLAGS:=-MD -MP
CFLAGS:=-Wextra -Wall -g 

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

all: ${COFILES} start ${TRUNS} end

start:
	@echo "\"tests\": ["

end:
	@echo "],"
.ifndef HARNESS
	-@rm -rf ${COFILES} ${CDFILES} ${GCHFILES}
.endif

.c.o:
	@${CC} ${ALLFLAGS} -c $< -o $@ > /dev/null 2>&1

.for TCNAME in ${TCNAMES}
TCOFILE-${TCNAME} = ${TCNAME}.o
TCDFILE-${TCNAME} = ${TCNAME}.d
${TCNAME}-run: ${TCNAME}-gen ${COFILES}
	@echo "\t{"
	@echo "\t\t\"name\": \"${TCNAME}\","
	-@./${TCNAME} > /dev/null 2>&1 ; echo "\t\t\"status\": "$$?"," 
.ifdef DEBUG
	@echo "\t\t\"debug\": ["
	-@./${TCNAME} 2>&1 | awk ' BEGIN { } { print ("\t\t\t\"" $$0  "\",")} END { } ' 
	@echo "\t\t],"
.endif
	@echo "\t},"
	-@rm -rf ${TCOFILE-${TCNAME}} ${TCDFILE-${TCNAME}} ${TCNAME} 
.if ${.TARGETS} == ${TCNAME}-run
	-@rm -rf ${COFILES} ${CDFILES} ${GCHFILES}
.endif

${TCNAME}-gen: ${TCOFILE-${TCNAME}} ${COFILES}
	@${CC} ${COFILES} ${TCOFILE-${TCNAME}} -o ${TCNAME} > /dev/null 2>&1
.endfor
