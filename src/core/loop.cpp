#include "core.hpp"

class frame{
	public:
		void imgui_window(core::sdl_event_ctx &ctx);
		void handle_events(core::sdl_event_ctx &ctx);
		void build_ui(core::sdl_event_ctx &ctx);
		void init_frame(core::sdl_event_ctx &ctx);
		void render_frame(core::sdl_event_ctx &ctx);

		void one_frame(core::sdl_event_ctx &ctx){
			handle_events(ctx);
			init_frame(ctx);
			build_ui(ctx);
			render_frame(ctx);
		}
};

class prepare{
	public:
		void set_up_current_ctx(core::sdl_event_ctx &ctx);
};

void frame::imgui_window(core::sdl_event_ctx &ctx){
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    IMGUI_CHECKVERSION();

	ImGui::DockSpaceOverViewport();

	ImGui::ShowDemoWindow();
	ImPlot::ShowDemoWindow();
	ImPlot3D::ShowDemoWindow();

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
}

void frame::build_ui(core::sdl_event_ctx &ctx){
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    IMGUI_CHECKVERSION();

	ImGui::StyleColorsLight();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	imgui_window(ctx);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void frame::init_frame(core::sdl_event_ctx &ctx){
	int w{},h{};
	SDL_GetWindowSize(ctx.swm.window,&w,&h);
	glViewport(0,0,w,h);

	//clear
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void frame::render_frame(core::sdl_event_ctx &ctx){
	SDL_GL_SwapWindow(ctx.swm.window);
	SDL_Delay(0);
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



void core::sdl_event_manager::loop() noexcept{
	prepare prepare;
	frame frame;

	prepare.set_up_current_ctx(ctx);

	while(ctx.running && ctx.status){
		frame.one_frame(ctx);
	}

}
