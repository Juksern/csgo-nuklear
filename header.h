#pragma once

//windows
#include <Windows.h>
#include <d3d9.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

#include "vmthook.h"
#include "helpers.h"
#include "hooks.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_d3d9.h"

struct nk_context;

//globals
extern IDirect3DDevice9* g_pDevice;
extern nk_context* g_pNkContext;