#pragma once
#include "JsObject.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript array value.
    /// </summary>
    public interface class IJsArray : IJsObject, Windows::Foundation::Collections::IVector<IJsValue^>
    {
    };

    ref class JsArrayImpl sealed : JsObjectImpl, IJsArray
    {
    internal:
        JsArrayImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
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
        using T = IJsValue^;
        using TVector = Windows::Foundation::Collections::IVector<T>;
        using TVectorView = Windows::Foundation::Collections::IVectorView<T>;
        using TIterable = Windows::Foundation::Collections::IIterable<T>;
        using TIterator = Windows::Foundation::Collections::IIterator<T>;
        virtual property uint32 ArraySize { uint32 get() = TVector::Size::get; }
        virtual void Append(T value);
        virtual void ArrayClear() = TVector::Clear;
        virtual T GetAt(uint32 index);
        virtual uint32 GetMany(uint32 startIndex, Platform::WriteOnlyArray<T>^ items);
        virtual TVectorView^ ArrayGetView() = TVector::GetView;
        virtual bool IndexOf(T value, uint32* index);
        virtual void InsertAt(uint32 index, T value);
        virtual void RemoveAt(uint32 index);
        virtual void RemoveAtEnd();
        virtual void ReplaceAll(const Platform::Array<T>^ items);
        virtual void SetAt(uint32 index, T value);
        virtual TIterator^ ArrayFirst() = TVector::First;
    };

    /// <summary>
    /// Static methods of <see cref="IJsArray"/>.
    /// </summary>
    public ref class JsArray sealed
    {
    private:
        JsArray() {}
    public:

        /// <summary>
        /// Creates a JavaScript array object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="length">The initial length of the array.</param>
        /// <returns>A JavaScript array object.</returns>
        static IJsArray^ Create(uint32 length);
    };
}
