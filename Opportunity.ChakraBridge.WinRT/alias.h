#pragma once

namespace Opportunity::ChakraBridge::WinRT
{
    using OverloadAttribute = Windows::Foundation::Metadata::OverloadAttribute;
    using DefaultOverloadAttribute = Windows::Foundation::Metadata::DefaultOverloadAttribute;
    using DefaultAttribute = Windows::Foundation::Metadata::DefaultAttribute;

    using object = ::Platform::Object;
    using string = ::Platform::String;
}

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)
#define INHERIT_INTERFACE_R_PROPERTY(pname, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; }}
#define INHERIT_INTERFACE_RW_PROPERTY(pname, ptype, pint) virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; } void set(ptype value) = pint::pname::set { pname = value; }}
#define INHERIT_INTERFACE_METHOD(pname, ptype, pint) virtual ptype UNIQUE_NAME(__##pname##__)() = pint::pname { return pname(); } 