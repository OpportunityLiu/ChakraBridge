#pragma once
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public ref class JsValue sealed
    {
    private:
    internal:
        JsValueRef Reference;
        JsValue(JsValueRef ref);
    public:
    };
}
