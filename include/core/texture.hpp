#ifndef CORE_HEADER_TEXTURE_GUARD
#define CORE_HEADER_TEXTURE_GUARD

#include "glad/gl.hpp"
#include <SDL3/SDL.h>

namespace core{

	class texture{

		//error status
		private:
			GLboolean status = GL_FALSE;
			GLchar *reason = nullptr;

		private:
			GLuint texture_id = 0;
			GLenum target = 0;

		//get error status methods
		public:
			GLboolean is_ok() noexcept{return status;}
			GLchar*    what() noexcept{return reason;}

		//methods
		public:
			GLuint id() noexcept{return texture_id;}
			GLenum get_target() noexcept{return target;}

			void bind() noexcept{
				glBindTexture(target,texture_id);
			}

			void unbind() noexcept{
				glBindTexture(target,0);
			}

			void generate_mipmap() noexcept{
				bind();
				glGenerateMipmap(target);
				unbind();
			}

			//GL_TEXTURE0 - GL_TEXTURE15 - ...
			void active(GLenum texture_unit) noexcept{
				glActiveTexture(texture_unit);
			}

			//glTexImagenD arguments:
			//level: mipmap level (0 for base image, 1 for first mipmap level, etc.)
			//internalformat: how to store the texture internally with optional precision (GL_RGBA,GL_RGBA8,etc.)
			//width, height, depth: dimensions of the texture
			//border: must be 0, historical
			//format: format of the pixel, only data sequence without precision (GL_RGBA GL_BGRA etc.)
			//type: data type of the pixel data (GL_UNSIGNED_BYTE, GL_FLOAT, etc.)
			//pixels: pointer to the image data in memory

			//1d
			void image1d(GLint level,GLenum internalformat,
					GLsizei width,GLint border,
					GLenum format, GLenum type, const void *pixels)noexcept{

				bind();
				glTexImage1D(target,level,internalformat,width,border,format,type,pixels);
				unbind();
			}

			//2d
			void image2d(GLint level,GLenum internalformat,
					GLsizei width,GLsizei height,GLint border,
					GLenum format, GLenum type, const void *pixels)noexcept{

				bind();
				glTexImage2D(target,level,internalformat,width,height,border,format,type,pixels);
				unbind();
			}

			//3d
			void image3d(GLint level,GLenum internalformat,
					GLsizei width,GLsizei height,GLsizei depth,GLint border,
					GLenum format, GLenum type, const void *pixels)noexcept{

				bind();
				glTexImage3D(target,level,internalformat,width,height,depth,border,format,type,pixels);
				unbind();
			}

			//sdl3 load png
			void png_image2d(const char* filepath)noexcept{
				SDL_Surface *texture_surface = SDL_LoadPNG(filepath);
				if (!texture_surface){
					SDL_Log("[png_image2d] %s",SDL_GetError());
					return;
				}

				//SDL_PIXELFORMAT32会自己调整和字节序匹配
				if (texture_surface->format != SDL_PIXELFORMAT_RGBA32){
					texture_surface = SDL_ConvertSurface(texture_surface,SDL_PIXELFORMAT_RGBA32);
				}

				image2d(0,GL_RGBA,texture_surface->w,texture_surface->h,0,
						GL_RGBA,GL_UNSIGNED_BYTE,(*texture_surface).pixels);

				SDL_free(texture_surface);
			}

			//sdl load png and use specified target
			void png_image2d_with_target(const char* filepath, GLuint target)noexcept{
				SDL_Surface *texture_surface = SDL_LoadPNG(filepath);
				if (!texture_surface){
					SDL_Log("[png_image2d] %s",SDL_GetError());
					return;
				}

				//SDL_PIXELFORMAT32会自己调整和字节序匹配
				if (texture_surface->format != SDL_PIXELFORMAT_RGBA32){
					texture_surface = SDL_ConvertSurface(texture_surface,SDL_PIXELFORMAT_RGBA32);
				}

				bind();
				glTexImage2D(target,
						0,GL_RGBA,texture_surface->w,texture_surface->h,0,
						GL_RGBA,GL_UNSIGNED_BYTE,(*texture_surface).pixels);
				unbind();

				SDL_free(texture_surface);
			}

			void parameteri(GLint pname, GLint param) noexcept{
				glTexParameteri(target,pname,param);
			}

			void parameteriv(GLint pname, const GLint *params) noexcept{
				glTexParameteriv(target,pname,params);
			}

			void parameterf(GLint pname, GLfloat param) noexcept{
				glTexParameterf(target,pname,param);
			}

			void parameterfv(GLint pname, const GLfloat *params) noexcept{
				glTexParameterfv(target,pname,params);
			}

			void release()noexcept{
				if(texture_id)glDeleteTextures(1,&texture_id);
			}

			void take(texture &other) noexcept{
				texture_id = other.texture_id;
				target = other.target;
				status = other.status;
				reason = other.reason;

				other.texture_id = 0;
				other.target = 0;
				other.status = GL_FALSE;
				other.reason = nullptr;
			}

		//RAII
		public:
			texture(GLenum target) noexcept{
				glGenTextures(1,&texture_id);
				status = GL_FALSE;
				reason = nullptr;
				(*this).target = target;
			}
			//防止重复析构
			texture(texture &) = delete;
			texture(texture &&other)noexcept {take(other);}

			texture& operator=(texture &) = delete;
			texture& operator=(texture &&other) noexcept{
				if(this != &other){
					release();
					take(other);
				}

				return *this;
			}

			~texture() noexcept{release();}
	};

}

#endif
