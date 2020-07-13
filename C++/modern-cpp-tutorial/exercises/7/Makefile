#
# Makefile
#
# exercise solution - chapter 7
# modern cpp tutorial
#
# created by changkun at changkun.de/modern-cpp
#

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	clang++ $< -o $@ -std=c++1z -pedantic

clean:
	rm *.out
