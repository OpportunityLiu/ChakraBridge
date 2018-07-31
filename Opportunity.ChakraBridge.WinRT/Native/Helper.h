#pragma once
#include "alias.h"
#include <jsrt.h>
#include <limits>
#include <vector>

namespace Opportunity::ChakraBridge::WinRT
{
    string_ref CHAKRA_LAST_ERROR();

    inline void __CHAKRA_CALL(const JsErrorCode result, const wchar_t* expr, const int line, const wchar_t* file)
    {
        [[noreturn]] void __CHARKA_CALL_THROW(const JsErrorCode result, const wchar_t* expr, const int line, const wchar_t* file);
        if (result == JsErrorCode::JsNoError)
            return;
        __CHARKA_CALL_THROW(result, expr, line, file);
    }

#define CHAKRA_CALL(expr) __CHAKRA_CALL((expr), _CRT_WIDE(_CRT_STRINGIZE(expr)), __LINE__, _CRT_WIDE(__FILE__))
}