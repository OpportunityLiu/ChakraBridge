#pragma once
#include <unordered_map>
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    ref class JsContext;
    ref class JsRuntime;

    public interface struct IJsRuntime
    {
        /// <summary>
        /// Raises before gabage collection.
        /// </summary>
        event Windows::Foundation::TypedEventHandler<JsRuntime^, Platform::Object^>^ CollectingGarbage;
    };

    public ref class JsRuntime sealed : [Windows::Foundation::Metadata::Default] IJsRuntime
    {
    private:
        static void CALLBACK JsBeforeCollectCallbackImpl(_In_opt_ void *callbackState);
    internal:
        JsRuntimeHandle Handle;
        JsRuntime(JsRuntimeHandle handle);
        std::unordered_map<JsContextRef, JsContext^> Contexts;
        static std::unordered_map<JsRuntimeHandle, JsRuntime^> RuntimeDictionary;
    public:
        /// <summary>
        /// Disposes a runtime.
        /// </summary>
        virtual ~JsRuntime();

        /// <summary>
        /// Raises before gabage collection.
        /// </summary>
        virtual event Windows::Foundation::TypedEventHandler<JsRuntime^, Platform::Object^>^ CollectingGarbage;

        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <param name="attributes">The attributes of the runtime to be created.</param>
        /// <returns>The runtime created.</returns>
        [Windows::Foundation::Metadata::Overload("CreateWithAttributes")]
        static JsRuntime^ Create(Opportunity::ChakraBridge::WinRT::JsRuntimeAttributes attributes);
        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <returns>The runtime created.</returns>
        [Windows::Foundation::Metadata::Overload("Create")]
        [Windows::Foundation::Metadata::DefaultOverload]
        static JsRuntime^ Create() { return Create(Opportunity::ChakraBridge::WinRT::JsRuntimeAttributes::None); }
    };

    /// <summary>
    /// Event handler of <see cref="JsRuntime::CollectingGarbage"/>.
    /// </summary>
    public delegate void JsCollectingGarbageEventHandler(JsRuntime^ sender, Platform::Object^ args);
}
