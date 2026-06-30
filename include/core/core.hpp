#ifndef CORE_HEADER_GUARD
#define CORE_HEADER_GUARD

#include "glad/gl.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "implot.h"
#include "implot3d.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <filesystem>
#include <format>

namespace core{

	//manage sdl init/quit context
	class sdl_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methos
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//RAII
		public:
			sdl_ctx_manager() noexcept{
				if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
					status = false;
					reason = "[sdl_ctx_manager] SDL_Init";
					return;
				}

				status = true;
			}

			~sdl_ctx_manager() noexcept{SDL_Quit();}

			//neither movable nor copyable
			sdl_ctx_manager& operator=(sdl_ctx_manager  &s) =delete;
			sdl_ctx_manager& operator=(sdl_ctx_manager &&s) =delete;
	};

	//manage SDL_GLContext
	class sdl_gl_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methos
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			SDL_GLContext gl_ctx = nullptr;

		//methods
		public:
			void create(SDL_Window *window) noexcept{
				if (gl_ctx){release();}

				if (window){
					gl_ctx = SDL_GL_CreateContext(window);
				}else{
					status = false;
					reason = "[sdl_gl_ctx_manager] invalid SDL_Window*";
					return;
				}

				if (!gl_ctx){
					status = false;
					reason = "[sdl_gl_ctx_manager] invalid SDL_GLContext";
					return;
				}

				if (!gladLoadGL(SDL_GL_GetProcAddress)){
					status = false;
					reason = "[sdl_gl_ctx_manager] gladLoadGL";
					return;
				}

				status = true;
			}

			void release() noexcept{if(gl_ctx){SDL_GL_DestroyContext(gl_ctx);}}

		//RAII
		public:
			sdl_gl_ctx_manager() noexcept = default;
			sdl_gl_ctx_manager(SDL_Window *window) noexcept{create(window);}

			//movable but not copyable
			sdl_gl_ctx_manager &operator=(sdl_gl_ctx_manager  &s) = delete;
			sdl_gl_ctx_manager &operator=(sdl_gl_ctx_manager &&s) = default;

			~sdl_gl_ctx_manager() noexcept{release();}
	};

	//manage sdl3 gl3 impl and imgui ctx
	class sdl3_gl3_imgui_ctx_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			ImGuiContext *imgui_ctx = nullptr;
			ImPlotContext *implot_ctx = nullptr;
			ImPlot3DContext *implot3d_ctx = nullptr;

			ImFont* font = nullptr;
			const float fontsize = 18.f;

		//RAII
		public:
			sdl3_gl3_imgui_ctx_manager(SDL_Window *glwindow,SDL_GLContext glctx) noexcept{
				imgui_ctx = ImGui::CreateContext();
				implot_ctx = ImPlot::CreateContext();
				implot3d_ctx = ImPlot3D::CreateContext();

				if (!imgui_ctx || !implot_ctx || !implot3d_ctx){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]CreateContext";
					return;
				}

				if(!ImGui_ImplSDL3_InitForOpenGL(glwindow,glctx)){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]ImGui_ImplSDL3_InitForOpenGL";
					return;
				}

				if(!ImGui_ImplOpenGL3_Init("#version 460")){
					status = false;
					reason = "[sdl_gl_imgui_ctx_manager]ImGui_ImplOpenGL3_Init";
					return;
				}

				status = true;

				//default font
				std::filesystem::path font_path{std::format(
						"{:s}/fonts/SarasaUiSC-Bold.ttf",
						SDL_GetBasePath())
				};

				ImFontConfig f{};
				f.Flags = ImFontFlags_NoLoadError;

				ImGuiIO &io = ImGui::GetIO();
				if (std::filesystem::exists(font_path)){
					font = (*io.Fonts).AddFontFromFileTTF(font_path.c_str(),fontsize,&f,nullptr);
					io.FontDefault = font;
				}

				io.IniFilename = nullptr;
				io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
				io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			}

			~sdl3_gl3_imgui_ctx_manager() noexcept{
				ImGui_ImplSDL3_Shutdown();
				ImGui_ImplOpenGL3_Shutdown();
				ImGui::DestroyContext(imgui_ctx);
				ImPlot::DestroyContext(implot_ctx);
				ImPlot3D::DestroyContext(implot3d_ctx);
			}

			//neither movable nor copyable
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager  &s) = delete;
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager &&s) = delete;
	};

	class sdl_window_manager{
		//error status
		private:
			bool status = false;
			const char* reason = nullptr;

		//get error status methods
		public:
			bool       is_ok() noexcept{return status;}
			const char* what() noexcept{return reason;}

		//res
		public:
			SDL_Window *window = nullptr;
			size_t w{},h{};

		//methods
		public:
			bool show() noexcept{return SDL_ShowWindow(window);}
			bool hide() noexcept{return SDL_HideWindow(window);}
			SDL_WindowFlags flags() noexcept{return SDL_GetWindowFlags(window);}

		//RAII
		public:
			sdl_window_manager(const char* title, int w, int h, SDL_WindowFlags flags) noexcept{
				if (!window){window = SDL_CreateWindow(title,w,h,flags);}

				if (!window){
					status = false;
					reason = "[sdl_window_manager]SDL_CreateWindow";
					return;
				}else{
					status = true;
				}

			}

			sdl_window_manager() noexcept{
				int denominator = 2;

				SDL_DisplayID did = SDL_GetPrimaryDisplay();
				const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(did);
				if (!dm){
					status = false;
					reason = "[sdl_window_manager]SDL_GetCurrentDisplayMode";
					return;
				}

				if (!window){
					window = SDL_CreateWindow("sdl3-gl3-imgui",
							(*dm).w/denominator,(*dm).h/denominator,
							SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
				}

				if (!window){
					status = false;
					reason = "[sdl_window_manager]SDL_CreateWindow";
					return;
				}else{
					status = true;
				}
			}

			~sdl_window_manager() noexcept{
				if(!window){SDL_DestroyWindow(window);}
			}
	};

	struct sdl_event_ctx{
		//error status
		bool status = false;
		const char* reason = nullptr;

		//ctx vars
		SDL_Event e;
		bool running = false;

		sdl_window_manager &swm;
		sdl_gl_ctx_manager &sgcm;
		sdl3_gl3_imgui_ctx_manager &sgicm;
	};

	class sdl_event_manager{
		//res
		public:
			sdl_event_ctx ctx;

		//get error status methods
		public:
			bool       is_ok() noexcept{return ctx.status;}
			const char* what() noexcept{return ctx.reason;}

		//methods
		public:
			sdl_event_ctx &get_ctx() noexcept{return ctx;}

			void loop() noexcept;

		//RAII
		public:
			sdl_event_manager(
					sdl_window_manager &swm,
					sdl_gl_ctx_manager &sgcm,
					sdl3_gl3_imgui_ctx_manager &sgicm
					) noexcept
				: ctx{
					.swm = swm,
					.sgcm = sgcm,
					.sgicm = sgicm
				}
			{}

			~sdl_event_manager() noexcept{}

			//neither movable nor copyable
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager  &s) = delete;
			sdl3_gl3_imgui_ctx_manager &operator=(sdl3_gl3_imgui_ctx_manager &&s) = delete;
	};

}
#endif
