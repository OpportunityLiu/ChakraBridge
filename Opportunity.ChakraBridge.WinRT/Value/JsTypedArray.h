#pragma once
#define NOMINMAX
#include "JsObject.h"
#include "JsEnum.h"
#include <unordered_map>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A Javascript typed array.
    /// </summary>
    public interface class IJsTypedArray : IJsObject
    {
        /// <summary>
        /// A <see cref="Windows::Storage::Streams::IBuffer"/> to access data of this <see cref="IJsTypedArray"/>.
        /// </summary>
        DECL_R_PROPERTY(IBuffer^, Data);
        /// <summary>
        /// Gets the type of array.
        /// </summary>
        DECL_R_PROPERTY(JsArrayType, ArrayType);
        /// <summary>
        /// Represents the <see cref="IJsArrayBuffer"/> referenced by the <see cref="IJsTypedArray"/> at construction time.
        /// </summary>
        DECL_R_PROPERTY(IJsArrayBuffer^, Buffer);
        /// <summary>
        /// Represents the size in bytes of each element in the <see cref="IJsTypedArray"/>.
        /// </summary>
        DECL_R_PROPERTY(uint32, BytesPerElement);
        /// <summary>
        /// Represents the length of the <see cref="IJsTypedArray"/> in bytes.
        /// </summary>
        DECL_R_PROPERTY(uint32, ByteLength);
        /// <summary>
        /// Represents the offset (in bytes) of this <see cref="IJsTypedArray"/> from the start of its <see cref="Buffer"/>.
        /// </summary>
        DECL_R_PROPERTY(uint32, ByteOffset);
    };

    ref class JsTypedArrayImpl abstract : JsObjectImpl, [Default] IJsTypedArray
    {
    internal:
        uint8 * const BufferPtr;
        const uint32 BufferLen;
        const JsArrayType ArrType;
        const uint32 ElementSize;

        JsTypedArrayImpl(RawValue ref, uint8*const bufferPtr, const uint32 bufferLen, const JsArrayType arrType, const uint32 elementSize);

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

        [[noreturn]] static void ThrowForFixedSize();
        property uint32 ArraySize { uint32 get(); }
        void BoundCheck(uint32 index);
    public:
        virtual DECL_R_PROPERTY(IBuffer^, Data);
        virtual DECL_R_PROPERTY(IJsArrayBuffer^, Buffer);
        virtual DECL_R_PROPERTY(JsArrayType, ArrayType);
        virtual DECL_R_PROPERTY(uint32, BytesPerElement);
        virtual DECL_R_PROPERTY(uint32, ByteLength);
        virtual DECL_R_PROPERTY(uint32, ByteOffset);
    };

    /// <summary>
    /// Static methods of <see cref="IJsTypedArray"/>.
    /// </summary>
    public ref class JsTypedArray sealed
    {
    private:
        JsTypedArray() {}

    internal:
        static JsTypedArrayImpl^ CreateTyped(RawValue ref);

    public:
        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [Overload("CreateEmpty")]
        static IJsTypedArray^ Create(JsArrayType arrayType);

        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [DefaultOverload]
        [Overload("CreateWithLength")]
        static IJsTypedArray^ Create(JsArrayType arrayType, uint32 length);

        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <param name="arrayLike">An array-like or iterable object to convert to an array.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [Overload("CreateWithArrayLike")]
        static IJsTypedArray^ Create(JsArrayType arrayType, IJsValue^ arrayLike);

        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [Overload("CreateWithArrayBuffer")]
        static IJsTypedArray^ Create(JsArrayType arrayType, IJsArrayBuffer^ buffer);

        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <param name="byteOffset">The offset of the created array to <paramref name="buffer"/>.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [Overload("CreateWithArrayBufferAndOffset")]
        static IJsTypedArray^ Create(JsArrayType arrayType, IJsArrayBuffer^ buffer, uint32 byteOffset);

        /// <summary>
        /// Create new instance of <see cref="IJsTypedArray"/>.
        /// </summary>
        /// <param name="arrayType">Type of array.</param>
        /// <param name="buffer">The <see cref="JsArrayBuffer"/> that the array will be based on.</param>
        /// <param name="byteOffset">The offset of the created array to <paramref name="buffer"/>.</param>
        /// <param name="length">Length of array.</param>
        /// <returns>A new instance of <see cref="IJsTypedArray"/>.</returns>
        [Overload("CreateWithArrayBufferAndLength")]
        static IJsTypedArray^ Create(JsArrayType arrayType, IJsArrayBuffer^ buffer, uint32 byteOffset, uint32 length);

    };

#pragma push_macro("interface")
#undef interface

    template<JsArrayType EEle>
    struct JsTypedArrayTempInfo {};

