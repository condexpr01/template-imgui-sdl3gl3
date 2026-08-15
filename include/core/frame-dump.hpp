#ifndef CORE_HEADER_FRAME_DUMP_GUARD
#define CORE_HEADER_FRAME_DUMP_GUARD

#include "core.hpp"

#include "stb_image_write.h"

namespace core{

	inline void frame_dump_png(size_t w,size_t h,std::filesystem::path path){
		size_t size = w*h*4;//rgba
		std::vector<uint8_t> pixels{size,std::allocator<uint8_t>()};
		glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());

		stbi_flip_vertically_on_write(1);
		if(stbi_write_png(path.c_str(),w,h,4,pixels.data(),w*4) == 0){
			SDL_Log("[frame_dump_png]stbi_write_png failed");//just log
		}
	}

}
#endif
