
.PHONY: all clean test

all: libfloatcheck.so

LLVM_VER:=3.5
LLVM_CXXFLAGS:=$(shell llvm-config-$(LLVM_VER) --cxxflags)
LLVM_LDFLAGS:=$(shell llvm-config-$(LLVM_VER) --ldflags --libs )

CLANG_LIBS:= \
	-Wl,--start-group \
	-lclangAST \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangEdit \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangEdit \
	-lclangASTMatchers \
	-lclangRewriteFrontend \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangSerialization \
	-lclangTooling \
	-Wl,--end-group

CXXFLAGS=-fno-rtti -Wall -g -std=c++11

CXXFLAGS+=$(LLVM_CXXFLAGS)
LDFLAGS+=$(LLVM_LDFLAGS) -shared

LOADLIBES+=-lstdc++
LOADLIBES+=$(CLANG_LIBS)

libfloatcheck.so: float-check.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	$(RM) libfloatcheck.so float-check.o
	$(RM) float.S double.S

float.S: float.c
double.S: double.c
	$(CC) -S -g $^ -o - | objdump --disassemble --source > $@


CHECKER:=performance.crit.ExcessPrecision

test: libfloatcheck.so
	scan-build  -load-plugin ~/src/floatcheck/libfloatcheck.so -enable-checker $(CHECKER) $(MAKE) -C test --always-make
	#clang++ -Xclang -load -Xclang $(PWD)/libfloatcheck.so -Xclang -analyzer-checker=core,$(CHECKER) -Xclang -analyze -Xclang -analyzer-output=text -c test/test_one.cpp
