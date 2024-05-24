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
COFILES!= find ${SUBCDIRS} -name "*.o"

GCHFILES:= ${HFILES:S/.h/.h.gch/g}

.SUFFIXES: .o .c

.PHONY: all 

all: start ${TRUNS} end
	
start:
	@echo "\"tests\": ["

end:
	@echo "],"

.c.o:
	@${CC} ${ALLFLAGS} -c $< -o $@ > /dev/null 2>&1

.for TCNAME in ${TCNAMES}
TCOFILE-${TCNAME} = ${TCNAME}.o
TCDFILE-${TCNAME} = ${TCNAME}.d
${TCNAME}-run: ${TCNAME}-gen
	@echo "\t{"
	@echo "\t\t\"name\": \"${TCNAME}\","
	-@./${TCNAME} > /dev/null 2>&1 ; echo "\t\t\"status\": "$$?"," 
	@echo "\t}"
	@bsdmake ${TCNAME}-clean
${TCNAME}-gen: ${TCOFILE-${TCNAME}}
	@${CC} ${COFILES} ${TCOFILE-${TCNAME}} -o ${TCNAME} > /dev/null 2>&1
${TCNAME}-clean:
	@rm -rf ${TCOFILE-${TCNAME}} ${TCDFILE-${TCNAME}} ${TCNAME} 	
.endfor
