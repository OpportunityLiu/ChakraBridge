#pragma once
#include "JsEnum.h"
#include "JsRuntime\JsRuntime.h"
#include "Native\ThrowHelper.h"
#include "JsContext.Static.h"
#include "JsContext.Scope.h"
#include "JsContext.Script.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public ref class JsContext sealed
    {
    internal:
        JsContextRef Reference;
        JsContext(JsContextRef ref);
    public:
        /// <summary>
        /// Releases reference to the script context.
        /// </summary>
        virtual ~JsContext();

        /// <summary>
        /// Gets the runtime that the context belongs to.
        /// </summary>
        property JsRuntime^ Runtime 
        {
            JsRuntime^ get();
        }

        /// <summary>
        /// Use the context in the following scope.
        /// </summary>
        /// <param name="disposeContext">
        /// Whether <see cref="~JsContext()"/> need to be invoke when invoking <see cref="JsContextScope::~JsContextScope()"/>.
        /// </param>
        /// <returns>A helper class to set the previous context back when invoking <see cref="JsContextScope::~JsContextScope()"/>.</returns>
        /// <remarks>
        /// Usage:
        /// <code>
        /// using(jsContext.Use())
        /// {
        ///     ...
        /// }
        /// </code>
        /// </remarks>
        JsContextScope^ Use(bool disposeContext);
    };
}
