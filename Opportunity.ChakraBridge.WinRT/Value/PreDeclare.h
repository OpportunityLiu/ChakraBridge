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

    inline JsValueImpl^ to_impl(IJsValue^ inter) { return safe_cast<JsValueImpl^>(inter); }
    inline JsUndefinedImpl^ to_impl(IJsUndefined^ inter) { return safe_cast<JsUndefinedImpl^>(inter); }
    inline JsNullImpl^ to_impl(IJsNull^ inter) { return safe_cast<JsNullImpl^>(inter); }
    inline JsNumberImpl^ to_impl(IJsNumber^ inter) { return safe_cast<JsNumberImpl^>(inter); }
    inline JsStringImpl^ to_impl(IJsString^ inter) { return safe_cast<JsStringImpl^>(inter); }
    inline JsBooleanImpl^ to_impl(IJsBoolean^ inter) { return safe_cast<JsBooleanImpl^>(inter); }
    inline JsObjectImpl^ to_impl(IJsObject^ inter) { return safe_cast<JsObjectImpl^>(inter); }
    inline JsExternalObjectImpl^ to_impl(IJsExternalObject^ inter) { return safe_cast<JsExternalObjectImpl^>(inter); }
    inline JsFunctionImpl^ to_impl(IJsFunction^ inter) { return safe_cast<JsFunctionImpl^>(inter); }
    inline JsErrorImpl^ to_impl(IJsError^ inter) { return safe_cast<JsErrorImpl^>(inter); }
    inline JsArrayImpl^ to_impl(IJsArray^ inter) { return safe_cast<JsArrayImpl^>(inter); }
    inline JsSymbolImpl^ to_impl(IJsSymbol^ inter) { return safe_cast<JsSymbolImpl^>(inter); }
    inline JsArrayBufferImpl^ to_impl(IJsArrayBuffer^ inter) { return safe_cast<JsArrayBufferImpl^>(inter); }
    inline JsTypedArrayImpl^ to_impl(IJsTypedArray^ inter) { return safe_cast<JsTypedArrayImpl^>(inter); }
    inline JsDataViewImpl^ to_impl(IJsDataView^ inter) { return safe_cast<JsDataViewImpl^>(inter); }
};
