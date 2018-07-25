#include "pch.h"
#include "JsFunction.h"
#include "JsContext\JsContext.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<RawValue, std::unique_ptr<JsFunctionImpl::FW>> JsFunctionImpl::FunctionTable;

RawValue JsFunctionImpl::JsNativeFunctionImpl(const RawValue& callee, const RawValue& caller, const bool isConstructCall, const RawValue*const arguments, const unsigned short argumentCount, const FWP& nativeFunc)
{
    try
    {
        _ASSERTE(nativeFunc != nullptr);
        const auto func = nativeFunc->Function;
        _ASSERTE(func != nullptr);
        auto callerv = JsValue::CreateTyped(caller);
        auto callObj = dynamic_cast<IJsObject^>(callerv);
        if (callObj == nullptr)
        {
            const auto type = caller.Type();
            if (type == JsType::Null || type == JsType::Undefined)
                callObj = JsValue::GlobalObject;
            else
                callObj = JsValue::ToJsObject(callerv);
        }

        auto args = std::vector<IJsValue^>(argumentCount);
        for (unsigned short i = 0; i < argumentCount; i++)
        {
            args[i] = JsValue::CreateTyped(arguments[i]);
        }
        auto result = func(ref new JsFunctionImpl(callee), callObj, isConstructCall, ref new Platform::Collections::VectorView<IJsValue^>(std::move(args)));
        return get_ref(result);
    }
    catch (Platform::Exception^ ex)
    {
        const auto mes = ex->Message;
        const auto error = RawValue::CreateError(RawValue(mes->Data(), mes->Length()));
        RawContext::SetException(error);
        return nullptr;
    }
}

void getArgs(IJsValue^ caller, vector_view<IJsValue>^ arguments, std::vector<RawValue>& args)
{
    if (caller == nullptr)
        args.push_back(RawValue::GlobalObject());
    else
        args.push_back(get_ref(caller));

    if (arguments == nullptr || arguments->Size == 0)
        return;

    if (arguments->Size > std::numeric_limits<unsigned short>::max() - 1u)
        Throw(E_INVALIDARG, L"Too many arguments");

    RawValue undef = nullptr;

    for (const auto var : arguments)
    {
        if (var == nullptr)
            goto SET_UNDEF;
        {
            const auto ref = get_ref(var);
            if (!ref.IsValid())
                goto SET_UNDEF;

            args.push_back(ref);
            continue;
        }

    SET_UNDEF:
        {
            if (!undef.IsValid())
                undef = RawValue::Undefined();
            args.push_back(undef);
            continue;
        }
    }
}

IJsValue^ JsFunctionImpl::Invoke(IJsValue^ caller, vector_view<IJsValue>^ arguments)
{
    std::vector<RawValue> args;
    getArgs(caller, arguments, args);
    const auto r = Reference.Invoke(&args[0], static_cast<unsigned int>(args.size()));
    return JsValue::CreateTyped(r);
}

IJsObject^ JsFunctionImpl::New(vector_view<IJsValue>^ arguments)
{
    std::vector<RawValue> args;
    getArgs(nullptr, arguments, args);
    const auto r = Reference.New(&args[0], static_cast<unsigned int>(args.size()));
    return safe_cast<IJsObject^>(JsValue::CreateTyped(r));
}

string^ JsFunctionImpl::Name::get()
{
    return Reference[L"name"]().ToString();
}

int32 JsFunctionImpl::Length::get()
{
    return Reference[L"length"]().ToInt();
}

IJsObject^ JsFunctionImpl::Prototype::get()
{
    return dynamic_cast<IJsObject^>(JsValue::CreateTyped(Reference[L"prototype"]));
}

void JsFunctionImpl::Prototype::set(IJsObject^ value)
{
    Reference[L"prototype"] = get_ref_or_undefined(value);
}

string^ JsFunctionImpl::ToString()
{
    if (Length >= 0)
        return JsObjectImpl::ToString();
    // for parsed scripts
    return L"function() { [parsed script code] }";
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function)
{
    NULL_CHECK(function);
    auto ptr = std::make_unique<JsFunctionImpl::FW>(function);
    const auto ref = RawValue::CreateFunction<JsFunctionImpl::FWP, JsFunctionImpl::JsNativeFunctionImpl>(ptr.get());
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(std::move(ptr));
    return func;
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, IJsString^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    auto ptr = std::make_unique<JsFunctionImpl::FW>(function);
    const auto ref = RawValue::CreateFunction<JsFunctionImpl::FWP, JsFunctionImpl::JsNativeFunctionImpl>(get_ref(name), ptr.get());
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(std::move(ptr));
    return func;
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, string^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    auto ptr = std::make_unique<JsFunctionImpl::FW>(function);
    const auto ref = RawValue::CreateFunction<JsFunctionImpl::FWP, JsFunctionImpl::JsNativeFunctionImpl>(RawValue(name->Data(), name->Length()), ptr.get());
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(std::move(ptr));
    return func;
}

void JsFunctionImpl::CollectNativeFunction(const RawValue& ref)
{
    FunctionTable.erase(ref);
}

void JsFunctionImpl::InitForNativeFunc(std::unique_ptr<FW> function)
{
    FunctionTable[Reference] = std::move(function);
    this->RegisterInternalBeforeCollectCallback(CollectNativeFunction);
}
