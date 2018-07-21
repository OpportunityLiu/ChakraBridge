#include "pch.h"
#include "JsObject.h"
#include <vector>

using namespace Opportunity::ChakraBridge::WinRT;

JsObjectImpl::~JsObjectImpl()
{
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
}

JsObjectImpl::JsObjectImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

IJsValue^ JsObjectImpl::Lookup(string^ key)
{
    NULL_CHECK(key);
    return JsValue::CreateTyped(RawGetProperty(Reference, key->Data()));
}

IJsValue^ JsObjectImpl::Lookup(IJsSymbol^ key)
{
    NULL_CHECK(key);
    return JsValue::CreateTyped(RawGetProperty(Reference, to_impl(key)->Reference));
}

bool JsObjectImpl::HasKey(string^ key)
{
    NULL_CHECK(key);
    bool r;
    CHAKRA_CALL(JsHasProperty(Reference, RawGetPropertyId(key->Data()), &r));
    return r;
}

bool JsObjectImpl::HasKey(IJsSymbol^ key)
{
    NULL_CHECK(key);
    bool r;
    CHAKRA_CALL(JsHasIndexedProperty(Reference, to_impl(key)->Reference, &r));
    return r;
}

bool JsObjectImpl::Insert(string^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    auto vref = value != nullptr ? to_impl(value)->Reference : RawNull();
    CHAKRA_CALL(JsSetProperty(Reference, RawGetPropertyId(key->Data()), vref, true));
    return r;
}

bool JsObjectImpl::Insert(IJsSymbol^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    auto vref = value != nullptr ? to_impl(value)->Reference : RawNull();
    CHAKRA_CALL(JsSetIndexedProperty(Reference, to_impl(key)->Reference, vref));
    return r;
}

void JsObjectImpl::Remove(string^ key)
{
    NULL_CHECK(key);
    JsValueRef r;
    CHAKRA_CALL(JsDeleteProperty(Reference, RawGetPropertyId(key->Data()), true, &r));
}

void JsObjectImpl::Remove(IJsSymbol^ key)
{
    NULL_CHECK(key);
    CHAKRA_CALL(JsDeleteIndexedProperty(Reference, to_impl(key)->Reference));
}

void JsObjectImpl::StrClear()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    for (int i = 0; i < n; i++)
    {
        auto pname = RawGetProperty(r, RawIntToNumber(i));
        CHAKRA_CALL(JsDeleteIndexedProperty(Reference, pname));
    }
}

void JsObjectImpl::SymClear()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertySymbols(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    for (int i = 0; i < n; i++)
    {
        auto pname = RawGetProperty(r, RawIntToNumber(i));
        CHAKRA_CALL(JsDeleteIndexedProperty(Reference, pname));
    }
}

JsObjectImpl::IStrIterator^ JsObjectImpl::StrFirst()
{
    return GetStrView()->First();
}

JsObjectImpl::ISymIterator^ JsObjectImpl::SymFirst()
{
    return GetSymView()->First();
}

JsObjectImpl::IStrMapView^ JsObjectImpl::GetStrView()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    auto view = ref new Platform::Collections::UnorderedMap<string^, IJsValue^>(static_cast<size_t>(n));
    for (int i = 0; i < n; i++)
    {
        auto kf = RawGetProperty(r, RawIntToNumber(i));
        auto k = RawStringToPointer(kf);
        view->Insert(k, JsValue::CreateTyped(RawGetProperty(Reference, kf)));
    }
    return view->GetView();
}

JsObjectImpl::ISymMapView^ JsObjectImpl::GetSymView()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    auto view = ref new Platform::Collections::UnorderedMap<IJsSymbol^, IJsValue^>(static_cast<size_t>(n));
    for (int i = 0; i < n; i++)
    {
        auto kf = RawGetProperty(r, RawIntToNumber(i));
        view->Insert(ref new JsSymbolImpl(kf), JsValue::CreateTyped(RawGetProperty(Reference, kf)));
    }
    return view->GetView();
}

uint32 JsObjectImpl::StrSize::get()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    return static_cast<uint32>(RawNumberToInt(RawGetProperty(r, L"length")));
}

uint32 JsObjectImpl::SymSize::get()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    return static_cast<uint32>(RawNumberToInt(RawGetProperty(r, L"length")));
}

string^ JsObjectImpl::ToString()
{
    JsValueRef strref;
    if (JsConvertValueToString(Reference, &strref) == JsNoError)
    {
        return RawStringToPointer(strref);
    }
    // no toString method.
    return "[object Object]";
}

void JsObjectImpl::PreventExtension()
{
    CHAKRA_CALL(JsPreventExtension(Reference));
}

