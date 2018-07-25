#pragma once
#include "JsNull.h"
#include "JsValue.h"
#include "JsEnum.h"
#include "JsSymbol.h"
#include <memory>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A callback called before collecting an object.
    /// </summary>
    /// <param name="obj">The object to be collected.</param>
    public delegate void JsObjectBeforeCollectCallback(IJsObject^ obj);

    /// <summary>
    /// A JavaScript object.
    /// </summary>
    public interface class IJsObject : IJsValue, map<string, IJsValue>, map<IJsSymbol, IJsValue>
    {
        /// <summary>
        /// Gets or sets the prototype of an object, use <see langword="null"/> instead of <see cref="IJsNull"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        DECL_RW_PROPERTY(IJsObject^, Proto);

        /// <summary>
        /// Sets an object to not be extensible.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        void PreventExtension();

        /// <summary>
        /// Gets a value indicating whether an object is extensible or not.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        DECL_R_PROPERTY(bool, IsExtensionAllowed);

        /// <summary>
        /// A callback function that is called by the runtime before garbage collection of the object. 
        /// </summary>
        DECL_RW_PROPERTY(JsObjectBeforeCollectCallback^, ObjectCollectingCallback);
    };

    ref class JsObjectImpl : JsValueImpl, [Default] IJsObject
    {
    protected private:
        ~JsObjectImpl();

    internal:
        using IStrMap = map<string, IJsValue>;
        using ISymMap = map<IJsSymbol, IJsValue>;
        using IStrMapView = map_view<string, IJsValue>;
        using ISymMapView = map_view<IJsSymbol, IJsValue>;
        using IStrKVP = kv_pair<string, IJsValue>;
        using ISymKVP = kv_pair<IJsSymbol, IJsValue>;
        using IStrIterator = iterator<IStrKVP>;
        using ISymIterator = iterator<ISymKVP>;
        using IStrIterable = iterable<IStrKVP>;
        using ISymIterable = iterable<ISymKVP>;

        using JsOBCC = ::Opportunity::ChakraBridge::WinRT::JsObjectBeforeCollectCallback;
        using IBCC = void(const RawValue&);
        using OWP = struct OW
        {
            JsOBCC^ BeforeCollectCallback;
            weak_ref Object;
            IBCC* InternalBeforeCollectCallback;
            OW(JsValueImpl^const thisObj, JsOBCC^const callback, IBCC*const callback2)
                :Object(thisObj),BeforeCollectCallback(callback), InternalBeforeCollectCallback(callback2){}

            bool InUse()
            {
                return BeforeCollectCallback != nullptr || InternalBeforeCollectCallback != nullptr;
            }
        }*;

        JsObjectImpl(RawValue ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

        static std::unordered_map<RawValue, std::unique_ptr<OW>> OBCCMap;
        static void JsObjectBeforeCollectCallbackImpl(const RawValue& ref, const OWP& callbackState);
        void RegisterInternalBeforeCollectCallback(IBCC*const callback);

    public:
        virtual Platform::String^ ToString() override;

        virtual DECL_RW_PROPERTY(IJsObject^, Proto);
        virtual void PreventExtension();
        virtual DECL_R_PROPERTY(bool, IsExtensionAllowed);
        virtual DECL_RW_PROPERTY(JsOBCC^, ObjectCollectingCallback);

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
        [DefaultOverload]
        [Overload("Create")]
        static IJsObject^ Create();

        /// <summary>
        /// Creates a new <see cref="IJsObject"/> with properties.
        /// </summary>
        /// <param name="properties">Properties to inintialize the new <see cref="IJsObject"/>.</param>
        /// <returns>A new <see cref="IJsObject"/> with properties.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateWithStringMap")]
        static IJsObject^ Create(map_view<string, IJsValue>^ properties);

        /// <summary>
        /// Creates a new <see cref="IJsObject"/> with properties.
        /// </summary>
        /// <param name="properties">Properties to inintialize the new <see cref="IJsObject"/>.</param>
        /// <returns>A new <see cref="IJsObject"/> with properties.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateWithJsValueMap")]
        static IJsObject^ Create(map_view<IJsValue, IJsValue>^ properties);

        /// <summary>
        /// Performs JavaScript "instanceof" operator test. 
        /// </summary>
        /// <param name="obj">The object to test against.</param>
        /// <param name="constructor">The constructor function to test against. </param>
        /// <returns>Whether the "obj instanceof constructor" is <see langword="true"/>. </returns>
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
        /// Gets the list of all named properties on the object.
        /// </summary>
        /// <param name="obj">The object to get the list on.</param>
        /// <returns>An array of property names.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsArray^ GetOwnPropertyNames(IJsObject^ obj);

        /// <summary>
        /// Gets the list of all symbol properties on the object. 
        /// </summary>
        /// <param name="obj">The object to get the list on.</param>
        /// <returns>An array of property symbols. </returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsArray^ GetOwnPropertySymbols(IJsObject^ obj);

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
