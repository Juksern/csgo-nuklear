#pragma once
#include "header.h"

struct Reset
{
	static auto COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE hooked(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params)->HRESULT;

	static decltype(&hooked) m_original;
};

struct EndScene
{
	static auto COM_DECLSPEC_NOTHROW STDMETHODCALLTYPE hooked(IDirect3DDevice9* thisptr)->HRESULT;

	static decltype(&hooked) m_original;
};