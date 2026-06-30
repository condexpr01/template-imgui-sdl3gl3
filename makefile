all:
	cmake -B build -G Ninja && cmake --build build
	-cp build/compile_commands.json src/core
	-cp build/compile_commands.json src/imgui
	-cp fonts build -r

clean:
	-rm -rf build
	
	-rm -rf .cache
	-rm -rf src/imgui/.cache
	-rm -rf src/core/.cache
	
	-rm -rf src/imgui/compile_commands.json
	-rm -rf src/core/compile_commands.json

run:
	./build/template
