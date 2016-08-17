FLEX ?= flex
BISON ?= /usr/local/opt/bison/bin/bison

scanner: scanner.lpp
	$(FLEX) scanner.lpp

parser: parser.ypp
	$(BISON) parser.ypp

clean:
	rm -f scanner.hxx scanner.cxx parser.cxx parser.hxx location.hh stack.hh position.hh


.PHONY: scanner parser clean