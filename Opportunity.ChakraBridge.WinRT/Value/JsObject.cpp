#include "pch.h"
#include "JsObject.h"
#include <vector>
#include <sstream>

using namespace Opportunity::ChakraBridge::WinRT;

JsObjectImpl::~JsObjectImpl()
{
    // ignore error
    JsRelease(Reference.Ref, nullptr);
}

JsObjectImpl::JsObjectImpl(RawValue ref)
    :JsValueImpl(std::move(ref))
{
    Reference.AddRef();
}

IJsValue^ JsObjectImpl::Lookup(string^ key)
{
    return JsValue::CreateTyped(Reference[key->Data()]);
}

IJsValue^ JsObjectImpl::Lookup(IJsSymbol^ key)
{
    NULL_CHECK(key);
    return JsValue::CreateTyped(Reference[get_ref(key)]);
}

bool JsObjectImpl::HasKey(string^ key)
{
    return Reference[key->Data()].Exist();
}

bool JsObjectImpl::HasKey(IJsSymbol^ key)
{
    NULL_CHECK(key);
    return Reference[get_ref(key)].Exist();
}

bool JsObjectImpl::Insert(string^ key, IJsValue^ value)
{
    const auto prop = Reference[key->Data()];
    bool r = prop.Exist();
    prop = get_ref_or_undefined(value);
    return r;
}

bool JsObjectImpl::Insert(IJsSymbol^ key, IJsValue^ value)
{
    NULL_CHECK(key);
    const auto prop = Reference[get_ref(key)];
    bool r = prop.Exist();
    prop = get_ref_or_undefined(value);
    return r;
}

void JsObjectImpl::Remove(string^ key)
{
    void(Reference[key->Data()].Delete());
}

void JsObjectImpl::Remove(IJsSymbol^ key)
{
    NULL_CHECK(key);
    Reference[get_ref(key)].Delete();
}

void JsObjectImpl::StrClear()
{
    const auto arr = Reference.ObjOwnPropertyNames();
    const auto n = arr[L"length"]().ToInt();
    for (int i = 0; i < n; i++)
    {
        Reference[arr[RawValue(i)]].Delete();
    }
}