bool JsObjectImpl::IsExtensionAllowed::get()
{
    bool r;
    CHAKRA_CALL(JsGetExtensionAllowed(Reference, &r));
    return r;
}

IJsObject^ JsObjectImpl::Prototype::get()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetPrototype(Reference, &r));
    return dynamic_cast<IJsObject^>(JsValue::CreateTyped(r));
}

void JsObjectImpl::Prototype::set(IJsObject^ value)
{
    if (value == nullptr)
    {
        CHAKRA_CALL(JsSetPrototype(Reference, RawNull()));
        return;
    }
    CHAKRA_CALL(JsSetPrototype(Reference, to_impl(value)->Reference));
}

using JsOBCC = Opportunity::ChakraBridge::WinRT::JsObjectBeforeCollectCallback;
std::unordered_map<JsValueRef, JsOBCC^> OBCCMap;

void CALLBACK JsObjectBeforeCollectCallbackImpl(_In_ JsRef ref, _In_opt_ void *callbackState)
{
    auto v = OBCCMap.find(ref);
    if (v == OBCCMap.end())
        return;
    auto f = v->second;
    OBCCMap.erase(ref);
    f(safe_cast<IJsObject^>(JsValue::CreateTyped(ref)));
}

JsOBCC^ JsObjectImpl::ObjectCollectingCallback::get()
{
    auto v = OBCCMap.find(Reference);
    if (v == OBCCMap.end())
        return nullptr;
    return v->second;
}

void JsObjectImpl::ObjectCollectingCallback::set(JsOBCC^ value)
{
    if (value == nullptr)
    {
        CHAKRA_CALL(JsSetObjectBeforeCollectCallback(Reference, Reference, nullptr));
        OBCCMap.erase(Reference);
        return;
    }
    CHAKRA_CALL(JsSetObjectBeforeCollectCallback(Reference, Reference, JsObjectBeforeCollectCallbackImpl));
    OBCCMap[Reference] = value;
}

IJsObject^ JsObject::Create()
{
    JsValueRef obj;
    CHAKRA_CALL(JsCreateObject(&obj));
    return ref new JsObjectImpl(obj);
}

bool JsObject::InstanceOf(IJsObject ^ obj, IJsFunction ^ constructor)
{
    NULL_CHECK(obj);
    NULL_CHECK(constructor);
    bool r;
    CHAKRA_CALL(JsInstanceOf(to_impl(obj)->Reference, to_impl(obj)->Reference, &r));
    return r;
}

bool InnerDefineProperty(JsObjectImpl^ obj, JsPropertyIdRef propertyId, JsObjectImpl^ descriptor)
{
    NULL_CHECK(obj);
    NULL_CHECK(descriptor);
    bool r;
    CHAKRA_CALL(JsDefineProperty(obj->Reference, propertyId, descriptor->Reference, &r));
    return r;
}

bool JsObject::DefineProperty(IJsObject^ obj, string^ propertyId, IJsObject^ descriptor)
{
    return InnerDefineProperty(to_impl(obj), RawGetPropertyId(propertyId->Data()), to_impl(descriptor));
}

bool JsObject::DefineProperty(IJsObject^ obj, IJsSymbol^ propertyId, IJsObject^ descriptor)
{
    NULL_CHECK(propertyId);
    return InnerDefineProperty(to_impl(obj), RawGetPropertyId(to_impl(propertyId)->Reference), to_impl(descriptor));
}

IJsArray^ JsObject::GetOwnPropertySymbols(IJsObject^ obj)
{
    NULL_CHECK(obj);
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertySymbols(to_impl(obj)->Reference, &r));
    return ref new JsArrayImpl(r);
}

IJsArray^ JsObject::GetOwnPropertyNames(IJsObject^ obj)
{
    NULL_CHECK(obj);
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(to_impl(obj)->Reference, &r));
    return ref new JsArrayImpl(r);
}

IJsObject^ JsObject::GetOwnPropertyDescriptor(IJsObject^ obj, string^ propertyId)
{
    NULL_CHECK(obj);
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyDescriptor(to_impl(obj)->Reference, RawGetPropertyId(propertyId->Data()), &r));
    return safe_cast<IJsObject^>(JsValue::CreateTyped(r));
}

IJsObject^ JsObject::GetOwnPropertyDescriptor(IJsObject^ obj, IJsSymbol^ propertyId)
{
    NULL_CHECK(obj);
    NULL_CHECK(propertyId);
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyDescriptor(to_impl(obj)->Reference, RawGetPropertyId(to_impl(propertyId)->Reference), &r));
    return safe_cast<IJsObject^>(JsValue::CreateTyped(r));
}
