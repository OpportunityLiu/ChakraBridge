#pragma once

namespace Opportunity::ChakraBridge::WinRT
{
    using OverloadAttribute = Windows::Foundation::Metadata::OverloadAttribute;
    using DefaultOverloadAttribute = Windows::Foundation::Metadata::DefaultOverloadAttribute;
    using DefaultAttribute = Windows::Foundation::Metadata::DefaultAttribute;

    using object = ::Platform::Object;
    using string = ::Platform::String;
}

template<typename T>
inline void __NULL_CHECK(T^ v, const wchar_t* message)
{
    if (v == nullptr)
        throw ref new Platform::InvalidArgumentException(ref new ::Platform::String(message));
}

template<>
inline void __NULL_CHECK<::Platform::String>(::Platform::String^ v, const wchar_t* message)
{
    if (v->IsEmpty())
        throw ref new Platform::InvalidArgumentException(ref new ::Platform::String(message));
}

#define NULL_CHECK(prop) __NULL_CHECK(prop, _CRT_WIDE(_CRT_STRINGIZE(prop)) L" is null.")

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)
#define INHERIT_INTERFACE_R_PROPERTY(pname, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; }}
#define INHERIT_INTERFACE_RW_PROPERTY(pname, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; } void set(ptype value) = pint::pname::set { pname = value; }}

#define INHERIT_INTERFACE_METHOD_PARAM1(pname, ptype, pint, p1t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1) = pint::pname { return pname(a1); }
#define INHERIT_INTERFACE_METHOD_PARAM2(pname, ptype, pint, p1t, p2t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2) = pint::pname { return pname(a1, a2); } 
#define INHERIT_INTERFACE_METHOD_PARAM3(pname, ptype, pint, p1t, p2t, p3t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2, p3t a3) = pint::pname { return pname(a1, a2, a3); } 
#define INHERIT_INTERFACE_METHOD(pname, ptype, pint) virtual ptype UNIQUE_NAME(__##pname##__)() = pint::pname { return pname(); } 

#define INHERIT_INTERFACE_R_PROPERTY_EXPLICT(pname, ename, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return ename; }}
#define INHERIT_INTERFACE_RW_PROPERTY_EXPLICT(pname, ename, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return ename; } void set(ptype value) = pint::pname::set { ename = value; }}

#define INHERIT_INTERFACE_METHOD_PARAM1_EXPLICT(pname, ename, ptype, pint, p1t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1) = pint::pname { return ename(a1); }
#define INHERIT_INTERFACE_METHOD_PARAM2_EXPLICT(pname, ename, ptype, pint, p1t, p2t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2) = pint::pname { return ename(a1, a2); } 
#define INHERIT_INTERFACE_METHOD_PARAM3_EXPLICT(pname, ename, ptype, pint, p1t, p2t, p3t) virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2, p3t a3) = pint::pname { return ename(a1, a2, a3); } 
#define INHERIT_INTERFACE_METHOD_EXPLICT(pname, ename, ptype, pint) virtual ptype UNIQUE_NAME(__##pname##__)() = pint::pname { return ename(); } 