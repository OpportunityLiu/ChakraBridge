#pragma once
#include "alias.h"
#include <jsrt.h>
#include <limits>

namespace Opportunity::ChakraBridge::WinRT
{
    inline void CHAKRA_CALL(JsErrorCode result)
    {
        string^ GetChakraError();
        using namespace Platform;
        if (result == JsErrorCode::JsNoError)
            return;
        switch (result)
        {
        case JsErrorCode::JsErrorInvalidArgument:
            throw ref new InvalidArgumentException(L"An argument to a hosting API was invalid.");
        case JsErrorCode::JsErrorNullArgument:
            throw ref new InvalidArgumentException(L"An argument to a hosting API was null in a context where null is not allowed.");
        case JsErrorCode::JsErrorNoCurrentContext:
            throw ref new FailureException(L"The hosting API requires that a context be current, but there is no current context.");
        case JsErrorCode::JsErrorInExceptionState:
            throw ref new FailureException(L"The engine is in an exception state and no APIs can be called until the exception is cleared.");
        case JsErrorCode::JsErrorNotImplemented:
            throw ref new NotImplementedException(L"A hosting API is not yet implemented.");
        case JsErrorCode::JsErrorWrongThread:
            throw ref new WrongThreadException(L"A hosting API was called on the wrong thread.");
        case JsErrorCode::JsErrorRuntimeInUse:
            throw ref new FailureException(L"A runtime that is still in use cannot be disposed.");
        case JsErrorCode::JsErrorBadSerializedScript:
            throw ref new InvalidArgumentException(L"A bad serialized script was used, or the serialized script was serialized by a different version of the Chakra engine.");
        case JsErrorCode::JsErrorInDisabledState:
            throw ref new FailureException(L"The runtime is in a disabled state.");
        case JsErrorCode::JsErrorCannotDisableExecution:
            throw ref new FailureException(L"Runtime does not support reliable script interruption.");
        case JsErrorCode::JsErrorAlreadyDebuggingContext:
            throw ref new FailureException(L"The context cannot be put into a debug state because it is already in a debug state.");
        case JsErrorCode::JsErrorHeapEnumInProgress:
            throw ref new FailureException(L"A heap enumeration is currently underway in the script context.");
        case JsErrorCode::JsErrorArgumentNotObject:
            throw ref new InvalidArgumentException(L"A hosting API that operates on object values was called with a non-object value.");
        case JsErrorCode::JsErrorInProfileCallback:
            throw ref new FailureException(L"A script context is in the middle of a profile callback.");
        case JsErrorCode::JsErrorInThreadServiceCallback:
            throw ref new FailureException(L"A thread service callback is currently underway.");
        case JsErrorCode::JsErrorCannotSerializeDebugScript:
            throw ref new FailureException(L"Scripts cannot be serialized in debug contexts.");
        case JsErrorCode::JsErrorAlreadyProfilingContext:
            throw ref new FailureException(L"The context cannot start profiling because it is already profiling.");
        case JsErrorCode::JsErrorIdleNotEnabled:
            throw ref new FailureException(L"Idle notification given when the host did not enable idle processing.");
        case JsErrorCode::JsErrorOutOfMemory:
            throw ref new OutOfMemoryException(L"The Chakra engine has run out of memory.");
        case JsErrorCode::JsErrorScriptException:
        {
            string^ error;
            try
            {
                error = GetChakraError();
            }
            catch (...)
            {
                throw ref new FailureException(L"A JavaScript exception occurred while running a script.");
            }
            throw ref new FailureException(error);
        }

        case JsErrorCode::JsErrorScriptCompile:
        {
            string^ error;
            try
            {
                error = GetChakraError();
            }
            catch (...)
            {
                throw ref new FailureException(L"JavaScript failed to compile.");
            }
            throw ref new FailureException(error);
        }

        case JsErrorCode::JsErrorScriptTerminated:
            throw ref new OperationCanceledException(L"A script was terminated due to a request to suspend a runtime.");
        case JsErrorCode::JsErrorScriptEvalDisabled:
            throw ref new FailureException(L"A script was terminated because it tried to use 'eval' or 'function' and eval was disabled.");
        case JsErrorCode::JsErrorFatal:
            throw ref new FailureException(L"A fatal error in the engine has occurred.");
        case JsErrorCode::JsCannotSetProjectionEnqueueCallback:
            throw ref new FailureException(L"The context did not accept the enqueue callback.");
        case JsErrorCode::JsErrorCannotStartProjection:
            throw ref new FailureException(L"Failed to start projection.");
        case JsErrorCode::JsErrorInObjectBeforeCollectCallback:
            throw ref new FailureException(L"The operation is not supported in an object before collect callback.");
        case JsErrorCode::JsErrorObjectNotInspectable:
            throw ref new FailureException(L"Object cannot be unwrapped to 'IInspectable' pointer.");
        case JsErrorCode::JsErrorPropertyNotSymbol:
            throw ref new FailureException(L"A hosting API that operates on symbol property ids but was called with a non-symbol property id.");
        case JsErrorCode::JsErrorPropertyNotString:
            throw ref new FailureException(L"A hosting API that operates on string property ids but was called with a non-string property id.");
        case JsErrorCode::JsErrorWrongRuntime:
            throw ref new FailureException(L"A hosting API was called with object created on different javascript runtime.");

        default:
            throw ref new FailureException(L"Unknown error.");
        }
    }

    inline JsValueRef RawGlobalObject()
    {
        JsValueRef g;
        CHAKRA_CALL(JsGetGlobalObject(&g));
        return g;
    }

    inline JsValueRef RawNull()
    {
        JsValueRef g;
        CHAKRA_CALL(JsGetNullValue(&g));
        return g;
    }

    inline JsValueRef RawUndefined()
    {
        JsValueRef g;
        CHAKRA_CALL(JsGetUndefinedValue(&g));
        return g;
    }

    inline JsPropertyIdRef RawGetPropertyId(const wchar_t* propname)
    {
        JsPropertyIdRef prop;
        CHAKRA_CALL(JsGetPropertyIdFromName(propname, &prop));
        return prop;
    }

    inline JsPropertyIdRef RawGetPropertyId(JsValueRef propsym)
    {
        JsPropertyIdRef prop;
        CHAKRA_CALL(JsGetPropertyIdFromSymbol(propsym, &prop));
        return prop;
    }

    template<typename TProp>
    inline JsValueRef RawGetProperty(JsValueRef ref, TProp prop)
    {
        return RawGetProperty(ref, RawGetPropertyId(prop));
    }

    inline JsValueRef RawGetProperty(JsValueRef ref, JsPropertyIdRef prop)
    {
        JsValueRef result;
        CHAKRA_CALL(::JsGetProperty(ref, prop, &result));
        return result;
    }

    template<typename arg0, typename... args>
    inline JsValueRef RawGetProperty(JsValueRef ref, arg0 propname, args... propnameRest)
    {
        JsValueRef result = RawGetProperty(ref, propname);
        return RawGetProperty(result, propnameRest...);
    }

    inline JsValueRef RawCallFunction(JsValueRef callee, JsValueRef caller)
    {
        if (caller == JS_INVALID_REFERENCE)
            caller = RawGlobalObject();
        JsValueRef r;
        CHAKRA_CALL(JsCallFunction(callee, &caller, 1, &r));
        return r;
    }
}