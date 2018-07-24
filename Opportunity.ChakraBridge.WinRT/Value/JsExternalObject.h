#pragma once
#include "JsObject.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    ///  A JavaScript object that stores some external data.
    /// </summary>
    public interface class IJsExternalObject : IJsObject
    {

        /// <summary>
        /// Gets or sets the data in an external object.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        DECL_RW_PROPERTY(object^, ExternalData);
    };

    ref class JsExternalObjectImpl sealed : JsObjectImpl, [Default] IJsExternalObject
    {
    internal:
        JsExternalObjectImpl(RawValue ref) :JsObjectImpl(std::move(ref)) {}
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

        static void CALLBACK JsFinalizeCallbackImpl(_In_opt_ void *data);

    public:
        virtual DECL_RW_PROPERTY(object^, ExternalData);
    };

    /// <summary>
    /// Static methods of <see cref="IJsExternalObject"/>.
    /// </summary>
    public ref class JsExternalObject sealed
    {
    private:
        JsExternalObject() {}
    public:

        /// <summary>
        /// Creates a new <see cref="IJsExternalObject"/> that stores some external data.
        /// </summary>
        /// <param name="data">External data that the object will represent. May be <see langword="null"/>.</param>
        /// <returns>A new <see cref="IJsExternalObject"/> that stores some external data.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsExternalObject^ Create(object^ data);
    };
}