void JsObjectImpl::SymClear()
{
    const auto arr = Reference.ObjOwnPropertySymbols();
    const auto n = arr[L"length"]().ToInt();
    for (int i = 0; i < n; i++)
    {
        Reference[arr[RawValue(i)]].Delete();
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
    const auto arr = Reference.ObjOwnPropertyNames();
    const auto n = arr[L"length"]().ToInt();
    auto view = ref new Platform::Collections::UnorderedMap<string^, IJsValue^>(static_cast<size_t>(n));
    for (int i = 0; i < n; i++)
    {
        const auto kf = arr[RawValue(i)]();
        auto k = kf.ToString();
        view->Insert(k, JsValue::CreateTyped(Reference[kf]));
    }
    return view->GetView();
}

JsObjectImpl::ISymMapView^ JsObjectImpl::GetSymView()
{
    const auto arr = Reference.ObjOwnPropertySymbols();
    const auto n = arr[L"length"]().ToInt();
    auto view = ref new Platform::Collections::UnorderedMap<IJsSymbol^, IJsValue^>(static_cast<size_t>(n));
    for (int i = 0; i < n; i++)
    {
        const auto kf = arr[RawValue(i)]();
        view->Insert(ref new JsSymbolImpl(kf), JsValue::CreateTyped(Reference[kf]));
    }
    return view->GetView();
}

uint32 JsObjectImpl::StrSize::get()
{
    return static_cast<uint32>(Reference.ObjOwnPropertyNames()[L"length"]().ToInt());
}

uint32 JsObjectImpl::SymSize::get()
{
    return static_cast<uint32>(Reference.ObjOwnPropertySymbols()[L"length"]().ToInt());
}

string^ JsObjectImpl::ToString()
{
    try
    {
        return Reference.ToJsString().ToString();
    }
    catch (Platform::Exception^ ex)
    {
        std::wstringstream msg;
        msg << L'<' << CHAKRA_LAST_ERROR().Data() << L'>';
        return ref new string(msg.str().c_str());
    }
}

void JsObjectImpl::PreventExtension()
{
    Reference.ObjPreventExtension();
}

bool JsObjectImpl::IsExtensionAllowed::get()
{
    return Reference.ObjIsExtensionAllowed();
}

IJsObject^ JsObjectImpl::Proto::get()
{
    return dynamic_cast<IJsObject^>(JsValue::CreateTyped(Reference.ObjProto()));
}

void JsObjectImpl::Proto::set(IJsObject^ value)
{
    if (value == nullptr)
        Reference.ObjProto(RawValue::Undefined());
    else
        Reference.ObjProto(to_impl(value)->Reference);
}

std::unordered_map<RawValue, std::unique_ptr<JsObjectImpl::OW>> JsObjectImpl::OBCCMap;

void JsObjectImpl::JsObjectBeforeCollectCallbackImpl(const RawValue& ref, const OWP& callbackState)
{
    _ASSERTE(callbackState != nullptr);
    const auto state = std::move(OBCCMap.at(ref));
    OBCCMap.erase(ref);
    _ASSERTE(callbackState == state.get());
    try
    {
        if (state->InternalBeforeCollectCallback)
            state->InternalBeforeCollectCallback(ref);
    }
    catch (...)
    {
        _ASSERT_EXPR(false, "Unexpected exception!");
    }
    const auto cbfunc = state->BeforeCollectCallback;
    if (cbfunc != nullptr)
    {
        auto obj = callbackState->Object.Resolve<JsObjectImpl>();
        if (obj == nullptr)
            obj = safe_cast<JsObjectImpl^>(JsValue::CreateTyped(ref));
        cbfunc(obj);
    }
}

void JsObjectImpl::RegisterInternalBeforeCollectCallback(IBCC*const callback)
{
    auto v = OBCCMap.find(Reference);
    const auto hasValue = (v != OBCCMap.end());

    if (hasValue)
    {
        v->second->Object = this;
        v->second->InternalBeforeCollectCallback = callback;

        if (!v->second->InUse())
        {
            Reference.ObjBeforeCollectCallback(nullptr);
            OBCCMap.erase(Reference);
        }
    }
    else
    {
        if (callback == nullptr)
            return;
        auto newValue = std::make_unique<OW>(this, nullptr, callback);
        Reference.ObjBeforeCollectCallback<OWP, JsObjectBeforeCollectCallbackImpl>(newValue.get());
        OBCCMap[Reference] = std::move(newValue);
    }
}

void JsObjectImpl::ObjectCollectingCallback::set(JsOBCC^ value)
{
    auto v = OBCCMap.find(Reference);
    const auto hasValue = (v != OBCCMap.end());

    if (hasValue)
    {
        v->second->Object = this;
        v->second->BeforeCollectCallback = value;

        if (!v->second->InUse())
        {
            Reference.ObjBeforeCollectCallback(nullptr);
            OBCCMap.erase(Reference);
        }
    }
    else
    {
        if (value == nullptr)
            return;
        auto newValue = std::make_unique<OW>(this, value, nullptr);
        Reference.ObjBeforeCollectCallback<OWP, JsObjectBeforeCollectCallbackImpl>(newValue.get());
        OBCCMap[Reference] = std::move(newValue);
    }
}

JsObjectImpl::JsOBCC^ JsObjectImpl::ObjectCollectingCallback::get()
{
    auto v = OBCCMap.find(Reference);
    if (v == OBCCMap.end())
        return nullptr;
    v->second->Object = this;
    return v->second->BeforeCollectCallback;
}

IJsObject^ JsObject::Create()
{
    return ref new JsObjectImpl(RawValue::CreateObject());
}

IJsObject^ JsObject::Create(map_view<string, IJsValue>^ properties)
{
    auto obj = Create();
    if (properties == nullptr)
        return obj;
    const auto ref = get_ref(obj);
    const auto udf = RawValue::Undefined();
    for (const auto var : properties)
    {
        const auto value = var->Value;
        if (value == nullptr)
            ref[var->Key->Data()] = udf;
        else
            ref[var->Key->Data()] = get_ref(value);
    }
    return obj;
}

IJsObject^ JsObject::Create(map_view<IJsValue, IJsValue>^ properties)
{
    auto obj = Create();
    if (properties == nullptr)
        return obj;
    const auto ref = get_ref(obj);
    const auto udf = RawValue::Undefined();
    for (const auto var : properties)
    {
        const auto key = var->Key;
        const auto keyref = get_ref_or_null(key);
        const auto value = var->Value;
        if (value == nullptr)
            ref[keyref] = udf;
        else
            ref[keyref] = get_ref(value);
    }
    return obj;
}

bool JsObject::InstanceOf(IJsObject^ obj, IJsFunction^ constructor)
{
    NULL_CHECK(obj);
    NULL_CHECK(constructor);
    return get_ref(obj).ObjInstanceOf(get_ref(constructor));
}

bool InnerDefineProperty(IJsObject^ obj, RawPropertyId propertyId, IJsObject^ descriptor)
{
    NULL_CHECK(obj);
    NULL_CHECK(descriptor);
    return get_ref(obj)[propertyId].Define(get_ref(descriptor));
}

bool JsObject::DefineProperty(IJsObject^ obj, string^ propertyId, IJsObject^ descriptor)
{
    return InnerDefineProperty(obj, RawPropertyId(propertyId->Data()), descriptor);
}

bool JsObject::DefineProperty(IJsObject^ obj, IJsSymbol^ propertyId, IJsObject^ descriptor)
{
    NULL_CHECK(propertyId);
    return InnerDefineProperty(obj, RawPropertyId(get_ref(propertyId)), descriptor);
}

IJsArray^ JsObject::GetOwnPropertyNames(IJsObject^ obj)
{
    NULL_CHECK(obj);
    return ref new JsArrayImpl(get_ref(obj).ObjOwnPropertyNames());
}

IJsArray^ JsObject::GetOwnPropertySymbols(IJsObject^ obj)
{
    NULL_CHECK(obj);
    return ref new JsArrayImpl(get_ref(obj).ObjOwnPropertySymbols());
}

IJsObject^ InnerGetOwnPropertyDescriptor(IJsObject^ obj, RawPropertyId propertyId)
{
    NULL_CHECK(obj);
    return safe_cast<IJsObject^>(JsValue::CreateTyped(get_ref(obj)[propertyId].Descriptor()));
}

IJsObject^ JsObject::GetOwnPropertyDescriptor(IJsObject^ obj, string^ propertyId)
{
    return InnerGetOwnPropertyDescriptor(obj, propertyId);
}

IJsObject^ JsObject::GetOwnPropertyDescriptor(IJsObject^ obj, IJsSymbol^ propertyId)
{
    NULL_CHECK(propertyId);
    return InnerGetOwnPropertyDescriptor(obj, get_ref(propertyId));
}