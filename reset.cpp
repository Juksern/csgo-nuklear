#include "hooks.h"

decltype(Reset::m_original) Reset::m_original;

auto COM_DECLSPEC_NOTHROW __stdcall Reset::hooked(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) -> HRESULT
{
	HRESULT hr;

	if (g_pNkContext)
	{
		nk_free(g_pNkContext);
		g_pNkContext = NULL;
	}

	nk_d3d9_release();

	hr = m_original(thisptr, params);
	if (SUCCEEDED(hr))
	{
		D3DVIEWPORT9 viewport;
		g_pDevice->GetViewport(&viewport);

		g_pNkContext = nk_d3d9_init(g_pDevice, viewport.Width, viewport.Height);
		struct nk_font_atlas* atlas;
		nk_d3d9_font_stash_begin(&atlas);
		nk_d3d9_font_stash_end();
	}

	return m_original(thisptr, params);
}