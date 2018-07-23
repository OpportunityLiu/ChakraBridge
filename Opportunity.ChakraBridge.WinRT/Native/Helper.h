#pragma once
#include "alias.h"
#include <jsrt.h>
#include <limits>
#include <vector>

namespace Opportunity::ChakraBridge::WinRT
{
    string^ CHAKRA_LAST_ERROR();

    inline void CHAKRA_CALL(const JsErrorCode result)
    {
        void GetChakraError();
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
                GetChakraError();
                error = CHAKRA_LAST_ERROR();
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
                GetChakraError();
                error = CHAKRA_LAST_ERROR();
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

    inline JsValueRef RawPointerToString(string^ pointer)
    {
        JsValueRef r;
        CHAKRA_CALL(JsPointerToString(pointer->Data(), static_cast<size_t>(pointer->Length()), &r));
        return r;
    }

    template<size_t LEN>
    inline JsValueRef RawPointerToString(const wchar_t(&pointer)[LEN])
    {
        JsValueRef r;
        CHAKRA_CALL(JsPointerToString(pointer, pointer[LEN - 1] == L'\0' ? LEN - 1 : LEN, &r));
        return r;
    }

    inline string^ RawStringToPointer(const JsValueRef strRef)
    {
        const wchar_t* str;
        size_t len;
        CHAKRA_CALL(JsStringToPointer(strRef, &str, &len));
        return ref new string(str, static_cast<unsigned int>(len));
    }

    inline int RawNumberToInt(const JsValueRef numRef)
    {
        int v;
        CHAKRA_CALL(JsNumberToInt(numRef, &v));
        return v;
    }

    inline double RawNumberToDouble(const JsValueRef numRef)
    {
        double v;
        CHAKRA_CALL(JsNumberToDouble(numRef, &v));
        return v;
    }

    inline JsValueRef RawIntToNumber(const int num)
    {
        JsValueRef v;
        CHAKRA_CALL(JsIntToNumber(num, &v));
        return v;
    }

    inline JsValueRef RawDoubleToNumber(const double num)
    {
        JsValueRef v;
        CHAKRA_CALL(JsDoubleToNumber(num, &v));
        return v;
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

    inline JsType RawGetValueType(const JsValueRef value)
    {
        ::JsValueType type;
        CHAKRA_CALL(JsGetValueType(value, &type));
        return static_cast<JsType>(type);
    }

    inline JsPropertyIdRef RawGetPropertyId(const wchar_t*const propname)
    {
        JsPropertyIdRef prop;
        CHAKRA_CALL(JsGetPropertyIdFromName(propname, &prop));
        return prop;
    }

    inline JsPropertyIdRef RawGetPropertyId(const JsValueRef propsym)
    {
        JsPropertyIdRef prop;
        CHAKRA_CALL(JsGetPropertyIdFromSymbol(propsym, &prop));
        return prop;
    }
    template<typename T>
    inline void RawSetProperty(const JsValueRef ref, T* propname, const JsValueRef value)
    {
        static_assert(false, "T* is not allowed");
    }

    inline void RawSetProperty(const JsValueRef ref, const JsValueRef propname, const JsValueRef value)
    {
        CHAKRA_CALL(::JsSetIndexedProperty(ref, propname, value));
    }

    inline void RawSetProperty(const JsValueRef ref, const wchar_t*const propname, const JsValueRef value)
    {
        CHAKRA_CALL(::JsSetProperty(ref, RawGetPropertyId(propname), value, true));
    }

    template<typename T>
    inline JsValueRef RawGetProperty(const JsValueRef ref, T* propname)
    {
        static_assert(false, "T* is not allowed");
    }

    inline JsValueRef RawGetProperty(const JsValueRef ref, const JsValueRef propname)
    {
        JsValueRef result;
        CHAKRA_CALL(::JsGetIndexedProperty(ref, propname, &result));
        return result;
    }

    inline JsValueRef RawGetProperty(const JsValueRef ref, const wchar_t*const propname)
    {
        JsValueRef result;
        CHAKRA_CALL(::JsGetProperty(ref, RawGetPropertyId(propname), &result));
        return result;
    }

    //inline JsValueRef RawGetProperty(JsValueRef ref, JsPropertyIdRef prop)
    //{
    //    JsValueRef result;
    //    CHAKRA_CALL(::JsGetProperty(ref, prop, &result));
    //    return result;
    //}

    template<typename arg0, typename arg1, typename... args>
    inline JsValueRef RawGetProperty(const JsValueRef ref, const arg0 propname1, const arg1 propname2, const args... propnameRest)
    {
        const JsValueRef result = RawGetProperty(ref, propname1);
        return RawGetProperty(result, propname2, propnameRest...);
    }

    template<typename... TArgs>
    inline JsValueRef RawCallFunction(const JsValueRef callee, const JsValueRef caller, const TArgs... args)
    {
        JsValueRef r;
        JsValueRef argsv[] = { caller == JS_INVALID_REFERENCE ? RawGlobalObject() : caller, args... };
        CHAKRA_CALL(JsCallFunction(callee, argsv, sizeof(argsv) / sizeof(JsValueRef), &r));
        return r;
    }
}