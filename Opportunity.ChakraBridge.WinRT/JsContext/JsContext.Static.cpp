#include "pch.h"
#include "JsContext.h"
#include "Value\JsError.h"

using namespace Opportunity::ChakraBridge::WinRT;

/// <summary>
/// Get the instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JS_INVALID_REFERENCE"/>. 
/// </summary>
/// <param name="reference">The reference.</param>
/// <returns>The instance of the <see cref="JsContext"/></returns>
JsContext^ JsContext::Get(JsContextRef reference)
{
    if (reference == JS_INVALID_REFERENCE)
        return nullptr;
    JsRuntimeHandle rth;
    CHAKRA_CALL(JsGetRuntime(reference, &rth));
    auto rt = JsRuntime::RuntimeDictionary[rth];
    return rt->Contexts[reference];
}

void JsContext::StartDebugging()
{
    CHAKRA_CALL(JsStartDebugging());
}

uint32 JsContext::Idle()
{
    unsigned int ticks;
    CHAKRA_CALL(JsIdle(&ticks));
    return static_cast<uint32>(ticks);
}

void JsContext::SetException(IJsError^ exception)
{
    NULL_CHECK(exception);
    CHAKRA_CALL(JsSetException(to_impl(exception)->Reference));
}

JsValueRef JsContext::LastJsError = JS_INVALID_REFERENCE;

void JsContext::GetAndClearExceptionCore()
{
    if (JsGetAndClearException(&LastJsError) != ::JsNoError)
        LastJsError = JS_INVALID_REFERENCE;
}

IJsError^ JsContext::GetAndClearException()
{
    GetAndClearExceptionCore();
    return LastError;
}

IJsError^ JsContext::LastError::get()
{
    if (LastJsError == JS_INVALID_REFERENCE)
        return nullptr;
    try
    {
        return safe_cast<IJsError^>(JsValue::CreateTyped(LastJsError));
    }
    catch (...)
    {
        return nullptr;
    }
}

void JsContext::ProjectWinRTNamespace(string ^ namespaceName)
{
    NULL_CHECK(namespaceName);
    CHAKRA_CALL(JsProjectWinRTNamespace(namespaceName->Data()));
}

JsContext^ JsContext::Current::get()
{
    JsContextRef c;
    CHAKRA_CALL(JsGetCurrentContext(&c));
    return Get(c);
}

void JsContext::Current::set(JsContext^ value)
{
    LastJsError = JS_INVALID_REFERENCE;
    if (value == nullptr || value->Reference == JS_INVALID_REFERENCE)
    {
        CHAKRA_CALL(JsSetCurrentContext(JS_INVALID_REFERENCE));
    }
    else
    {
        CHAKRA_CALL(JsSetCurrentContext(value->Reference));
        CHAKRA_CALL(JsSetPromiseContinuationCallback(JsPromiseContinuationCallbackImpl, value->Reference));
        return;
    }
}

bool JsContext::HasException::get()
{
    bool hasException;
    CHAKRA_CALL(JsHasException(&hasException));
    return hasException;
}