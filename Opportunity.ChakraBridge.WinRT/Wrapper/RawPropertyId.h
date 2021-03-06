﻿#pragma once
#include "PreDeclear.h"
#include "RawRef.h"

namespace Opportunity::ChakraBridge::WinRT
{
    struct[[nodiscard]] RawPropertyId sealed : public RawRcRef<JsPropertyIdRef>
    {
        const wchar_t* Name() const
        {
            const wchar_t* r;
            CHAKRA_CALL(JsGetPropertyNameFromId(Ref, &r));
            return r;
        }

        RawValue Symbol() const;

        JsPropertyIdType Type() const
        {
            JsPropertyIdType r;
            CHAKRA_CALL(JsGetPropertyIdType(Ref, &r));
            return r;
        }

        RawPropertyId(const wchar_t* name)
        {
            CHAKRA_CALL(JsGetPropertyIdFromName(name, &Ref));
        }
        RawPropertyId(string^ const name)
            :RawPropertyId(name->Data()) {}
        RawPropertyId(const RawValue& symbol);

        constexpr RawPropertyId(std::nullptr_t) : RawRcRef(JS_INVALID_REFERENCE) {}
        constexpr RawPropertyId() :RawRcRef(JS_INVALID_REFERENCE) {}
        explicit constexpr RawPropertyId(JsPropertyIdRef ref) : RawRcRef(std::move(ref)) {}
    };

    static_assert(sizeof(RawPropertyId) == sizeof(JsPropertyIdRef));

};

template<> struct ::std::hash<Opportunity::ChakraBridge::WinRT::RawPropertyId>
{
    constexpr size_t operator()(const Opportunity::ChakraBridge::WinRT::RawPropertyId& v) const
    {
        return reinterpret_cast<size_t>(v.Ref);
    }
};