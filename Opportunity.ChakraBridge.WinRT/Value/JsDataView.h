#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include <unordered_map>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A Javascript DataView.
    /// </summary>
    public interface class IJsDataView : IJsObject
    {
        using IBuffer = Windows::Storage::Streams::IBuffer;
        /// <summary>
        /// A <see cref="Windows::Storage::Streams::IBuffer"/> to access data of this <see cref="IJsDataView"/>.
        /// </summary>
        DECL_R_PROPERTY(IBuffer^, Data);
        /// <summary>
        /// Represents the <see cref="IJsArrayBuffer"/> referenced by the <see cref="IJsDataView"/> at construction time.
        /// </summary>
        DECL_R_PROPERTY(IJsArrayBuffer^, Buffer);
        /// <summary>
        /// Represents the length of the <see cref="IJsDataView"/> in bytes.
        /// </summary>
        DECL_R_PROPERTY(uint32, ByteLength);
        /// <summary>
        /// Represents the offset (in bytes) of this <see cref="IJsDataView"/> from the start of its <see cref="Buffer"/>.
        /// </summary>
        DECL_R_PROPERTY(uint32, ByteOffset);
    };

    ref class JsDataViewImpl sealed : JsObjectImpl, IJsDataView
    {
    internal:
        uint8 * BufferPtr;
        unsigned int BufferLen;

        JsDataViewImpl(JsValueRef ref);

        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToString, string^, Windows::Foundation::IStringable);

        INHERIT_INTERFACE_RW_PROPERTY(Proto, IJsObject^, IJsObject);
        INHERIT_INTERFACE_METHOD(PreventExtension, void, IJsObject);
        INHERIT_INTERFACE_R_PROPERTY(IsExtensionAllowed, bool, IJsObject);
        INHERIT_INTERFACE_RW_PROPERTY(ObjectCollectingCallback, JsObjectBeforeCollectCallback^, IJsObject);

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
        virtual DECL_R_PROPERTY(IJsDataView::IBuffer^, Data);
        virtual DECL_R_PROPERTY(IJsArrayBuffer^, Buffer);
        virtual DECL_R_PROPERTY(uint32, ByteLength);
        virtual DECL_R_PROPERTY(uint32, ByteOffset);
    };

    /// <summary>
    /// Static methods of <see cref="IJsDataView"/>.
    /// </summary>
    public ref class JsDataView sealed
    {
    private:
        JsDataView() {}
    public:

        /// <summary>
        /// Create new instance of <see cref="IJsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="IJsArrayBuffer"/> that the <see cref="IJsDataView"/> will be based on.</param>
        [Overload("Create")]
        [DefaultOverload]
        static IJsDataView^ Create(IJsArrayBuffer^ buffer);

        /// <summary>
        /// Create new instance of <see cref="IJsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="IJsArrayBuffer"/> that the <see cref="IJsDataView"/> will be based on.</param>
        /// <param name="byteOffset">The offset (in bytes) of the <see cref="IJsDataView"/> from the start of <paramref name="buffer"/>.</param>
        [Overload("CreateWithOffset")]
        static IJsDataView^ Create(IJsArrayBuffer^ buffer, uint32 byteOffset);

        /// <summary>
        /// Create new instance of <see cref="IJsDataView"/>.
        /// </summary>
        /// <param name="buffer">The <see cref="IJsArrayBuffer"/> that the <see cref="IJsDataView"/> will be based on.</param>
        /// <param name="byteOffset">The offset (in bytes) of the <see cref="IJsDataView"/> from the start of <paramref name="buffer"/>.</param>
        /// <param name="byteLength">The length of the <see cref="IJsDataView"/> in bytes</param>
        [Overload("CreateWithOffsetAndLength")]
        static IJsDataView^ Create(IJsArrayBuffer^ buffer, uint32 byteOffset, uint32 byteLength);
    };
}
