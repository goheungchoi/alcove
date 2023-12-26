build: init
	cmake --build build

build-test: init
	cmake --build build -DBUILD_TESTING=ON

build-verbose: init
	cmake --build build --verbose --trace

graph: init
	cd build && cmake .. --graphviz=graph.dot && dot -Tsvg graph.dot -o dependency_graph.svg

init:
	cmake -S . -B build

clean:
	rm -rf build
	mkdir build