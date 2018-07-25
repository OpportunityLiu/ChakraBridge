#pragma once
#include "Wrapper\Declear.h"

namespace Opportunity::ChakraBridge::WinRT
{
    interface class IJsValue;
    interface class IJsUndefined;
    interface class IJsNull;
    interface class IJsNumber;
    interface class IJsString;
    interface class IJsBoolean;
    interface class IJsObject;
    interface class IJsExternalObject;
    interface class IJsFunction;
    interface class IJsError;
    interface class IJsArray;
    interface class IJsSymbol;
    interface class IJsArrayBuffer;
    interface class IJsTypedArray;
    interface class IJsDataView;

    ref class JsValueImpl;
    ref class JsUndefinedImpl;
    ref class JsNullImpl;
    ref class JsNumberImpl;
    ref class JsStringImpl;
    ref class JsBooleanImpl;
    ref class JsObjectImpl;
    ref class JsExternalObjectImpl;
    ref class JsFunctionImpl;
    ref class JsErrorImpl;
    ref class JsArrayImpl;
    ref class JsSymbolImpl;
    ref class JsArrayBufferImpl;
    ref class JsTypedArrayImpl;
    ref class JsDataViewImpl;

    [[nodiscard]]inline JsValueImpl^ to_impl(IJsValue^ inter) { return safe_cast<JsValueImpl^>(inter); }
    [[nodiscard]]inline JsUndefinedImpl^ to_impl(IJsUndefined^ inter) { return safe_cast<JsUndefinedImpl^>(inter); }
    [[nodiscard]]inline JsNullImpl^ to_impl(IJsNull^ inter) { return safe_cast<JsNullImpl^>(inter); }
    [[nodiscard]]inline JsNumberImpl^ to_impl(IJsNumber^ inter) { return safe_cast<JsNumberImpl^>(inter); }
    [[nodiscard]]inline JsStringImpl^ to_impl(IJsString^ inter) { return safe_cast<JsStringImpl^>(inter); }
    [[nodiscard]]inline JsBooleanImpl^ to_impl(IJsBoolean^ inter) { return safe_cast<JsBooleanImpl^>(inter); }
    [[nodiscard]]inline JsObjectImpl^ to_impl(IJsObject^ inter) { return safe_cast<JsObjectImpl^>(inter); }
    [[nodiscard]]inline JsExternalObjectImpl^ to_impl(IJsExternalObject^ inter) { return safe_cast<JsExternalObjectImpl^>(inter); }
    [[nodiscard]]inline JsFunctionImpl^ to_impl(IJsFunction^ inter) { return safe_cast<JsFunctionImpl^>(inter); }
    [[nodiscard]]inline JsErrorImpl^ to_impl(IJsError^ inter) { return safe_cast<JsErrorImpl^>(inter); }
    [[nodiscard]]inline JsArrayImpl^ to_impl(IJsArray^ inter) { return safe_cast<JsArrayImpl^>(inter); }
    [[nodiscard]]inline JsSymbolImpl^ to_impl(IJsSymbol^ inter) { return safe_cast<JsSymbolImpl^>(inter); }
    [[nodiscard]]inline JsArrayBufferImpl^ to_impl(IJsArrayBuffer^ inter) { return safe_cast<JsArrayBufferImpl^>(inter); }
    [[nodiscard]]inline JsTypedArrayImpl^ to_impl(IJsTypedArray^ inter) { return safe_cast<JsTypedArrayImpl^>(inter); }
    [[nodiscard]]inline JsDataViewImpl^ to_impl(IJsDataView^ inter) { return safe_cast<JsDataViewImpl^>(inter); }
};
