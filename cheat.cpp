#include "header.h"

//nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D9_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_d3d9.h"

vmt_smart_hook* s_d3d_hook;
nk_context* g_pNkContext;
IDirect3DDevice9* g_pDevice;

auto initialize(void* instance) -> void
{
	while (!helpers::get_module_info("serverbrowser.dll").first)
		std::this_thread::sleep_for(200ms);

	try
	{
		const auto device_addr = helpers::find_pattern("shaderapidx9.dll", "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C",
			"x????xxxxxx");
		auto* const device = **reinterpret_cast<IDirect3DDevice9***>(device_addr + 1);
		assert(device);
		
		g_pDevice = device;

		if (!g_pDevice)
			throw std::exception("failed to initialize g_pDevice");

		s_d3d_hook = new vmt_smart_hook(g_pDevice);
		s_d3d_hook->apply_hook<EndScene>(42);
		s_d3d_hook->apply_hook<Reset>(16);
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "failed to initialize", MB_ICONWARNING | MB_OK);
	}
}