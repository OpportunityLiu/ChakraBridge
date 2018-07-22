#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "JsSymbol.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A callback called before collecting an object.
    /// </summary>
    /// <param name="obj">The object to be collected.</param>
    public delegate void JsObjectBeforeCollectCallback(IJsObject^ obj);

    public interface class IJsObject : IJsValue,
        Windows::Foundation::Collections::IMap<string^, IJsValue^>,
        Windows::Foundation::Collections::IMap<IJsSymbol^, IJsValue^>
    {
        /// <summary>
        /// Gets or sets the prototype of an object, use <see langword="null"/> instead of <see cref="IJsNull"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        property IJsObject^ Proto;

        /// <summary>
        /// Sets an object to not be extensible.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        void PreventExtension();

        /// <summary>
        /// Gets a value indicating whether an object is extensible or not.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        property bool IsExtensionAllowed { bool get(); };

        /// <summary>
        /// A callback function that is called by the runtime before garbage collection of the object. 
        /// </summary>
        property JsObjectBeforeCollectCallback^ ObjectCollectingCallback;
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

        virtual property IJsObject^ Proto { IJsObject^ get(); void set(IJsObject^ value); }
        virtual void PreventExtension();
        virtual property bool IsExtensionAllowed { bool get(); };
        virtual property JsObjectBeforeCollectCallback^ ObjectCollectingCallback { JsObjectBeforeCollectCallback^ get(); void set(JsObjectBeforeCollectCallback^ value); }

        virtual IJsValue^ Lookup(string^ key);
        virtual void Remove(string^ key);
        virtual void StrClear() = IStrMap::Clear;
        virtual bool HasKey(string^ key);
        virtual bool Insert(string^ key, IJsValue^ value);
        virtual IStrMapView^ GetStrView() = IStrMap::GetView;
        virtual property uint32 StrSize { uint32 get() = IStrMap::Size::get; };
        virtual IStrIterator^ StrFirst() = IStrIterable::First;

        virtual IJsValue^ Lookup(IJsSymbol^ key);
        virtual void Remove(IJsSymbol^ key);
        virtual void SymClear() = ISymMap::Clear;
        virtual bool HasKey(IJsSymbol^ key);
        virtual bool Insert(IJsSymbol^ key, IJsValue^ value);
        virtual ISymMapView^ GetSymView() = ISymMap::GetView;
        virtual property uint32 SymSize { uint32 get() = ISymMap::Size::get; };
        virtual ISymIterator^ SymFirst() = ISymIterable::First;
    };

    /// <summary>
    /// Static methods of <see cref="IJsObject"/>.
    /// </summary>
    public ref class JsObject sealed
    {
    private:
        JsObject() {}
    public:
        /// <summary>
        /// Creates a new <see cref="IJsObject"/>.
        /// </summary>
        /// <returns>A new <see cref="IJsObject"/>.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsObject^ Create();

        /// <summary>
        /// Performs JavaScript "instanceof" operator test. 
        /// </summary>
        /// <param name="constructor">The constructor function to test against. </param>
        /// <returns>Whether the "object instanceof constructor" is <see langword="true"/>. </returns>
        static bool InstanceOf(IJsObject^ obj, IJsFunction^ constructor);

        /// <summary>
        /// Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <param name="obj">The object to define the property on.</param>
        /// <param name="propertyId">The ID of the property.</param>
        /// <param name="descriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("DefinePropertyByName")]
        static bool DefineProperty(IJsObject^ obj, string^ propertyId, IJsObject^ descriptor);

        /// <summary>
        /// Defines a new object's own property from a property descriptor.
        /// </summary>
        /// <param name="obj">The object to define the property on.</param>
        /// <param name="propertyId">The ID of the property.</param>
        /// <param name="descriptor">The property descriptor.</param>
        /// <returns>Whether the property was defined.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("DefinePropertyBySymbol")]
        static bool DefineProperty(IJsObject^ obj, IJsSymbol^ propertyId, IJsObject^ descriptor);

        /// <summary>
        /// Gets the list of all symbol properties on the object. 
        /// </summary>
        /// <param name="obj">The object to get the list on.</param>
        /// <returns>An array of property symbols. </returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsArray^ GetOwnPropertySymbols(IJsObject^ obj);

        /// <summary>
        /// Gets the list of all named properties on the object.
        /// </summary>
        /// <param name="obj">The object to get the list on.</param>
        /// <returns>An array of property names.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsArray^ GetOwnPropertyNames(IJsObject^ obj);

        /// <summary>
        /// Gets a property descriptor for an object's own property.
        /// </summary>
        /// <param name="obj">The object to get the property on.</param>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("GetOwnPropertyDescriptorByName")]
        static IJsObject^ GetOwnPropertyDescriptor(IJsObject^ obj, string^ propertyId);

        /// <summary>
        /// Gets a property descriptor for an object's own property.
        /// </summary>
        /// <param name="obj">The object to get the property on.</param>
        /// <param name="propertyId">The ID of the property.</param>
        /// <returns>The property descriptor.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("GetOwnPropertyDescriptorBySymbol")]
        static IJsObject^ GetOwnPropertyDescriptor(IJsObject^ obj, IJsSymbol^ propertyId);
    };
}
