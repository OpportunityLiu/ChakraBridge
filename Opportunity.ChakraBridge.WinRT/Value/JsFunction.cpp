#include "pch.h"
#include "JsFunction.h"
#include "JsContext\JsContext.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace Opportunity::ChakraBridge::WinRT;

size_t FuncId = 0;

_Ret_maybenull_ JsValueRef CALLBACK JsNativeFunctionImpl(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void* callbackState)
{
    auto& ftable = JsContext::Current->Runtime->FunctionTable;
    auto funciter = ftable.find(reinterpret_cast<size_t>(callbackState));
    if (funciter == ftable.end())
    {
        JsValueRef error;
        CHAKRA_CALL(JsCreateError(RawPointerToString(L"Specific native function is not found."), &error));
        CHAKRA_CALL(JsSetException(error));
        return JS_INVALID_REFERENCE;
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
        JsValueRef error;
        CHAKRA_CALL(JsCreateError(RawPointerToString(ex->Message), &error));
        CHAKRA_CALL(JsSetException(error));
        return JS_INVALID_REFERENCE;
    }
    catch (...)
    {
        JsValueRef error;
        CHAKRA_CALL(JsCreateError(RawPointerToString(L"Unknown exception in native function."), &error));
        CHAKRA_CALL(JsSetException(error));
        return JS_INVALID_REFERENCE;
    }
}

void getArgs(IJsValue^ caller, JsFunctionImpl::IJsValueVectorView^ arguments, std::vector<JsValueRef>& args)
{
    if (caller == nullptr || to_impl(caller)->Reference == JS_INVALID_REFERENCE)
        args.push_back(RawGlobalObject());
    else
        args.push_back(to_impl(caller)->Reference);

    if (arguments == nullptr || arguments->Size == 0)
        return;

    if (arguments->Size > std::numeric_limits<unsigned short>::max() - 1u)
        throw ref new Platform::InvalidArgumentException(L"Too many arguments");

    JsValueRef undef = JS_INVALID_REFERENCE;

    for each (auto var in arguments)
    {
        if (var == nullptr || to_impl(var)->Reference == JS_INVALID_REFERENCE)
        {
            if (undef == JS_INVALID_REFERENCE)
                undef = RawUndefined();
            args.push_back(undef);
        }
        else
            args.push_back(to_impl(var)->Reference);
    }
}

IJsValue^ JsFunctionImpl::Invoke(IJsValue^ caller, IJsValueVectorView^ arguments)
{
    std::vector<JsValueRef> args;
    getArgs(caller, arguments, args);
    JsValueRef result;
    CHAKRA_CALL(JsCallFunction(Reference, &args[0], static_cast<unsigned int>(args.size()), &result));
    return JsValue::CreateTyped(result);
}

IJsObject^ JsFunctionImpl::New(IJsValueVectorView^ arguments)
{
    std::vector<JsValueRef> args;
    getArgs(nullptr, arguments, args);
    JsValueRef result;
    CHAKRA_CALL(JsConstructObject(Reference, &args[0], static_cast<unsigned int>(args.size()), &result));
    return safe_cast<IJsObject^>(JsValue::CreateTyped(result));
}

string^ JsFunctionImpl::Name::get()
{
    return RawStringToPointer(RawGetProperty(Reference, L"name"));
}

int32 JsFunctionImpl::Length::get()
{
    return RawNumberToInt(RawGetProperty(Reference, L"length"));
}

IJsObject^ JsFunctionImpl::Prototype::get()
{
    return dynamic_cast<IJsObject^>(JsValue::CreateTyped(RawGetProperty(Reference, L"prototype")));
}

void JsFunctionImpl::Prototype::set(IJsObject^ value)
{
    if (value == nullptr)
    {
        RawSetProperty(Reference, L"prototype", RawUndefined());
        return;
    }
    RawSetProperty(Reference, L"prototype", to_impl(value)->Reference);
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
    auto id = FuncId++;
    JsContext::Current->Runtime->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateFunction(JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, IJsString^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    JsValueRef ref;
    auto id = FuncId++;
    JsContext::Current->Runtime->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateNamedFunction(to_impl(name)->Reference, JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}

IJsFunction^ JsFunction::Create(JsFunctionDelegate^ function, string^ name)
{
    if (name == nullptr)
        return Create(function);
    NULL_CHECK(function);
    JsValueRef ref, nameref;
    CHAKRA_CALL(JsPointerToString(name->Data(), name->Length(), &nameref));
    auto id = FuncId++;
    JsContext::Current->Runtime->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateNamedFunction(nameref, JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}
