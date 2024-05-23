# suite harness

# get all sub-dirs 
SUBDIRS != ls -d */

# remove /
SUBDIRNMS :=
.for DIR in ${SUBDIRS}
SUBDIRNMS += ${DIR:/=}
.endfor

# generate rule names for all suites
RULENMS:=
.for DIR in ${SUBDIRNMS}
RULENMS += ${DIR}-run
.endfor

# save in json
.ifdef JSON
json: 
	@$(MAKE) run > ${JSON}.json
.endif

run: start ${RULENMS} end

start:
	@echo "{"

end:
	@echo "}"

# generate rules for each test suite
.for DIR in ${SUBDIRNMS}
${DIR}-run:
	@echo "\t\"${DIR}\": {"
	@bsdmake -C ./${DIR}/ 2>/dev/null | awk ' BEGIN { } { print "\t\t" $$0 } END { } ' 
	@echo "\t},"
.endfor


