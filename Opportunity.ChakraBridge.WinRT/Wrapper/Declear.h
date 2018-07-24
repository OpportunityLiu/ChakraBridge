#pragma once
#include "RawRuntime.h"
#include "RawContext.h"
#include "RawPropertyId.h"
#include "RawValue.h"


namespace Opportunity::ChakraBridge::WinRT 
{
    inline void RawContext::SetException(const RawValue& exception)
    {
        CHAKRA_CALL(JsSetException(exception.Ref));
    }

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
}