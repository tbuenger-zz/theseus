CXX ?= g++
FLEX ?= flex
BISON ?= /usr/local/opt/bison/bin/bison

all: theseus run

parser: src/tptp/parser.ypp src/tptp/scanner.lpp src/tptp/scanner.hpp
	cd src/tptp && $(MAKE) parser scanner

theseus: parser src/prover.cpp
	$(CXX) -Isrc/ src/tptp/parser.cxx src/tptp/scanner.cxx src/tptp/driver.cpp -o theseus --std=c++11 -Wno-deprecated-register

run: theseus
	./theseus

clean:
	cd src/tptp && $(MAKE) clean

.PHONY: all run clean parser scanner
