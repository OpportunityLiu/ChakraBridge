#pragma once
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsValue
    {
    };

    ref class JsValueImpl abstract : IJsValue
    {
    private:
    internal:
        JsValueRef Reference;
        JsValueImpl(JsValueRef ref);
    };

    static public ref class JsValue sealed
    {
    internal:
        static JsValueImpl^ CreateTyped(JsValueRef ref);
    public:
        /// <summary>
        /// Gets the global object in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static property IJsObject^ GlobalObject
        {
            IJsObject^ get();
        }
    };
}
