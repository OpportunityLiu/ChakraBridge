#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsError : IJsObject
    {

        /// <summary>
        /// <c>message</c> property of Error.
        /// </summary>
        property Platform::String^ Message;

        /// <summary>
        /// <c>name</c> property of Error.
        /// </summary>
        property Platform::String^ Name;
    };

    ref class JsErrorImpl sealed : JsObjectImpl, IJsError
    {
    internal:
        JsErrorImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMapView, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMapView, IJsSymbol^);
        INHERIT_INTERFACE_METHOD(Clear, void, IStrMap);
        INHERIT_INTERFACE_METHOD(Clear, void, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMapView, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMapView, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, IStrMap, string^, IJsValue^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, ISymMap, IJsSymbol^, IJsValue^);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetStrView, IStrMapView^, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetSymView, ISymMapView^, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM2(Split, void, IStrMapView, IStrMapView^*, IStrMapView^*);
        INHERIT_INTERFACE_METHOD_PARAM2(Split, void, ISymMapView, ISymMapView^*, ISymMapView^*);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMapView);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMapView);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, StrFirst, IStrIterator^, IStrIterable);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, SymFirst, ISymIterator^, ISymIterable);

    public:
        virtual Platform::String^ ToString() override;
        virtual property Platform::String^ Message;
        virtual property Platform::String^ Name;
    };

    /// <summary>
    /// Static methods of <see cref="IJsError"/>.
    /// </summary>
    public ref class JsError sealed
    {
    private:
        JsError() {}
    };
}
