#ifndef CORE_HEADER_DEBUG_GUARD
#define CORE_HEADER_DEBUG_GUARD

#include "core.hpp"

namespace core{

	inline void debug_error(){
		GLenum error = glGetError();
		if(error!=GL_NO_ERROR){
			SDL_Log("[core::debug_error]error: %d",error);
		}
	}

}
#endif
