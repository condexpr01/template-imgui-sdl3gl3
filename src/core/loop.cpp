#include "core.hpp"
#include "gl-debug.hpp"

#include "gl-frame-dump.hpp"

/*
struct ubo_struct{
	glm::mat4 mvp;       //64[16]
}ubo_data;
*/

class prepare{
	public:
		void set_up_current_ctx(core::sdl_event_ctx &ctx);
		void set_up_gl_program(core::sdl_event_ctx &ctx);
		void set_up_gl_capabilities(core::sdl_event_ctx &ctx);
		void set_up_gl_objects(core::sdl_event_ctx &ctx);

	public:
		void all(core::sdl_event_ctx &ctx){
			set_up_current_ctx(ctx);

			set_up_gl_capabilities(ctx);
			set_up_gl_objects(ctx);
			set_up_gl_program(ctx);
		}
};

class frame{
	public:
		void imgui_window(core::sdl_event_ctx &ctx);

	public:
		void handle_events(core::sdl_event_ctx &ctx);
		void init_frame(core::sdl_event_ctx &ctx);
		void draw_scene(core::sdl_event_ctx &ctx);
		void build_ui(core::sdl_event_ctx &ctx);
		void draw_ui(core::sdl_event_ctx &ctx);
		void render_frame(core::sdl_event_ctx &ctx);

	//all
	public:
		void one_frame(core::sdl_event_ctx &ctx){
			handle_events(ctx);
			init_frame(ctx);

			build_ui(ctx);

			draw_scene(ctx);
			draw_ui(ctx);

			render_frame(ctx);
		}
};


void prepare::set_up_gl_capabilities(core::sdl_event_ctx &ctx){

	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	//glEnable(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(std::numeric_limits<GLuint>::max());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
}


void prepare::set_up_gl_program(core::sdl_event_ctx &ctx){
}

void prepare::set_up_gl_objects(core::sdl_event_ctx &ctx){
}

void prepare::set_up_current_ctx(core::sdl_event_ctx &ctx){
	if(!SDL_GL_MakeCurrent(ctx.swm.window,ctx.sgcm.gl_ctx)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_MakeCurrent";
		return;
	}

	if(!SDL_GL_SetSwapInterval(1)){
		ctx.status = false;
		ctx.reason = "[prepare::set_up_current_ctx]SDL_GL_SetSwapInterval";
		return;
	}

	ctx.status = true;
	ctx.running = true;
}

void frame::handle_events(core::sdl_event_ctx &ctx){

	while(SDL_PollEvent(&ctx.e)){
		ImGui_ImplSDL3_ProcessEvent(&ctx.e);

		if (ctx.e.type == SDL_EVENT_QUIT){ctx.running = false;}

		do{//change running state in main window
			if(ctx.e.window.windowID != SDL_GetWindowID(ctx.swm.window))break;

			if (ctx.e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED){ctx.running = false;}
			if (ctx.e.type == SDL_EVENT_KEY_DOWN && ctx.e.key.key == SDLK_ESCAPE){ctx.running = false;}
		}while(false);
	}

	#if 0
	if(ctx.keyboard.is_pressed(SDL_SCANCODE_F)){
		ctx.camera.translate(0.000f,-0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_R)){
		ctx.camera.translate(0.000f,0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_SPACE)){
		ctx.camera.translate(0.000f,0.001f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_W)){
		ctx.camera.translate(0.f,0.f,0.001f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_S)){
		ctx.camera.translate(0.f,0.f,-0.001f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_A)){
		ctx.camera.translate(-0.001f,0.f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_D)){
		ctx.camera.translate(0.001f,0.f,0.f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_H)){
		ctx.camera.auto_roll_rotate(-0.1f,0);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_J)){
		ctx.camera.auto_roll_rotate(0,-0.1f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_K)){
		ctx.camera.auto_roll_rotate(0,0.1f);
	}

	if(ctx.keyboard.is_pressed(SDL_SCANCODE_L)){
		ctx.camera.auto_roll_rotate(0.1f,0);
	}
	#endif

}


void frame::draw_scene(core::sdl_event_ctx &ctx){

	//debug
	core::debug_error();
}

void frame::imgui_window(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context.");
	IMGUI_CHECKVERSION();

	ImGui::NewFrame();

	ImDrawList* drawlist = ImGui::GetForegroundDrawList();
	drawlist->AddText(ImVec2{0,0},IM_COL32(0x39,0xc5,0xbb,0xff),"condexpr01");

	//ImGui::DockSpaceOverViewport(0,nullptr,ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::SetNextWindowSize(ImVec2{200,200});
	if(ImGui::Begin("NodeEditor")){
		ImNodes::BeginNodeEditor();

		ImNodes::BeginNode(1);

		ImNodes::BeginNodeTitleBar();
		ImGui::Text("node");
		ImNodes::EndNodeTitleBar();

		ImNodes::BeginInputAttribute(2);
		ImGui::Text("in");
		ImNodes::EndInputAttribute();

		ImGui::SameLine();

		ImNodes::BeginOutputAttribute(3);
		ImGui::Text("out");
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();

		ImNodes::EndNodeEditor();
	}
	ImGui::End();

	ImGui::ShowDemoWindow();
	ImPlot3D::ShowDemoWindow();
	ImPlot::ShowDemoWindow();

	ImGui::EndFrame();
}


void frame::build_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	imgui_window(ctx);
}

void frame::draw_ui(core::sdl_event_ctx &ctx){
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context!");
	IMGUI_CHECKVERSION();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void frame::init_frame(core::sdl_event_ctx &ctx){

	//clear
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClearDepth(0.f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//int w,h;
	//SDL_GetWindowSize(ctx.swm.window,&w,&h);
	//ctx.projection = glm::perspective(glm::radians(45.f),(float)w/h,100.f,0.1f);
}

void frame::render_frame(core::sdl_event_ctx &ctx){

	#if 1
		if(!SDL_GL_SwapWindow(ctx.swm.window)){
			ctx.running = false;
			ctx.reason = "[prepare::render_frame]SDL_GL_SwapWindow";
			return;
		}

		//SDL_Delay(0);
		//ctx.running = false;
	#else
		glFinish();

		int w,h;
		SDL_GetWindowSize(ctx.swm.window,&w,&h);
		core::frame_dump_png(w,h,ctx.png_path);

		ctx.running = false;
	#endif

}

//call top in loop
void core::sdl_event_manager::loop() noexcept{
	prepare prepare;
	frame frame;

	prepare.all(ctx);

	while(ctx.running && ctx.status){
		frame.one_frame(ctx);
	}

}

