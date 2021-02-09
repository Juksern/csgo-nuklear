#include "hooks.h"
#include <intrin.h>

bool bOpen = true;

WNDPROC oWndProc;
LRESULT __stdcall WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_INSERT)
		{
			bOpen = !bOpen;
		}
	}

	if (bOpen)
		if (nk_d3d9_handle_event(hWnd, uMsg, wParam, lParam))
			return 0;

	return CallWindowProcW(oWndProc, hWnd, uMsg, wParam, lParam);
}

decltype(EndScene::m_original) EndScene::m_original;

auto COM_DECLSPEC_NOTHROW __stdcall EndScene::hooked(IDirect3DDevice9* thisptr) -> HRESULT
{
	static auto init = false;
	static void* ret_addr = nullptr;
	RECT rect = { 0, 0, 0, 0 };

	if (!ret_addr)
		ret_addr = _ReturnAddress();

	if (_ReturnAddress() == ret_addr)
	{
		if (!init)
		{
			//setup WndProcHook
			D3DDEVICE_CREATION_PARAMETERS params;
			thisptr->GetCreationParameters(&params);
			oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(params.hFocusWindow, GWL_WNDPROC,
				reinterpret_cast<LONG_PTR>(WndProc)));

			GetWindowRect(params.hFocusWindow, &rect);

			g_pNkContext = nk_d3d9_init(g_pDevice, rect.left + rect.right, rect.top + rect.bottom);

			struct nk_font_atlas* atlas;
			nk_d3d9_font_stash_begin(&atlas);
			nk_d3d9_font_stash_end();
			init = true;
		}
		else {

			if (bOpen)
			{
				if (nk_begin(g_pNkContext, "private", nk_rect(50, 50, 400, 350), NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_BORDER))
				{
					nk_layout_row_begin(g_pNkContext, NK_DYNAMIC, 25, 4);
					{
						nk_layout_row_push(g_pNkContext, 0.25f);
						nk_button_label(g_pNkContext, "button");
						nk_layout_row_push(g_pNkContext, 0.25f);
						nk_button_label(g_pNkContext, "button");
						nk_layout_row_push(g_pNkContext, 0.25f);
						nk_button_label(g_pNkContext, "button");
						nk_layout_row_push(g_pNkContext, 0.25f);
						nk_button_label(g_pNkContext, "button");
					}
					nk_end(g_pNkContext);
				}
			}

			nk_d3d9_render(NK_ANTI_ALIASING_ON);

			nk_input_begin(g_pNkContext);
			nk_input_end(g_pNkContext);
		}
	}
	return m_original(thisptr);
}