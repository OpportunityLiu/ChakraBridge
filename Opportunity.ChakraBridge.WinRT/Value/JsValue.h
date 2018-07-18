#pragma once
#include "PreDeclare.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)
#define INHERIT_INTERFACE_R_PROPERTY(pname, ptype, pint) virtual property ptype UNIQUE_NAME(__##ptype##_##pname##__) { ptype get() = pint::pname::get { return pname; }}

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript value.
    /// </summary>
    /// <remarks>
    /// A JavaScript value is one of the following types of values: Undefined, Null, Boolean, 
    /// String, Number, or Object.
    /// </remarks>
    public interface class IJsValue: Windows::Foundation::IStringable
    {
        property JsValueType Type
        {
            JsValueType get();
        }
    };

    ref class JsValueImpl abstract : IJsValue
    {
    private:
    internal:
        JsValueRef Reference;
        JsValueImpl(JsValueRef ref);
    public:
        virtual Platform::String^ ToString() override = 0;
        virtual property JsValueType Type
        {
            JsValueType get();
        }
    };

    /// <summary>
    /// Static methods of <see cref="IJsValue"/>.
    /// </summary>
    public ref class JsValue sealed
    {
    private:
        JsValue() {}
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
