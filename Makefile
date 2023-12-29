all: 

###############################
##### Build Targets #########
release: clean config-release
	cmake --build build

build-debug: clean config-debug
	cmake --build build

build-verbose: clean config-debug
	cmake --build build --verbose

build-test: clean config-test
	cmake --build build

###############################
##### Quick Build Targets #####
quick-build:
	@if [ -d "build" ] && [ -f "./build/CMakeCache.txt" ]; then \
		echo "CMakeCache.txt found."; \
		echo "Build targets..."; \
		cmake --build build; \
	else \
		echo "\033[0;31mWARNING: Configuration is not found.\033[0m"; \
		echo "Please, make sure to run config-* commands first."; \
		exit 1; \
	fi

###############################
##### Utility Targets #########
# Make a dependency graph in the `build` directory.
graph: config-debug
	cd build && cmake .. --graphviz=graph.dot && dot -Tsvg graph.dot -o dependency_graph.svg

###############################
##### Config Targets ##########
config-release:
	cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF 

config-debug:
	cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=OFF

config-test:
	cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON

###############################
##### Conan Install Command #####
conan-install:
	conan install conanfile.txt --output-folder=build --build=missing --profile=debug

conan-release:
	conan install conanfile.txt --output-folder=build --build=missing --profile=debug

###############################
##### Clean Command ###########
clean:
	rm -rf build
	mkdir build