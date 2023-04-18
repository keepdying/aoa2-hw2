build_convex:
	g++ -std=c++11 -Wall -Werror convex.cpp -o convex -g

build_prim:
	g++ -std=c++11 -Wall -Werror prim.cpp -o prim -g

all:
	make build_convex
	make build_prim