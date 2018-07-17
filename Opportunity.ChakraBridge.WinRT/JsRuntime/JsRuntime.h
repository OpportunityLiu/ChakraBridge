#pragma once
#include <unordered_map>
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    ref class JsContext;
    ref class JsRuntime;

    /// <summary>
    /// Event args.
    /// </summary>
    public ref class JsMemoryEventArgs sealed
    {
    public:
        /// <summary>
        /// The type of type allocation event.
        /// </summary>
        property JsMemoryEventType EventType { JsMemoryEventType get() { return _EventType; } }

        /// <summary>
        /// The size of the allocation.
        /// </summary>
        property uint64 AllocationSize { uint64 get() { return _AllocationSize; } }

        ///// <summary>
        ///// For the Allocate event, <see langword="false"/> allows the runtime to continue with allocation. 
        ///// <see langword="true"/> indicates the allocation request is rejected. 
        ///// The value is ignored for other allocation events.
        ///// </summary>
        //property bool IsRejected;
    internal:
        JsMemoryEventArgs(::JsMemoryEventType eventType, size_t allocationSize)
            :_EventType(static_cast<JsMemoryEventType>(eventType)), _AllocationSize(static_cast<uint64>(allocationSize))
        {
            //IsRejected = false;
        }
    private:
        const JsMemoryEventType _EventType;
        const uint64 _AllocationSize;
    };

    public interface struct IJsRuntime
    {
        /// <summary>
        /// Raises before gabage collection.
        /// </summary>
        event Windows::Foundation::TypedEventHandler<JsRuntime^, Platform::Object^>^ CollectingGarbage;
        /// <summary>
        /// Raises when the charka engine allocating or freeing memories.
        /// </summary>
        event Windows::Foundation::TypedEventHandler<JsRuntime^, JsMemoryEventArgs^>^ AllocatingMemory;
    };

    public ref class JsRuntime sealed : [Windows::Foundation::Metadata::Default] IJsRuntime
    {
    private:
        static void CALLBACK JsBeforeCollectCallbackImpl(_In_opt_ void *callbackState);
        static bool CALLBACK JsRuntime::JsMemoryAllocationCallbackImpl(_In_opt_ void *callbackState, _In_::JsMemoryEventType allocationEvent, _In_ size_t allocationSize);
    internal:
        JsRuntimeHandle Handle;
        JsRuntime(JsRuntimeHandle handle);
        std::unordered_map<JsContextRef, JsContext^> Contexts;
        static std::unordered_map<JsRuntimeHandle, JsRuntime^> RuntimeDictionary;
    public:
        /// <summary>
        /// Performs a full garbage collection.
        /// </summary>
        void CollectGarbage() { CHAKRA_CALL(JsCollectGarbage(this->Handle)); }

        /// <summary>
        /// Creates a script context for running scripts.
        /// </summary>
        /// <remarks>
        /// Each script context has its own global object that is isolated from all other script 
        /// contexts.
        /// </remarks>
        /// <returns>The created script context.</returns>
        JsContext^ CreateContext();

        /// <summary>
        /// Gets created contexts.
        /// </summary>
        /// <returns>Collection of contexts.</returns>
        Windows::Foundation::Collections::IIterable<JsContext^>^ GetContexts();

        /// <summary>
        /// Disposes a runtime.
        /// </summary>
        virtual ~JsRuntime();

        /// <summary>
        /// Raises before gabage collection.
        /// </summary>
        virtual event Windows::Foundation::TypedEventHandler<JsRuntime^, Platform::Object^>^ CollectingGarbage;
        /// <summary>
        /// Raises when the charka engine allocating or freeing memories.
        /// </summary>
        virtual event Windows::Foundation::TypedEventHandler<JsRuntime^, JsMemoryEventArgs^>^ AllocatingMemory;

        /// <summary>
        /// Gets the current memory usage for a runtime.
        /// </summary>
        /// <remarks>
        /// Memory usage can be always be retrieved, regardless of whether or not the runtime is active
        /// on another thread.
        /// </remarks>
        property uint64 MemoryUsage
        {
            uint64 get();
        };

        /// <summary>
        /// Gets or sets the current memory limit for a runtime.
        /// </summary>
        /// <remarks>
        /// The memory limit of a runtime can be always be retrieved, regardless of whether or not the 
        /// runtime is active on another thread.
        /// </remarks>
        property uint64 MemoryLimit
        {
            uint64 get();
            void set(uint64 value);
        };

        /// <summary>
        /// Gets or sets a value indicating whether script execution is enabled in the runtime.
        /// </summary>
        property bool IsEnabled
        {
            bool get();
            void set(bool value);
        };

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
        /// <summary>
        /// Gets created runtimes.
        /// </summary>
        /// <returns>Collection of runtimes.</returns>
        static Windows::Foundation::Collections::IIterable<JsRuntime^>^ GetRuntimes();
    };
};
