solver: solver.h solver.cc
	g++ --std=c++11 -o solver solver.cc

solution.dat: solver
	./solver

solution.pdf: solution.dat shape.py
	python3 shape.py

.DEFAULT_GOAL := solution.pdf

.PHONY clean:
	rm -f solver solution.dat solution.pdf
