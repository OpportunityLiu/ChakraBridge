#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"
#include "JsSymbol.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsObject : IJsValue, 
        Windows::Foundation::Collections::IMap<string^, IJsValue^>, Windows::Foundation::Collections::IMapView<string^, IJsValue^>, 
        Windows::Foundation::Collections::IMap<IJsSymbol^, IJsValue^>, Windows::Foundation::Collections::IMapView<IJsSymbol^, IJsValue^>
    {
    };

    ref class JsObjectImpl : JsValueImpl, IJsObject
    {
    internal:
        using IStrMap = Windows::Foundation::Collections::IMap<string^, IJsValue^>;
        using ISymMap = Windows::Foundation::Collections::IMap<IJsSymbol^, IJsValue^>;
        using IStrMapView = Windows::Foundation::Collections::IMapView<string^, IJsValue^>;
        using ISymMapView = Windows::Foundation::Collections::IMapView<IJsSymbol^, IJsValue^>;
        using IStrKVP = Windows::Foundation::Collections::IKeyValuePair<string^, IJsValue^>;
        using ISymKVP = Windows::Foundation::Collections::IKeyValuePair<IJsSymbol^, IJsValue^>;
        using IStrIterator = Windows::Foundation::Collections::IIterator<IStrKVP^>;
        using ISymIterator = Windows::Foundation::Collections::IIterator<ISymKVP^>;
        using IStrIterable = Windows::Foundation::Collections::IIterable<IStrKVP^>;
        using ISymIterable = Windows::Foundation::Collections::IIterable<ISymKVP^>;

        JsObjectImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual ~JsObjectImpl();
        virtual Platform::String^ ToString() override;

        virtual void Clear() {};

        virtual IJsValue^ Lookup(string^ key);
        virtual void Remove(string^ key) {};
        virtual bool HasKey(string^ key) { return false; };
        virtual bool Insert(string^ key, IJsValue^ value) { return false; };
        virtual IStrMapView^ GetStrView() = IStrMap::GetView { return this; };
        virtual void Split(IStrMapView^* first, IStrMapView^* second) {};
        virtual property uint32 StrSize { uint32 get() = IStrMap::Size::get, IStrMapView::Size::get { return 0; } };
        virtual IStrIterator^ StrFirst() = IStrIterable::First { return nullptr; }

        virtual IJsValue^ Lookup(IJsSymbol^ key);
        virtual void Remove(IJsSymbol^ key) {};
        virtual bool HasKey(IJsSymbol^ key) { return false; };
        virtual bool Insert(IJsSymbol^ key, IJsValue^ value) { return false; };
        virtual ISymMapView^ GetSymView() = ISymMap::GetView { return this; };
        virtual void Split(ISymMapView^* first, ISymMapView^* second) {};
        virtual property uint32 SymSize { uint32 get() = ISymMap::Size::get, ISymMapView::Size::get { return 0; } };
        virtual ISymIterator^ SymFirst() = ISymIterable::First { return nullptr; }
    };

    /// <summary>
    /// Static methods of <see cref="IJsObject"/>.
    /// </summary>
    public ref class JsObject sealed
    {
    private:
        JsObject() {}

    };
}
