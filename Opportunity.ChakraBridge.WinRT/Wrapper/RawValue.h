#pragma once
#include "PreDeclear.h"
#include "RawRef.h"
#include "RawPropertyId.h"
#include <cstring>
#include <string>

namespace Opportunity::ChakraBridge::WinRT
{
    template<typename T>
    using RawNativeFunction = RawValue(const RawValue& callee, const RawValue& caller, const bool isConstructCall, const RawValue*const arguments, const unsigned short argumentCount, const T& callbackState);

    template<typename T>
    using RawFinalizeCallback = void(const T& data);

    template<typename T>
    using RawObjectBeforeCollectCallback = void(const RawValue& ref, const T& callbackState);

    struct[[nodiscard]] RawValue sealed :public RawRcRef<JsValueRef>
    {
#pragma region Static Objects

        static RawValue GlobalObject()
        {
            RawValue g;
            CHAKRA_CALL(JsGetGlobalObject(&g.Ref));
            return g;
        }

        static RawValue Null()
        {
            RawValue g;
            CHAKRA_CALL(JsGetNullValue(&g.Ref));
            return g;
        }

        static RawValue Undefined()
        {
            RawValue g;
            CHAKRA_CALL(JsGetUndefinedValue(&g.Ref));
            return g;
        }

        static RawValue True()
        {
            RawValue g;
            CHAKRA_CALL(JsGetTrueValue(&g.Ref));
            return g;
        }

        static RawValue False()
        {
            RawValue g;
            CHAKRA_CALL(JsGetFalseValue(&g.Ref));
            return g;
        }

#pragma endregion

#pragma region Basic Construction

        constexpr RawValue() :RawRcRef(JS_INVALID_REFERENCE) {}
        constexpr RawValue(std::nullptr_t) : RawRcRef(JS_INVALID_REFERENCE) {}
        explicit constexpr RawValue(JsValueRef ref) : RawRcRef(std::move(ref)) {}

#pragma endregion

#pragma region Runtime Construction

        static RawValue CreateObject()
        {
            RawValue obj;
            CHAKRA_CALL(JsCreateObject(&obj.Ref));
            return obj;
        }

        template<typename T, RawFinalizeCallback<T> finalizeCallback>
        static void CALLBACK JsrtFinalizeCallback(_In_opt_ void *data)
        {
            finalizeCallback(DataFromJsrt<T>(data));
        }

        template<typename T, RawFinalizeCallback<T> finalizeCallback>
        static RawValue CreateExternalObject(const T& data)
        {
            RawValue obj;
            CHAKRA_CALL(JsCreateExternalObject(DataToJsrt(data), JsrtFinalizeCallback<T, finalizeCallback>, &obj.Ref));
            return obj;
        }

        template<typename T, RawNativeFunction<T> function>
        static JsValueRef CALLBACK RawNativeFunctionCall(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void *callbackState);

        template<typename T, RawNativeFunction<T> function>
        static RawValue CreateFunction(const T& callbackState)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateFunction(RawNativeFunctionCall<T, function>, DataToJsrt(callbackState), &r.Ref));
            return r;
        }

        template<typename T, RawNativeFunction<T> function>
        static RawValue CreateFunction(const RawValue& name, const T& callbackState)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateNamedFunction(name.Ref, RawNativeFunctionCall<T, function>, DataToJsrt(callbackState), &r.Ref));
            return r;
        }

        static RawValue CreateSymbol(const RawValue& description)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateSymbol(description.Ref, &r.Ref));
            return r;
        }

        static RawValue CreateArray(unsigned int length)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateArray(length, &r.Ref));
            return r;
        }

        static RawValue CreateArrayBuffer(unsigned int length)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateArrayBuffer(length, &r.Ref));
            return r;
        }

        static RawValue CreateArrayBuffer(void *data, unsigned int byteLength, JsFinalizeCallback finalizeCallback, void *callbackState)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateExternalArrayBuffer(data, byteLength, finalizeCallback, callbackState, &r.Ref));
            return r;
        }

        static RawValue CreateDataView(const RawValue& arrayBuffer, unsigned int byteOffset, unsigned int byteLength)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateDataView(arrayBuffer.Ref, byteOffset, byteLength, &r.Ref));
            return r;
        }

        static RawValue CreateTypedArray(const JsArrayType type, const RawValue& base, const uint32 byteOffset, const uint32 elementLength)
        {
            RawValue r;
            CHAKRA_CALL(JsCreateTypedArray(static_cast<::JsTypedArrayType>(type), base.Ref, byteOffset, elementLength, &r.Ref));
            return r;
        }