#define __TYPED_ARRAY_INTERFACE_DECL(name, actualType, rtType) \
    public interface class IJs##name##Array :IJsTypedArray, vector<rtType>{}; \
    template<>struct JsTypedArrayTempInfo<JsArrayType::name>{using t_ele = actualType;using t_winrt = rtType; using t_int = IJs##name##Array;}

    /// <summary>A Javascript Int8Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Int8, int8, int16);
    /// <summary>A Javascript Uint8Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Uint8, uint8, uint8);
    /// <summary>A Javascript Uint8ClampedArray.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Uint8Clamped, uint8, uint8);
    /// <summary>A Javascript Int16Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Int16, int16, int16);
    /// <summary>A Javascript Uint16Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Uint16, uint16, uint16);
    /// <summary>A Javascript Int32Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Int32, int32, int32);
    /// <summary>A Javascript Uint32Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Uint32, uint32, uint32);
    /// <summary>A Javascript Float32Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Float32, float32, float32);
    /// <summary>A Javascript Float64Array.</summary>
    __TYPED_ARRAY_INTERFACE_DECL(Float64, float64, float64);

#undef __TYPED_ARRAY_INTERFACE_DECL
#pragma pop_macro("interface")

    template<JsArrayType EEle,
        typename TAct = typename JsTypedArrayTempInfo<EEle>::t_ele,
        typename TRt = typename JsTypedArrayTempInfo<EEle>::t_winrt,
        typename TInterface = typename JsTypedArrayTempInfo<EEle>::t_int>
    ref class JsTypedArrayTempImpl sealed : JsTypedArrayImpl, [Default] TInterface
    {
    internal:
        JsTypedArrayTempImpl(RawValue ref, uint8*const bufferPtr, const uint32 bufferLen, const JsArrayType arrType, const uint32 elementSize)
            :JsTypedArrayImpl(std::move(ref), bufferPtr, bufferLen, arrType, elementSize)
        {
            static_assert(std::numeric_limits<TRt>::max() >= std::numeric_limits<TAct>::max());
            static_assert(std::numeric_limits<TRt>::min() <= std::numeric_limits<TAct>::min());
            static_assert(sizeof(TRt) >= sizeof(TAct));
            if (arrType != EEle)
                Throw(E_INVALIDARG, L"Wrong array type.");
            if (elementSize != sizeof(TAct))
                Throw(E_INVALIDARG, L"Wrong element size.");
        }

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

        INHERIT_INTERFACE_R_PROPERTY(Data, IBuffer^, IJsTypedArray);
        INHERIT_INTERFACE_R_PROPERTY(Buffer, IJsArrayBuffer^, IJsTypedArray);
        INHERIT_INTERFACE_R_PROPERTY(ArrayType, JsArrayType, IJsTypedArray);
        INHERIT_INTERFACE_R_PROPERTY(BytesPerElement, uint32, IJsTypedArray);
        INHERIT_INTERFACE_R_PROPERTY(ByteLength, uint32, IJsTypedArray);
        INHERIT_INTERFACE_R_PROPERTY(ByteOffset, uint32, IJsTypedArray);

        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, ArraySize, uint32, vector<TRt>);

        static TAct InElementCheck(TRt value)
        {
            if constexpr(std::numeric_limits<TRt>::max() != std::numeric_limits<TAct>::max()
                || std::numeric_limits<TRt>::min() != std::numeric_limits<TAct>::min())
            {
                if (value > std::numeric_limits<TAct>::max() || value < std::numeric_limits<TAct>::min())
                {
                    std::wstringstream stream;
                    stream << L"value is too large of too small for the element in this tyepd array, a value in range of "
                        << std::numeric_limits<TAct>::min() << " ~ " << std::numeric_limits<TAct>::max() << " is expected.";
                    auto str = stream.str();
                    Throw(E_INVALIDARG, string_ref(str.c_str(), str.length()));
                }
            }
            return static_cast<TAct>(value);
        }

    public:
        virtual void Append(TRt value) { ThrowForFixedSize(); }
        virtual void ArrayClear() = vector<TRt>::Clear{ ThrowForFixedSize(); }
        virtual void InsertAt(uint32 index, TRt value) { ThrowForFixedSize(); }
        virtual void RemoveAt(uint32 index) { ThrowForFixedSize(); }
        virtual void RemoveAtEnd() { ThrowForFixedSize(); }
        virtual void ReplaceAll(const array<TRt>^ items) { ThrowForFixedSize(); }

        virtual TRt GetAt(uint32 index)
        {
            BoundCheck(index);
            return reinterpret_cast<TAct*>(BufferPtr)[index];
        }

        virtual uint32 GetMany(uint32 startIndex, write_only_array<TRt>^ items)
        {
            NULL_CHECK(items);
            BoundCheck(startIndex);
            auto end = static_cast<uint32>(std::min(static_cast<uint64>(startIndex) + items->Length, static_cast<uint64>(ArraySize)));
            std::copy_n(reinterpret_cast<TAct*>(BufferPtr) + startIndex, end - startIndex, items->begin());
            return end - startIndex;
        }

        virtual bool IndexOf(TRt value, uint32* index)
        {
            const auto actualValue = InElementCheck(value);
            const auto first = reinterpret_cast<TAct*>(BufferPtr);
            const auto last = first + ArraySize;
            auto p = std::find(first, last, actualValue);
            if (p == last)
                return false;
            *index = static_cast<uint32>(p - first);
            return true;
        }

        virtual void SetAt(uint32 index, TRt value)
        {
            BoundCheck(index);
            reinterpret_cast<TAct*>(BufferPtr)[index] = InElementCheck(value);
        }

        virtual vector_view<TRt>^ ArrayGetView() = vector<TRt>::GetView
        {
            auto vec = std::vector<TRt>(reinterpret_cast<TAct*>(BufferPtr), reinterpret_cast<TAct*>(BufferPtr) + ArraySize);
            return ref new Platform::Collections::VectorView<TRt>(std::move(vec));
        }

        virtual iterator<TRt>^ ArrayFirst() = vector<TRt>::First{ return ArrayGetView()->First(); }
    };
}
