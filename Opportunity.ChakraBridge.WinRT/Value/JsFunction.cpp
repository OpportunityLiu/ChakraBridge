#include "pch.h"
#include "JsFunction.h"
#include "JsContext\JsContext.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace Opportunity::ChakraBridge::WinRT;

std::unordered_map<RawValue, JsFunctionDelegate^> JsFunctionImpl::FunctionTable;

RawValue JsFunctionImpl::JsNativeFunctionImpl(
    const RawValue callee, const bool isConstructCall, const RawValue*const arguments, const unsigned short argumentCount, void* callbackState)
{
    auto& ftable = JsFunctionImpl::FunctionTable;
    auto funciter = ftable.find(callee);
    if (funciter == ftable.end())
    {
        const auto error = RawValue::CreateError(RawValue(L"Specific native function is not found."));
        RawContext::SetException(error);
        return RawValue::Invalid();
    }
    try
    {
        auto func = funciter->second;
        auto caller = JsValue::CreateTyped(arguments[0]);
        auto callObj = dynamic_cast<IJsObject^>(caller);
        if (callObj == nullptr)
        {
            if (dynamic_cast<IJsNull^>(caller) != nullptr || dynamic_cast<IJsUndefined^>(caller) != nullptr)
                callObj = JsValue::GlobalObject;
            else
                callObj = JsValue::ToJsObject(caller);
        }
        auto args = std::vector<IJsValue^>(argumentCount - 1);
        for (short i = 0; i < argumentCount - 1; i++)
        {
            args[i] = JsValue::CreateTyped(arguments[i + 1]);
        }
        auto result = func(ref new JsFunctionImpl(callee), callObj, isConstructCall, ref new Platform::Collections::VectorView<IJsValue^>(std::move(args)));
        if (result == nullptr)
            return JS_INVALID_REFERENCE;
        return to_impl(result)->Reference;
    }
    catch (Platform::Exception^ ex)
    {
        const auto error = RawValue::CreateError(RawValue(ex->Message));
        RawContext::SetException(error);
        return RawValue::Invalid();
    }
    catch (...)
    {
        const auto error = RawValue::CreateError(RawValue(L"Unknown exception in native function."));
        RawContext::SetException(error);
        return RawValue::Invalid();
    }
}

void getArgs(IJsValue^ caller, vector_view<IJsValue>^ arguments, std::vector<RawValue>& args)
{
    if (caller == nullptr)
        args.push_back(RawValue::GlobalObject());
    else
        args.push_back(to_impl(caller)->Reference);

    if (arguments == nullptr || arguments->Size == 0)
        return;

    if (arguments->Size > std::numeric_limits<unsigned short>::max() - 1u)
        Throw(E_INVALIDARG, L"Too many arguments");

    RawValue undef;

    for each (auto var in arguments)
    {
        if (var == nullptr)
            goto SET_UNDEF;
        {
            const auto ref = to_impl(var)->Reference;
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

void JsFunctionImpl::JsFunctionBeforeCollectCallbackImpl(const RawValue ref, void *const callbackState)
{
    __try
    {
        JsObjectImpl::JsObjectBeforeCollectCallbackImpl(ref, callbackState);
    }
    __finally
    {
        FunctionTable.erase(ref);
    }
}

JsFunctionImpl::JsOBCC^ JsFunctionImpl::ObjectCollectingCallback::get()
{
    return JsObjectImpl::ObjectCollectingCallback;
}

void JsFunctionImpl::ObjectCollectingCallback::set(JsOBCC^ value)
{
    if (FunctionTable.find(Reference) == FunctionTable.end())
    {
        JsObjectImpl::ObjectCollectingCallback = value;
        return;
    }
    CHAKRA_CALL(JsSetObjectBeforeCollectCallback(Reference.Ref, nullptr, reinterpret_cast<::JsObjectBeforeCollectCallback>(JsFunctionBeforeCollectCallbackImpl)));
    if (value == nullptr)
        OBCCMap.erase(Reference);
    else
        OBCCMap[Reference] = value;
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
    if (value == nullptr)
    {
        Reference[L"prototype"] = RawValue::Undefined();
        return;
    }
    Reference[L"prototype"] = to_impl(value)->Reference;
}

string^ JsFunctionImpl::ToString()
{
    // for parsed scripts
    if (Length < 0)
    {
        return L"function() { [parsed script code] }";
    }
    return JsObjectImpl::ToString();
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function)
{
    NULL_CHECK(function);
    JsValueRef ref;
    CHAKRA_CALL(JsCreateFunction(reinterpret_cast<::JsNativeFunction>(JsFunctionImpl::JsNativeFunctionImpl), nullptr, &ref));
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(function);
    return func;
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, IJsString^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    JsValueRef ref;
    CHAKRA_CALL(JsCreateNamedFunction(to_impl(name)->Reference.Ref, reinterpret_cast<::JsNativeFunction>(JsFunctionImpl::JsNativeFunctionImpl), nullptr, &ref));
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(function);
    return func;
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, string^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    JsValueRef ref;
    CHAKRA_CALL(JsCreateNamedFunction(RawValue(name).Ref, reinterpret_cast<::JsNativeFunction>(JsFunctionImpl::JsNativeFunctionImpl), nullptr, &ref));
    auto func = ref new JsFunctionImpl(ref);
    func->InitForNativeFunc(function);
    return func;
}

void JsFunctionImpl::InitForNativeFunc(JsFunctionDelegate^ function)
{
    FunctionTable[Reference] = function;
    this->ObjectCollectingCallback = nullptr;
}