#define CREATE_ERROR(methodName, implName)                  \
static RawValue methodName(const RawValue& message)         \
{                                                           \
    RawValue r;                                             \
    CHAKRA_CALL(::Js##implName(message.Ref, &r.Ref));       \
    return r;                                               \
}

        CREATE_ERROR(CreateError, CreateError);
        CREATE_ERROR(CreateRangeError, CreateRangeError);
        CREATE_ERROR(CreateReferenceError, CreateReferenceError);
        CREATE_ERROR(CreateSyntaxError, CreateSyntaxError);
        CREATE_ERROR(CreateTypeError, CreateTypeError);
        CREATE_ERROR(CreateUriError, CreateURIError);

#undef CREATE_ERROR

        explicit RawValue(const bool v)
        {
            CHAKRA_CALL(JsBoolToBoolean(v, &Ref));
        }

        explicit RawValue(const int v)
        {
            CHAKRA_CALL(JsIntToNumber(v, &Ref));
        }

        explicit RawValue(const double v)
        {
            CHAKRA_CALL(JsDoubleToNumber(v, &Ref));
        }

        template<size_t Len>
        explicit RawValue(const wchar_t(&v)[Len])
        {
            CHAKRA_CALL(JsPointerToString(v, Len, &Ref));
        }

        explicit RawValue(const wchar_t*const v, const size_t len)
        {
            CHAKRA_CALL(JsPointerToString(v, len, &Ref));
        }

        explicit RawValue(const std::wstring& v)
        {
            CHAKRA_CALL(JsPointerToString(v.c_str(), v.length(), &Ref));
        }

        explicit RawValue(IInspectable*const v)
        {
            CHAKRA_CALL(JsInspectableToObject(v, &Ref));
        }

#pragma endregion

#pragma region Inner Convertion

        RawValue ToJsBoolean() const
        {
            RawValue r;
            CHAKRA_CALL(JsConvertValueToBoolean(Ref, &r.Ref));
            return r;
        }

        RawValue ToJsNumber() const
        {
            RawValue r;
            CHAKRA_CALL(JsConvertValueToNumber(Ref, &r.Ref));
            return r;
        }

        RawValue ToJsString() const
        {
            RawValue r;
            CHAKRA_CALL(JsConvertValueToString(Ref, &r.Ref));
            return r;
        }

        RawValue ToJsObjet() const
        {
            RawValue r;
            CHAKRA_CALL(JsConvertValueToObject(Ref, &r.Ref));
            return r;
        }

#pragma endregion

#pragma region To Native Convertion

        bool ToBoolean() const
        {
            bool v;
            CHAKRA_CALL(JsBooleanToBool(Ref, &v));
            return v;
        }

        int ToInt() const
        {
            int v;
            CHAKRA_CALL(JsNumberToInt(Ref, &v));
            return v;
        }

        double ToDouble() const
        {
            double v;
            CHAKRA_CALL(JsNumberToDouble(Ref, &v));
            return v;
        }

        string_ref ToString() const
        {
            const wchar_t* ptr;
            size_t len;
            CHAKRA_CALL(JsStringToPointer(Ref, &ptr, &len));
            return string_ref(ptr, len);
        }

        IInspectable* ToInspectable() const
        {
            IInspectable* v;
            CHAKRA_CALL(JsObjectToInspectable(Ref, &v));
            return v;
        }

#pragma endregion

#pragma region Value Properties

        JsType Type() const
        {
            JsType type;
            CHAKRA_CALL(JsGetValueType(Ref, reinterpret_cast<::JsValueType*>(&type)));
            return type;
        }

        RawContext Context() const;

#pragma endregion

#pragma region Type Specific Properties

        int StrLength() const
        {
            int l;
            CHAKRA_CALL(JsGetStringLength(Ref, &l));
            return l;
        }

        bool ObjHasExternalData() const
        {
            bool hasExternalData;
            CHAKRA_CALL(JsHasExternalData(Ref, &hasExternalData));
            return hasExternalData;
        }

        void ObjPreventExtension() const
        {
            CHAKRA_CALL(JsPreventExtension(Ref));
        }

        bool ObjIsExtensionAllowed() const
        {
            bool r;
            CHAKRA_CALL(JsGetExtensionAllowed(Ref, &r));
            return r;
        }

        RawValue ObjProto() const
        {
            RawValue r;
            CHAKRA_CALL(JsGetPrototype(Ref, &r.Ref));
            return r;
        }

        void ObjProto(const RawValue& value) const
        {
            CHAKRA_CALL(JsSetPrototype(Ref, value.Ref));
        }

        bool ObjInstanceOf(const RawValue& constructor) const
        {
            bool r;
            CHAKRA_CALL(JsInstanceOf(Ref, constructor.Ref, &r));
            return r;
        }

        RawValue ObjOwnPropertyNames() const
        {
            RawValue r;
            CHAKRA_CALL(JsGetOwnPropertyNames(Ref, &r.Ref));
            return r;
        }

        RawValue ObjOwnPropertySymbols() const
        {
            RawValue r;
            CHAKRA_CALL(JsGetOwnPropertySymbols(Ref, &r.Ref));
            return r;
        }

        template<typename T, RawObjectBeforeCollectCallback<T> callback>
        static void CALLBACK JsObjectBeforeCollectCallback(_In_ JsValueRef ref, _In_opt_ void *callbackState)
        {
            callback(RawValue(ref), DataFromJsrt<T>(callbackState));
        }

        template<typename T, RawObjectBeforeCollectCallback<T> callback>
        void ObjBeforeCollectCallback(const T& callbackState) const
        {
            CHAKRA_CALL(JsSetObjectBeforeCollectCallback(Ref, DataToJsrt(callbackState), JsObjectBeforeCollectCallback<T, callback>));
        }

        void ObjBeforeCollectCallback(nullptr_t) const
        {
            CHAKRA_CALL(JsSetObjectBeforeCollectCallback(Ref, nullptr, nullptr));
        }

        template<typename T>
        T ExtObjExternalData() const
        {
            T v;
            CHAKRA_CALL(JsGetExternalData(Ref, DataToJsrtPtr(v)));
            return v;
        }
        template<typename T>
        void ExtObjExternalData(const T&) const
        {
            T v;
            CHAKRA_CALL(JsSetExternalData(Ref, DataToJsrt(v)));
            return v;
        }

#pragma endregion

#pragma region Funtion Call

        RawValue New(const RawValue*callerAndArgs, unsigned int len) const
        {
            RawValue r;
            CHAKRA_CALL(JsConstructObject(Ref, reinterpret_cast<JsValueRef*>(const_cast<RawValue*>(callerAndArgs)), len, &r.Ref));
            return r;
        }

        [[maybe_unused]]RawValue Invoke(const RawValue*callerAndArgs, unsigned int len) const
        {
            RawValue r;
            CHAKRA_CALL(JsCallFunction(Ref, reinterpret_cast<JsValueRef*>(const_cast<RawValue*>(callerAndArgs)), len, &r.Ref));
            return r;
        }

        template<unsigned short len>
        RawValue New(const RawValue(&callerAndArgs)[len]) const
        {
            return New(callerAndArgs, len);
        }

        template<unsigned short len>
        [[maybe_unused]]RawValue Invoke(const RawValue(&callerAndArgs)[len]) const
        {
            return Invoke(callerAndArgs, len);
        }

        RawValue New() const
        {
            RawValue argsv;
            RawValue r;
            CHAKRA_CALL(JsConstructObject(Ref, reinterpret_cast<JsValueRef*>(&argsv), 1, &r.Ref));
            return r;
        }

        [[maybe_unused]]RawValue Invoke(const RawValue& caller) const
        {
            if (caller.IsValid())
            {
                RawValue r;
                CHAKRA_CALL(JsCallFunction(Ref, reinterpret_cast<JsValueRef*>(const_cast<RawValue*>(&caller)), 1, &r.Ref));
                return r;
            }
            return Invoke(RawValue::GlobalObject());
        }

        template<typename... TArgs>
        RawValue New(const RawValue& arg0, const TArgs&... args) const
        {
            const RawValue argsv[] = { RawValue::GlobalObject(), arg0, args... };
            return New(argsv);
        }

        template<typename... TArgs>
        [[maybe_unused]]RawValue Invoke(const RawValue& caller, const RawValue& arg0, const TArgs&... args) const
        {
            const RawValue argsv[] = { caller.IsValid() ? caller : RawValue::GlobalObject(), arg0, args... };
            return Invoke(argsv);
        }

#pragma endregion

#pragma region Object Property Operation

        struct PropertyStub;
        struct IndexedPropertyStub;

        struct[[nodiscard]] PropertyStub sealed
        {
            PropertyStub(const PropertyStub&) = delete;
            PropertyStub(PropertyStub&&) = delete;
            PropertyStub& operator =(const PropertyStub&) = delete;
            PropertyStub& operator =(PropertyStub&&) = delete;

            PropertyStub(const RawValue& parent, const RawPropertyId& propName)
                : Parent(parent.Ref), PropIdRef(propName.Ref) {}

            const JsValueRef Parent;
            const JsPropertyIdRef PropIdRef;

            RawValue operator()() const
            {
                return RawValue(*this);
            }
            operator RawValue() const
            {
                RawValue r;
                CHAKRA_CALL(JsGetProperty(Parent, PropIdRef, &r.Ref));
                return r;
            }
            PropertyStub& operator =(const RawValue & value)
            {
                CHAKRA_CALL(JsSetProperty(Parent, PropIdRef, value.Ref, true));
                return *this;
            }
            const PropertyStub& operator =(const RawValue & value) const
            {
                CHAKRA_CALL(JsSetProperty(Parent, PropIdRef, value.Ref, true));
                return *this;
            }
            PropertyStub operator[](const wchar_t*const key) const
            {
                return PropertyStub(RawValue(*this), key);
            }
            PropertyStub operator[](const RawPropertyId& key) const
            {
                return PropertyStub(RawValue(*this), key);
            }
            IndexedPropertyStub operator[](const RawValue& key) const
            {
                return IndexedPropertyStub(RawValue(*this), key);
            }
            [[nodiscard]] bool Exist() const
            {
                bool r;
                CHAKRA_CALL(JsHasProperty(Parent, PropIdRef, &r));
                return r;
            }
            bool Define(const RawValue& descriptor) const
            {
                bool r;
                CHAKRA_CALL(JsDefineProperty(Parent, PropIdRef, descriptor.Ref, &r));
                return r;
            }
            RawValue Descriptor()
            {
                RawValue r;
                CHAKRA_CALL(JsGetOwnPropertyDescriptor(Parent, PropIdRef, &r.Ref));
                return r;
            }
            [[maybe_unused]]RawValue Delete() const
            {
                RawValue r;
                CHAKRA_CALL(JsDeleteProperty(Parent, PropIdRef, true, &r.Ref));
                return r;
            }
        };

        struct[[nodiscard]] IndexedPropertyStub sealed
        {
            IndexedPropertyStub(const IndexedPropertyStub&) = delete;
            IndexedPropertyStub(IndexedPropertyStub&&) = delete;
            IndexedPropertyStub& operator =(const IndexedPropertyStub&) = delete;
            IndexedPropertyStub& operator =(IndexedPropertyStub&&) = delete;

            IndexedPropertyStub(const RawValue& parent, const RawValue& propName)
                : Parent(parent.Ref), PropIdRef(propName.Ref) {}
            const JsValueRef Parent;
            const JsValueRef PropIdRef;

            RawValue operator()() const
            {
                return static_cast<RawValue>(*this);
            }
            operator RawValue() const
            {
                RawValue r;
                CHAKRA_CALL(JsGetIndexedProperty(Parent, PropIdRef, &r.Ref));
                return r;
            }
            const IndexedPropertyStub& operator =(const RawValue& value) const
            {
                CHAKRA_CALL(JsSetIndexedProperty(Parent, PropIdRef, value.Ref));
                return *this;
            }
            IndexedPropertyStub& operator =(const RawValue& value)
            {
                CHAKRA_CALL(JsSetIndexedProperty(Parent, PropIdRef, value.Ref));
                return *this;
            }
            PropertyStub operator[](const wchar_t*const key) const
            {
                return PropertyStub(RawValue(*this), key);
            }
            PropertyStub operator[](const RawPropertyId& key) const
            {
                return PropertyStub(RawValue(*this), key);
            }
            IndexedPropertyStub operator[](const RawValue& key) const
            {
                return IndexedPropertyStub(RawValue(*this), key);
            }
            [[nodiscard]] bool Exist() const
            {
                bool r;
                CHAKRA_CALL(JsHasIndexedProperty(Parent, PropIdRef, &r));
                return r;
            }
            void Delete() const
            {
                CHAKRA_CALL(JsDeleteIndexedProperty(Parent, PropIdRef));
            }
        };

        PropertyStub operator[](const wchar_t*const key) const
        {
            return PropertyStub(*this, key);
        }
        PropertyStub operator[](const RawPropertyId& key) const
        {
            return PropertyStub(*this, key);
        }
        IndexedPropertyStub operator[](const RawValue& key) const
        {
            return IndexedPropertyStub(*this, key);
        }
#pragma endregion

    };

    static_assert(sizeof(RawValue) == sizeof(JsValueRef));
};

template<> struct ::std::hash<Opportunity::ChakraBridge::WinRT::RawValue>
{
    constexpr size_t operator()(const Opportunity::ChakraBridge::WinRT::RawValue& v) const
    {
        return reinterpret_cast<size_t>(v.Ref);
    }
};