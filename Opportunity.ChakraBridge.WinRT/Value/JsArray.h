#pragma once
#include "JsObject.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript array value.
    /// </summary>
    public interface class IJsArray : IJsObject, vector<IJsValue>
    {
    };

    ref class JsArrayImpl sealed : JsObjectImpl, [Default] IJsArray
    {
    internal:
        JsArrayImpl(RawValue ref) :JsObjectImpl(std::move(ref)) {}
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
        using T = IJsValue;
        virtual property uint32 ArraySize { uint32 get() = vector<T>::Size::get; }
        virtual void Append(T^ value);
        virtual void ArrayClear() = vector<T>::Clear;
        virtual T^ GetAt(uint32 index);
        virtual uint32 GetMany(uint32 startIndex, write_only_array<T>^ items);
        virtual vector_view<T>^ ArrayGetView() = vector<T>::GetView;
        virtual bool IndexOf(T^ value, uint32* index);
        virtual void InsertAt(uint32 index, T^ value);
        virtual void RemoveAt(uint32 index);
        virtual void RemoveAtEnd();
        virtual void ReplaceAll(const array<T>^ items);
        virtual void SetAt(uint32 index, T^ value);
        virtual iterator<T>^ ArrayFirst() = vector<T>::First;
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
        /// <param name="length">The initial length of the array.</param>
        /// <returns>A JavaScript array object.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("CreateWithLength")]
        static IJsArray^ Create(uint32 length);

        /// <summary>
        /// Creates a JavaScript array object from items.
        /// </summary>
        /// <param name="items">The initial data of the array.</param>
        /// <returns>A JavaScript array object.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateWithItems")]
        static IJsArray^ Create(vector_view<IJsValue>^ items);

        /// <summary>
        /// Creates a JavaScript array object from an array-like or iterable object.
        /// </summary>
        /// <param name="arrayLike">An array-like or iterable object to convert to an array.</param>
        /// <returns>A JavaScript array object.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateWithArrayLike")]
        static IJsArray^ Create(IJsValue^ arrayLike);
    };
}
