# suite harness

.ifndef DEBUG
DEBUG:=0
.endif

# get all suite dirs 
SUBDIRS != ls -d */

# remove / from suitdirs
SUBDIRNMS :=
.for DIR in ${SUBDIRS}
SUBDIRNMS += ${DIR:/=}
.endfor

# generate rule names for all suites
RULENMS:=
.for DIR in ${SUBDIRNMS}
RULENMS += ${DIR}-run
.endfor

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
TOPDIR := ${PWD}/../ 

# get all top-level sub-dirs 
TSUBDIRS != ls -d ${TOPDIR}/*/

# remove regress, giving us all code-dirs 
SUBCDIRS:= ${TSUBDIRS:N*/regress/}

# get all c files
CFILES!= find ${SUBCDIRS} -name "*.c"
HFILES!= find ${SUBCDIRS} -name "*.h"

# get residual build files 
COFILES:= ${CFILES:S/.c/.o/g}
CDFILES:= ${CFILES:S/.c/.d/g}

GCHFILES:= ${HFILES:S/.h/.h.gch/g}

.SUFFIXES: .o .c

# save in json
.ifdef JSON
json: 
	@$(MAKE) run > ${JSON}.json
.endif

run: ${COFILES} start ${RULENMS} end clean

.c.o:
.if ${DEBUG} >= 2
	${CC} ${ALLFLAGS} -c $< -o $@ 
.else
	@${CC} ${ALLFLAGS} -c $< -o $@ > /dev/null 2>&1
.endif

clean:
	-@rm -rf ${COFILES} ${CDFILES} ${GCHFILES} 

start:
	@echo "{"
	@echo "\t\"suites\":["

end:
	@echo "\t],"
	@echo "}"

# generate rules for each test suite
.for DIR in ${SUBDIRNMS}
${DIR}-run:
	@echo "\t\t{"
	@echo "\t\t\t\"name\": \"${DIR}\","
	@bsdmake DEBUG=${DEBUG} HARNESS=1 -C ./${DIR}/ 2>/dev/null | awk ' BEGIN { } { print "\t\t\t" $$0 } END { } ' 
#	@bsdmake -C ./${DIR}/ | awk ' BEGIN { } { print "\t\t" $$0 } END { } ' 
	@echo "\t\t},"
.endfor


