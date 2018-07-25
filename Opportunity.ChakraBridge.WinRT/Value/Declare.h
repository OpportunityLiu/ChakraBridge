#pragma once
#include "JsValue.h"

#include "JsUndefined.h"
#include "JsNull.h"
#include "JsNumber.h"
#include "JsBoolean.h"
#include "JsString.h"
#include "JsSymbol.h"

#include "JsObject.h"
#include "JsExternalObject.h"
#include "JsArray.h"
#include "JsError.h"

#include "JsArrayBuffer.h"
#include "JsDataView.h"
#include "JsTypedArray.h"

namespace Opportunity::ChakraBridge::WinRT
{
    template<typename T>
    inline std::enable_if_t<std::is_base_of_v<IJsValue, T>, RawValue> get_ref(T^const inter)
    {
        if (inter == nullptr)
            return nullptr;
        return safe_cast<JsValueImpl^>(inter)->Reference;
    }

    template<typename T>
    inline RawValue get_ref_or_null(T^const inter)
    {
        auto r = get_ref(inter);
        if (r.IsValid())
            return r;
        return RawValue::Null();
    }
    template<typename T>
    inline RawValue get_ref_or_undefined(T^const inter)
    {
        auto r = get_ref(inter);
        if (r.IsValid())
            return r;
        return RawValue::Undefined();
    }
}

