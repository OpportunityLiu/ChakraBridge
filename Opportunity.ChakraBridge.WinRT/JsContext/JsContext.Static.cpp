#include "pch.h"
#include "JsContext.h"
#include "Value\JsError.h"

using namespace Opportunity::ChakraBridge::WinRT;

/// <summary>
/// Get the instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JS_INVALID_REFERENCE"/>. 
/// </summary>
/// <param name="reference">The reference.</param>
/// <returns>The instance of the <see cref="JsContext"/></returns>
JsContext^ JsContext::Get(const RawContext& reference)
{
    if (!reference.IsValid())
        return nullptr;
    const auto rth = reference.Runtime();
    auto rt = JsRuntime::RuntimeDictionary[rth];
    return rt->Contexts[reference];
}

void JsContext::StartDebugging()
{
    RawContext::StartDebugging();
}

uint32 JsContext::Idle()
{
    return static_cast<uint32>(RawContext::Idle());
}

void JsContext::SetException(IJsError^ exception)
{
    NULL_CHECK(exception);
    RawContext::SetException(get_ref(exception));
}

RawValue JsContext::LastJsError = nullptr;

void JsContext::GetAndClearExceptionCore()
{
    if (JsGetAndClearException(&LastJsError.Ref) != ::JsNoError)
        LastJsError = nullptr;
}

IJsError^ JsContext::GetAndClearException()
{
    GetAndClearExceptionCore();
    return LastError;
}

IJsError^ JsContext::LastError::get()
{
    return dynamic_cast<IJsError^>(JsValue::CreateTyped(LastJsError));
}

void JsContext::ProjectWinRTNamespace(string^ namespaceName)
{
    NULL_CHECK(namespaceName);
    RawContext::ProjectWinRTNamespace(namespaceName->Data());
}

JsContext^ JsContext::Current::get()
{
    return Get(RawContext::Current());
}

void JsContext::Current::set(JsContext^ value)
{
    LastJsError = nullptr;
    const RawContext v = value == nullptr ? nullptr : value->Reference;
    RawContext::Current(v);
    if (v.IsValid())
    {
        RawContext::SetPromiseContinuationCallback<RawContext, JsPromiseContinuationCallbackImpl>(v);
    }
}

bool JsContext::HasException::get()
{
    return RawContext::HasException();
}