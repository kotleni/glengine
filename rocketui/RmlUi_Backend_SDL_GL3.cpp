#include "RmlUi_Backend.h"
#include "RmlUi_Platform_SDL.h"
#include "RmlUi_Renderer_GL3.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Profiling.h>
#include <SDL.h>
#include <SDL_image.h>

#if defined RMLUI_PLATFORM_EMSCRIPTEN
	#include <emscripten.h>
#else
	#if !(SDL_VIDEO_RENDER_OGL)
		#error "Only the OpenGL SDL backend is supported."
	#endif
#endif

/**
    Custom render interface example for the SDL/GL3 backend.

    Overloads the OpenGL3 render interface to load textures through SDL_image's built-in texture loading functionality.
 */
class RenderInterface_GL3_SDL : public RenderInterface_GL3 {
public:
	RenderInterface_GL3_SDL() {}

	Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override
	{
		Rml::FileInterface* file_interface = Rml::GetFileInterface();
		Rml::FileHandle file_handle = file_interface->Open(source);
		if (!file_handle)
			return {};

		file_interface->Seek(file_handle, 0, SEEK_END);
		const size_t buffer_size = file_interface->Tell(file_handle);
		file_interface->Seek(file_handle, 0, SEEK_SET);

		using Rml::byte;
		Rml::UniquePtr<byte[]> buffer(new byte[buffer_size]);
		file_interface->Read(buffer.get(), buffer_size, file_handle);
		file_interface->Close(file_handle);

		const size_t i_ext = source.rfind('.');
		Rml::String extension = (i_ext == Rml::String::npos ? Rml::String() : source.substr(i_ext + 1));

		SDL_Surface* surface = IMG_LoadTyped_RW(SDL_RWFromMem(buffer.get(), int(buffer_size)), 1, extension.c_str());
		if (!surface)
			return {};

		texture_dimensions = {surface->w, surface->h};

		if (surface->format->format != SDL_PIXELFORMAT_RGBA32)
		{
			// Ensure correct format for premultiplied alpha conversion and GenerateTexture below.
			SDL_Surface* converted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
			SDL_FreeSurface(surface);

			if (!converted_surface)
				return {};

			surface = converted_surface;
		}

		// Convert colors to premultiplied alpha, which is necessary for correct alpha compositing.
		const size_t pixels_byte_size = surface->w * surface->h * 4;
		byte* pixels = static_cast<byte*>(surface->pixels);
		for (size_t i = 0; i < pixels_byte_size; i += 4)
		{
			const byte alpha = pixels[i + 3];
			for (size_t j = 0; j < 3; ++j)
				pixels[i + j] = byte(int(pixels[i + j]) * int(alpha) / 255);
		}

		Rml::TextureHandle texture_handle = RenderInterface_GL3::GenerateTexture({pixels, pixels_byte_size}, texture_dimensions);

		SDL_FreeSurface(surface);

		return texture_handle;
	}
};

/**
    Global data used by this backend.

    Lifetime governed by the calls to Backend::Initialize() and Backend::Shutdown().
 */
struct BackendData {
	SystemInterface_SDL system_interface;
	RenderInterface_GL3_SDL render_interface;

	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;

	bool running = true;
};
static Rml::UniquePtr<BackendData> data;

bool Backend::Initialize(int width, int height, SDL_Window *window, SDL_GLContext glcontext)
{
	RMLUI_ASSERT(!data);

	// if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
	// 	return false;

	// Submit click events when focusing the window.
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

#if defined RMLUI_PLATFORM_EMSCRIPTEN
	// GLES 3.0 (WebGL 2.0)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
	// GL 3.3 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

	// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// const Uint32 window_flags = (SDL_WINDOW_OPENGL | (allow_resize ? SDL_WINDOW_RESIZABLE : 0));

	// SDL_Window* window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
	// if (!window)
	// {
	// 	Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on create window: %s", SDL_GetError());
	// 	return false;
	// }

	// SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	// SDL_GL_MakeCurrent(window, glcontext);
	// SDL_GL_SetSwapInterval(1);

	if (!RmlGL3::Initialize())
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to initialize OpenGL renderer");
		return false;
	}

	data = Rml::MakeUnique<BackendData>();

	if (!data->render_interface)
	{
		data.reset();
		Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to initialize OpenGL3 render interface");
		return false;
	}

	data->window = window;
	data->glcontext = glcontext;

	data->system_interface.SetWindow(window);
	data->render_interface.SetViewport(width, height);

	return true;
}

void Backend::Shutdown()
{
	RMLUI_ASSERT(data);

	SDL_GL_DeleteContext(data->glcontext);
	SDL_DestroyWindow(data->window);

	data.reset();

	SDL_Quit();
}

Rml::SystemInterface* Backend::GetSystemInterface()
{
	RMLUI_ASSERT(data);
	return &data->system_interface;
}

Rml::RenderInterface* Backend::GetRenderInterface()
{
	RMLUI_ASSERT(data);
	return &data->render_interface;
}

bool Backend::ProcessEvent(Rml::Context* context, SDL_Event event) {
	RMLUI_ASSERT(data && context);

	switch(event.type) {
		case SDL_WINDOWEVENT:
			if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				Rml::Vector2i dimensions(event.window.data1, event.window.data2);
				data->render_interface.SetViewport(dimensions.x, dimensions.y);
			}
			break;
	}

	RmlSDL::InputEventHandler(context, event);
}

void Backend::RequestExit()
{
	RMLUI_ASSERT(data);

	data->running = false;
}

void Backend::BeginFrame()
{
	RMLUI_ASSERT(data);

	// data->render_interface.Clear();
	data->render_interface.BeginFrame();
}

void Backend::PresentFrame()
{
	RMLUI_ASSERT(data);

	data->render_interface.EndFrame();
	// SDL_GL_SwapWindow(data->window);

	// Optional, used to mark frames during performance profiling.
	RMLUI_FrameMark;
}
