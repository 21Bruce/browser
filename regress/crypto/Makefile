# compiler
CC:=clang
OPT:=-O0
DEPFLAGS:=-MD -MP
CFLAGS:=-Wextra -Wall -g 

# accumulate flags
ALLFLAGS:=${CFLAGS} ${DEPFLAGS} ${OPT}

# get curr dir
PWD != pwd

# get topdir 
TOPDIR := ${PWD}/../../ 

# get all top-level sub-dirs 
SUBDIRS != ls -d ${TOPDIR}/*/

# remove regress, giving us all code-dirs 
SUBCDIRS:= ${SUBDIRS:N*/regress/}

# get all c files
TCFILES!= find ./ -name "*.c"
TRUNS:=
.for TCFILE in ${TCFILES}
TCNAME:= ${TCFILE:S/.c//g}
TCNAME:= ${TCNAME:.//%=%}
TRUNS+= ${TCNAME}-run
.endfor

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

.for TCFILE in ${TCFILES}
TCNAME:= ${TCFILE:S/.c//g}
TCNAME:= ${TCNAME:.//%=%}
TCOFILE:= ${TCFILE:S/.c/.o/g}
TCDFILE:= ${TCFILE:S/.c/.d/g}
${TCNAME}-run: ${TCNAME}-gen
	@echo "\t{"
	@echo "\t\t\"name\": \"${TCNAME}\","
	-@./${TCNAME} > /dev/null 2>&1 ; echo "\t\t\"status\": "$$?"," 
	@echo "\t}"
	@bsdmake ${TCNAME}-clean
${TCNAME}-gen: ${TCOFILE}
	@${CC} ${COFILES} ${TCOFILE} -o ${TCNAME} > /dev/null 2>&1
${TCNAME}-clean:
	@rm -rf ${TCOFILE} ${TCDFILE} ${TCNAME} 	
.endfor
