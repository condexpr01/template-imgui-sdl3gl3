all:
	cmake -S . -B build -G Ninja && cmake --build build

cross-mingw:
	#cross compile on linux:
	cmake -S . -B build -G Ninja -DCROSS_MINGW=ON && cmake --build build

clean:
	-rm -rf build
	
	-rm -rf .cache
	-rm -rf src/imgui/.cache
	-rm -rf src/core/.cache

run:
	./build/prog
