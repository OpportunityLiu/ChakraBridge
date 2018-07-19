#include "pch.h"
#include "JsFunction.h"
#include "JsContext\JsContext.h"
#include <limits>

using namespace Opportunity::ChakraBridge::WinRT;

size_t FuncId = 0;

_Ret_maybenull_ JsValueRef CALLBACK JsNativeFunctionImpl(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void* callbackState)
{
    auto funciter = JsContext::Current->FunctionTable.find(reinterpret_cast<size_t>(callbackState));

    //TODO:
    return JS_INVALID_REFERENCE;
}

void getArgs(IJsValue^ caller, JsFunctionImpl::IJsValueVectorView^ arguments, std::vector<JsValueRef>& args)
{
    if (caller == nullptr || to_impl(caller)->Reference == JS_INVALID_REFERENCE)
    {
        JsValueRef globalobj;
        CHAKRA_CALL(JsGetGlobalObject(&globalobj));
        args.push_back(globalobj);
    }
    else
        args.push_back(to_impl(caller)->Reference);

    if (arguments == nullptr || arguments->Size == 0)
        return;

    if (arguments->Size > std::numeric_limits<unsigned short>::max() - 1u)
        throw ref new Platform::InvalidArgumentException("Too many arguments");

    JsValueRef undef = JS_INVALID_REFERENCE;

    for each (auto var in arguments)
    {
        if (var == nullptr || to_impl(var)->Reference == JS_INVALID_REFERENCE)
        {
            if (undef == JS_INVALID_REFERENCE)
                CHAKRA_CALL(JsGetUndefinedValue(&undef));
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

IJsFunction^ JsFunction::Of(JsFunctionDelegate^ function)
{
    if (function == nullptr)
        throw ref new Platform::InvalidArgumentException("function is null.");
    JsValueRef ref;
    auto id = FuncId++;
    JsContext::Current->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateFunction(JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}

IJsFunction^ JsFunction::Of(JsFunctionDelegate^ function, IJsString^ name)
{
    if (name == nullptr)
        return Of(function);
    if (function == nullptr)
        throw ref new Platform::InvalidArgumentException("function is null.");
    JsValueRef ref;
    auto id = FuncId++;
    JsContext::Current->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateNamedFunction(to_impl(name)->Reference, JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}

IJsFunction^ JsFunction::Of(JsFunctionDelegate^ function, string^ name)
{
    if (name == nullptr)
        return Of(function);
    if (function == nullptr)
        throw ref new Platform::InvalidArgumentException("function is null.");
    JsValueRef ref, nameref;
    CHAKRA_CALL(JsPointerToString(name->Data(), name->Length(), &nameref));
    auto id = FuncId++;
    JsContext::Current->FunctionTable[id] = function;
    CHAKRA_CALL(JsCreateNamedFunction(nameref, JsNativeFunctionImpl, reinterpret_cast<void*>(id), &ref));
    return ref new JsFunctionImpl(ref);
}
