#pragma once
#include "JsEnum.h"
#include "JsRuntime\JsRuntime.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public ref class JsContext sealed
    {
    internal:
        JsContextRef Reference;
        JsContext(JsContextRef ref);
    public:
        virtual ~JsContext();

        property JsRuntime^ Runtime {
            JsRuntime^ get();
        }
    };
}
