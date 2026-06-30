#include "core.hpp"

//manager layer
void app(){
	core::sdl_ctx_manager scm{};
	if(!scm.is_ok()){throw std::runtime_error{scm.what()};}

	core::sdl_window_manager swm{};
	if(!swm.is_ok()){throw std::runtime_error{swm.what()};}

	core::sdl_gl_ctx_manager sgcm{swm.window};
	if(!sgcm.is_ok()){throw std::runtime_error{sgcm.what()};}

	core::sdl3_gl3_imgui_ctx_manager sgicm{swm.window,sgcm.gl_ctx};
	if(!sgicm.is_ok()){throw std::runtime_error{sgicm.what()};}

	core::sdl_event_manager sem{swm,sgcm,sgicm};
	sem.loop();
	if(!sem.is_ok()){throw std::runtime_error{sem.what()};}
}

int main(int argc, char** argv) try{
	app();

	return 0;
}

catch(std::runtime_error &e){
	SDL_Log("%s",e.what());
	return 1;
}

catch(std::exception &e){
	SDL_Log("%s",e.what());
	return 2;
}

