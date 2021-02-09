#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <cstdint>
#include <utility>
#include <array>
#include <algorithm>

namespace helpers
{
    namespace detail
    {
        constexpr auto process_pattern_byte(const std::pair<char, char> pair) -> std::pair<std::uint8_t, bool>
        {
            return { static_cast<std::uint8_t>(pair.first), pair.second == 'x' };
        }

        template <std::size_t N>
        constexpr auto select_pattern_byte(const char(&pattern)[N], const char(&mask)[N],
            std::size_t n) -> std::pair<char, char>
        {
            return { pattern[n], mask[n] };
        }

        template <typename>
        struct sig_processor
        {
        };

        template <std::size_t... Indices>
        struct sig_processor<std::index_sequence<Indices...>>
        {
            template <std::size_t N>
            static constexpr auto process(const char(&pattern)[N],
                const char(&mask)[N])->std::array<std::pair<std::uint8_t, bool>, N>
            {
                return { process_pattern_byte(select_pattern_byte(pattern, mask, Indices))... };
            }
        };
    }

    auto get_interface(const char* module_name, const char* interface_name) -> void*;
    auto get_module_info(const char* module_name)->std::pair<std::uintptr_t, std::size_t>;
    auto is_code_ptr(void* ptr) -> bool;
    auto get_export(const char* module_name, const char* export_name) -> void*;

    template <std::size_t N>
    auto find_pattern(const char* module_name, const char(&pattern)[N], const char(&mask)[N]) -> std::uintptr_t
    {
        const std::array<std::pair<std::uint8_t, bool>, N> signature = detail::sig_processor<std::make_index_sequence<N>
        >::process(pattern, mask);

        const auto [fst, snd] = get_module_info(module_name);
        const auto address = reinterpret_cast<std::uint8_t*>(fst);
        const auto size = snd;

        const auto ret = std::search(address, address + size, signature.begin(), signature.end(),
            [](const std::uint8_t curr, const std::pair<std::uint8_t, bool> curr_pattern)
        {
            return (!curr_pattern.second) || curr == curr_pattern.first;
        });

        return ret == address + size ? 0 : reinterpret_cast<std::uintptr_t>(ret);
    }
}