#pragma once
#include "RawRuntime.h"
#include "RawContext.h"
#include "RawPropertyId.h"
#include "RawValue.h"


namespace Opportunity::ChakraBridge::WinRT 
{
    inline RawPropertyId::RawPropertyId(const RawValue& symbol)
    {
        CHAKRA_CALL(JsGetPropertyIdFromSymbol(symbol.Ref, &Ref));
    }

    inline RawValue RawPropertyId::Symbol() const
    {
        RawValue r;
        CHAKRA_CALL(JsGetSymbolFromPropertyId(Ref, &r.Ref));
        return r;
    }

    inline RawContext RawValue::Context() const
    {
        RawContext ref;
        CHAKRA_CALL(JsGetContextOfObject(Ref, &ref.Ref));
        return ref;
    }
}