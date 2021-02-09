#include "helpers.h"

#include <Windows.h>
#include <Psapi.h>

using CreateInterfaceFn = void* (*)(const char*, int*);

namespace helpers
{
	auto get_interface(const char* module_name, const char* interface_name) -> void*
	{
		const auto addr = get_export(module_name, "CreateInterface");
		const auto create_interface_fn = static_cast<CreateInterfaceFn>(addr);

		return create_interface_fn(interface_name, nullptr);
	}

	auto get_module_info(const char* module_name) -> std::pair<std::uintptr_t, std::size_t>
	{
		auto* const hModule = GetModuleHandleA(module_name);
		if (!hModule)
			return { 0, 0 };
		MODULEINFO module_info;
		K32GetModuleInformation(GetCurrentProcess(), hModule, &module_info, sizeof(MODULEINFO));
		return { reinterpret_cast<std::uintptr_t>(module_info.lpBaseOfDll), module_info.SizeOfImage };
	}

	auto is_code_ptr(void* ptr) -> bool
	{
		constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		MEMORY_BASIC_INFORMATION out;
		VirtualQuery(ptr, &out, sizeof out);

		return out.Type
			&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			&& out.Protect & protect_flags;
	}

	auto get_export(const char* module_name, const char* export_name) -> void*
	{
		HMODULE mod;
		while (!((mod = GetModuleHandleA(module_name))))
			Sleep(100);

		return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
	}
}