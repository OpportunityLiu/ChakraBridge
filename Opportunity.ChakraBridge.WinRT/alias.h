#pragma once
#include <type_traits>

namespace Opportunity::ChakraBridge::WinRT
{
    using OverloadAttribute = Windows::Foundation::Metadata::OverloadAttribute;
    using DefaultOverloadAttribute = Windows::Foundation::Metadata::DefaultOverloadAttribute;
    using DefaultAttribute = Windows::Foundation::Metadata::DefaultAttribute;

    using JsType = Opportunity::ChakraBridge::WinRT::JsValueType;

    template<typename T>
    using remove_hat_t = typename ::Platform::Details::__remove_hat<T>::type;

    template<typename T>
    struct __add_hat_if_winrt
    {
        using type = remove_hat_t<T>^;
    };
    template<> struct __add_hat_if_winrt<bool> { using type = bool; };
    template<> struct __add_hat_if_winrt<::default::char16> { using type = ::default::char16; };
    template<> struct __add_hat_if_winrt<::default::int8> { using type = ::default::int8; };
    template<> struct __add_hat_if_winrt<::default::int16> { using type = ::default::int16; };
    template<> struct __add_hat_if_winrt<::default::int32> { using type = ::default::int32; };
    template<> struct __add_hat_if_winrt<::default::int64> { using type = ::default::int64; };
    template<> struct __add_hat_if_winrt<::default::uint8> { using type = ::default::uint8; };
    template<> struct __add_hat_if_winrt<::default::uint16> { using type = ::default::uint16; };
    template<> struct __add_hat_if_winrt<::default::uint32> { using type = ::default::uint32; };
    template<> struct __add_hat_if_winrt<::default::uint64> { using type = ::default::uint64; };
    template<> struct __add_hat_if_winrt<::default::float32> { using type = ::default::float32; };
    template<> struct __add_hat_if_winrt<::default::float64> { using type = ::default::float64; };

    template<typename T>
    using add_hat_t = typename __add_hat_if_winrt<T>::type;

    template<typename T>
    struct __winrt_collections
    {
        using element_t = add_hat_t<T>;
        using vector = ::Windows::Foundation::Collections::IVector<element_t>;
        using vector_view = ::Windows::Foundation::Collections::IVectorView<element_t>;
        using iterable = ::Windows::Foundation::Collections::IIterable<element_t>;
        using iterator = ::Windows::Foundation::Collections::IIterator<element_t>;
    };
    template<typename K, typename V>
    struct __winrt_collections_2
    {
        using k_t = add_hat_t<K>;
        using v_t = add_hat_t<V>;
        using map = ::Windows::Foundation::Collections::IMap<k_t, v_t>;
        using map_view = ::Windows::Foundation::Collections::IMapView<k_t, v_t>;
        using kv_pair = ::Windows::Foundation::Collections::IKeyValuePair<k_t, v_t>;
    };

    using weak_ref = typename ::Platform::WeakReference;

    using object = ::Platform::Object;
    using string = ::Platform::String;

    template<typename T>
    using array = typename ::Platform::Array<add_hat_t<T>>;
    template<typename T>
    using write_only_array = typename ::Platform::WriteOnlyArray<add_hat_t<T>>;

    template<typename T>
    using vector = typename __winrt_collections<T>::vector;
    template<typename T>
    using vector_view = typename __winrt_collections<T>::vector_view;
    template<typename T>
    using iterable = typename __winrt_collections<T>::iterable;
    template<typename T>
    using iterator = typename __winrt_collections<T>::iterator;
    template<typename K, typename V>
    using map = typename __winrt_collections_2<K, V>::map;
    template<typename K, typename V>
    using map_view = typename __winrt_collections_2<K, V>::map_view;
    template<typename K, typename V>
    using kv_pair = typename __winrt_collections_2<K, V>::kv_pair;
}

#define NULL_CHECK(prop) \
do{\
    if constexpr (std::is_same_v<decltype(prop), ::Platform::String^>)\
    {\
        if ((prop) == nullptr)\
            throw ref new ::Platform::InvalidArgumentException(_CRT_WIDE(_CRT_STRINGIZE(prop)) L" is null or empty.");\
    }\
    else\
    {\
        if ((prop) == nullptr)\
            throw ref new ::Platform::InvalidArgumentException(_CRT_WIDE(_CRT_STRINGIZE(prop)) L" is null.");\
    }\
}while (false)

#define DECL_R_PROPERTY(propType, propName) property propType propName { propType get(); }
#define DECL_RW_PROPERTY(propType, propName) property propType propName { propType get(); void set(propType); }

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_NAME(base) PP_CAT(base, __LINE__)

#define INHERIT_INTERFACE_R_PROPERTY(pname, ptype, pint)\
virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; }}
#define INHERIT_INTERFACE_RW_PROPERTY(pname, ptype, pint)\
virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return pname; } void set(ptype value) = pint::pname::set { pname = value; }}

#define INHERIT_INTERFACE_METHOD(pname, ptype, pint)\
virtual ptype UNIQUE_NAME(__##pname##__)() = pint::pname { return pname(); } 
#define INHERIT_INTERFACE_METHOD_PARAM1(pname, ptype, pint, p1t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1) = pint::pname { return pname(a1); }
#define INHERIT_INTERFACE_METHOD_PARAM2(pname, ptype, pint, p1t, p2t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2) = pint::pname { return pname(a1, a2); } 
#define INHERIT_INTERFACE_METHOD_PARAM3(pname, ptype, pint, p1t, p2t, p3t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2, p3t a3) = pint::pname { return pname(a1, a2, a3); } 

#define INHERIT_INTERFACE_R_PROPERTY_EXPLICT(pname, ename, ptype, pint)\
virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return ename; }}
#define INHERIT_INTERFACE_RW_PROPERTY_EXPLICT(pname, ename, ptype, pint)\
virtual property ptype UNIQUE_NAME(__##pname##__) { ptype get() = pint::pname::get { return ename; } void set(ptype value) = pint::pname::set { ename = value; }}

#define INHERIT_INTERFACE_METHOD_EXPLICT(pname, ename, ptype, pint)\
virtual ptype UNIQUE_NAME(__##pname##__)() = pint::pname { return ename(); } 
#define INHERIT_INTERFACE_METHOD_PARAM1_EXPLICT(pname, ename, ptype, pint, p1t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1) = pint::pname { return ename(a1); }
#define INHERIT_INTERFACE_METHOD_PARAM2_EXPLICT(pname, ename, ptype, pint, p1t, p2t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2) = pint::pname { return ename(a1, a2); } 
#define INHERIT_INTERFACE_METHOD_PARAM3_EXPLICT(pname, ename, ptype, pint, p1t, p2t, p3t)\
virtual ptype UNIQUE_NAME(__##pname##__)(p1t a1, p2t a2, p3t a3) = pint::pname { return ename(a1, a2, a3); } 