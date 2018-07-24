#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include <unordered_map>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A Javascript ArrayBuffer.
    /// </summary>
    public interface class IJsArrayBuffer : IJsObject
    {
        using IBuffer = Windows::Storage::Streams::IBuffer;
        /// <summary>
        /// A <see cref="Windows::Storage::Streams::IBuffer"/> to access data of this <see cref="IJsArrayBuffer"/>.
        /// </summary>
        DECL_R_PROPERTY(IBuffer^, Data);
        /// <summary>
        /// Represents the length of the <see cref="IJsArrayBuffer"/> in bytes.
        /// </summary>
        DECL_R_PROPERTY(uint32, ByteLength);
    };

    ref class JsArrayBufferImpl sealed : JsObjectImpl, [Default] IJsArrayBuffer
    {
    internal:
        // map from reinterpret_cast<void*>(buffer) to reference
        static std::unordered_map<void*, RawValue> ExternalBufferKeyMap;
        // map from reference to IBuffer^
        static std::unordered_map<RawValue, IJsArrayBuffer::IBuffer^> ExternalBufferDataMap;
        static void CALLBACK JsFinalizeCallbackImpl(_In_opt_ void *data);

        uint8* BufferPtr;
        unsigned int BufferLen;

        JsArrayBufferImpl(RawValue ref);

        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToString, string^, Windows::Foundation::IStringable);

        INHERIT_INTERFACE_RW_PROPERTY(Proto, IJsObject^, IJsObject);
        INHERIT_INTERFACE_METHOD(PreventExtension, void, IJsObject);
        INHERIT_INTERFACE_R_PROPERTY(IsExtensionAllowed, bool, IJsObject);
        INHERIT_INTERFACE_RW_PROPERTY(ObjectCollectingCallback, JsOBCC^, IJsObject);

        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_EXPLICT(Clear, StrClear, void, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(Clear, SymClear, void, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, IStrMap, string^, IJsValue^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, ISymMap, IJsSymbol^, IJsValue^);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetStrView, IStrMapView^, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetSymView, ISymMapView^, ISymMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, StrFirst, IStrIterator^, IStrIterable);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, SymFirst, ISymIterator^, ISymIterable);

    public:
        virtual DECL_R_PROPERTY(IJsArrayBuffer::IBuffer^, Data);
        virtual DECL_R_PROPERTY(uint32, ByteLength);
    };

    /// <summary>
    /// Static methods of <see cref="IJsArrayBuffer"/>.
    /// </summary>
    public ref class JsArrayBuffer sealed
    {
    private:
        JsArrayBuffer() {}
    public:
        /// <summary>
        /// Create a new instance of <see cref="IJsArrayBuffer"/>.
        /// </summary>
        /// <param name="length">Length of buffer in bytes.</param>
        /// <returns>A new instance of <see cref="IJsArrayBuffer"/>.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("Create")]
        static IJsArrayBuffer^ Create(uint32 length);

        /// <summary>
        /// Create a new instance of <see cref="IJsArrayBuffer"/>.
        /// </summary>
        /// <param name="buffer">A buffer that will be used as backing data storage.</param>
        /// <returns>A new instance of <see cref="IJsArrayBuffer"/>.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateWithBuffer")]
        static IJsArrayBuffer^ Create(IJsArrayBuffer::IBuffer^ buffer);
    };
}
