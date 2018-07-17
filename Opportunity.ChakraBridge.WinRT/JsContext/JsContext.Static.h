#pragma once
#include "JsEnum.h"
#include "JsRuntime\JsRuntime.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    partial ref class JsContext sealed
    {
    internal:
        /// <summary>
        /// Get the instance of the <see cref="JsContext"/>, or <see langword="null"/>, if <paramref name="reference"/> is <see cref="JS_INVALID_REFERENCE"/>. 
        /// </summary>
        /// <param name="reference">The reference.</param>
        /// <returns>The instance of the <see cref="JsContext"/></returns>
        static JsContext^ Get(JsContextRef reference);

    public:
        /// <summary>
        /// Gets or sets the current script context on the thread.
        /// </summary>
        static property JsContext^ Current
        {
            JsContext^ get();
            void set(JsContext^ value);
        };

        /// <summary>
        /// Gets a value indicating whether the runtime of the current context is in an exception state.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If a call into the runtime results in an exception (either as the result of running a 
        /// script or due to something like a conversion failure), the runtime is placed into an 
        /// "exception state." All calls into any context created by the runtime (except for the 
        /// exception APIs) will fail with <c>InExceptionState</c> until the exception is 
        /// cleared.
        /// </para>
        /// <para>
        /// If the runtime of the current context is in the exception state when a callback returns 
        /// into the engine, the engine will automatically rethrow the exception.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        static property bool HasException
        {
            bool get();
        }

        /// <summary>
        /// Starts debugging in the current context. 
        /// </summary>
        static void StartDebugging();

        /// <summary>
        /// Tells the runtime to do any idle processing it need to do.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If idle processing has been enabled for the current runtime, calling <c>Idle</c> will 
        /// inform the current runtime that the host is idle and that the runtime can perform 
        /// memory cleanup tasks.
        /// </para>
        /// <para>
        /// <c>Idle</c> will also return the number of system ticks until there will be more idle work
        /// for the runtime to do. Calling <c>Idle</c> before this number of ticks has passed will do
        /// no work.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>
        /// The next system tick when there will be more idle work to do. Returns the 
        /// maximum number of ticks if there no upcoming idle work to do.
        /// </returns>
        static uint32 Idle();

        /// <summary>
        /// Project a WinRT namespace. 
        /// </summary>
        /// <param name="namespaceName">The WinRT namespace to be projected. </param>
        /// <remarks>Requires an active script context.</remarks>
        static void ProjectWinRTNamespace(Platform::String^ namespaceName);
    };
};