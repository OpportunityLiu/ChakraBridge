#pragma once
#include <unordered_map>
#include "JsEnum.h"
#include "Value\JsFunction.h"
#include "alias.h"

namespace Opportunity::ChakraBridge::WinRT
{
    ref class JsContext;
    ref class JsRuntime;

    public interface struct IJsAllocatingMemoryEventArgs
    {
        /// <summary>
        /// The type of type allocation event.
        /// </summary>
        DECL_R_PROPERTY(JsMemoryEventType, EventType);

        /// <summary>
        /// The size of the allocation.
        /// </summary>
        DECL_R_PROPERTY(uint64, AllocationSize);
    };

    /// <summary>
    /// Event args.
    /// </summary>
    ref struct JsMemoryEventArgs sealed : IJsAllocatingMemoryEventArgs
    {
    public:
        virtual property JsMemoryEventType EventType { JsMemoryEventType get() { return _EventType; } }
        virtual property uint64 AllocationSize { uint64 get() { return _AllocationSize; } }

    internal:
        ///// <summary>
        ///// For the Allocate event, <see langword="false"/> allows the runtime to continue with allocation. 
        ///// <see langword="true"/> indicates the allocation request is rejected. 
        ///// The value is ignored for other allocation events.
        ///// </summary>
        //property bool IsRejected;
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
        event typed_event_handler<JsRuntime, object>^ CollectingGarbage;
        event typed_event_handler<JsRuntime, IJsAllocatingMemoryEventArgs>^ AllocatingMemory;
    };

    /// <summary>
    /// A Chakra runtime.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Each Chakra runtime has its own independent execution engine, JIT compiler, and garbage 
    /// collected heap. As such, each runtime is completely isolated from other runtimes.
    /// </para>
    /// <para>
    /// Runtimes can be used on any thread, but only one thread can call into a runtime at any 
    /// time.
    /// </para>
    /// <para>
    /// NOTE: A JsRuntime, unlike other objects in the Chakra hosting API, is not 
    /// garbage collected since it contains the garbage collected heap itself. A runtime will 
    /// continue to exist until Dispose is called.
    /// </para>
    /// </remarks>
    public ref class JsRuntime sealed : [Default] IJsRuntime
    {
    private:
        static void CALLBACK JsBeforeCollectCallbackImpl(_In_opt_ void *callbackState);
        static bool CALLBACK JsRuntime::JsMemoryAllocationCallbackImpl(_In_opt_ void *callbackState, _In_::JsMemoryEventType allocationEvent, _In_ size_t allocationSize);

    internal:
        const RawRuntime Handle;
        JsRuntime(RawRuntime handle);
        std::unordered_map<RawContext, JsContext^> Contexts;
        static std::unordered_map<RawRuntime, JsRuntime^> RuntimeDictionary;

        static bool CALLBACK JsThreadServiceCallbackImpl(_In_ JsBackgroundWorkItemCallback callback, _In_opt_ void *callbackState);

    public:
        /// <summary>
        /// Performs a full garbage collection.
        /// </summary>
        void CollectGarbage();

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
        virtual event typed_event_handler<JsRuntime, object>^ CollectingGarbage;
        /// <summary>
        /// Raises when the charka engine allocating or freeing memories.
        /// </summary>
        virtual event typed_event_handler<JsRuntime, IJsAllocatingMemoryEventArgs>^ AllocatingMemory;

        /// <summary>
        /// Gets the current memory usage for a runtime.
        /// </summary>
        /// <remarks>
        /// Memory usage can be always be retrieved, regardless of whether or not the runtime is active
        /// on another thread.
        /// </remarks>
        DECL_R_PROPERTY(uint64, MemoryUsage);

        /// <summary>
        /// Gets or sets the current memory limit for a runtime.
        /// </summary>
        /// <remarks>
        /// The memory limit of a runtime can be always be retrieved, regardless of whether or not the 
        /// runtime is active on another thread.
        /// </remarks>
        DECL_RW_PROPERTY(uint64, MemoryLimit);

        /// <summary>
        /// Gets or sets a value indicating whether script execution is enabled in the runtime.
        /// </summary>
        DECL_RW_PROPERTY(bool, IsEnabled);

        using JsRtAttr = ::Opportunity::ChakraBridge::WinRT::JsRuntimeAttributes;

        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <param name="attributes">The attributes of the runtime to be created.</param>
        /// <returns>The runtime created.</returns>
        [Overload("CreateWithAttributes")]
        static JsRuntime^ Create(JsRtAttr attributes);
        /// <summary>
        /// Creates a new runtime.
        /// </summary>
        /// <returns>The runtime created.</returns>
        [Overload("Create")]
        [DefaultOverload]
        static JsRuntime^ Create() { return Create(JsRtAttr::None); }
        /// <summary>
        /// Gets created runtimes.
        /// </summary>
        /// <returns>Collection of runtimes.</returns>
        static Windows::Foundation::Collections::IIterable<JsRuntime^>^ GetRuntimes();
    };
};
