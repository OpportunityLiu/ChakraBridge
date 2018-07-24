#pragma once
#include "RawRef.h"

namespace Opportunity::ChakraBridge::WinRT
{
    struct RawValue;

    struct RawPropertyId sealed : public RawRcRef
    {
        static constexpr RawPropertyId Invalid() { return RawPropertyId(); }

        constexpr RawPropertyId() :RawRcRef() {}
        constexpr RawPropertyId(JsPropertyIdRef ref) : RawRcRef(std::move(ref)) {}

        RawPropertyId(const wchar_t* name)
        {
            CHAKRA_CALL(JsGetPropertyIdFromName(name, &Ref));
        }
        RawPropertyId(const RawValue& symbol);

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