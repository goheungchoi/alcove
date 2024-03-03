GENERATOR = "MSYS Makefiles"
CMAKE_CONFIG = -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -G$(GENERATOR) -DCMAKE_TOOLCHAIN_FILE=cmake/conan_toolchain.cmake 

###############################
all: 

###############################
##### Build Targets ###########
release: clean config-release
	cmake --build build

build-debug: clean config-debug
	cmake --build build

build-verbose: clean config-debug
	cmake --build build --verbose

build-test: clean config-test
	cmake --build build

msvc-release:
	cmake --build build --config Release

msvc-debug:
	cmake --build build --config Debug

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
	cmake $(CMAKE_CONFIG) -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF 

config-debug:
	cmake $(CMAKE_CONFIG) -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=OFF

config-test:
	cmake $(CMAKE_CONFIG) -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON

config-msvc:
	cmake -S . -B build -G "Visual Studio 17 2022" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_TOOLCHAIN_FILE=cmake/conan_toolchain.cmake 

#################################
##### Conan Install Command #####
conan-debug:
	conan install conanfile.py --build=missing --profile=debug

conan-release:
	conan install conanfile.py --build=missing --profile=release

###############################
##### Clean Command ###########
clean:
	rm -rf ./build
	mkdir ./build