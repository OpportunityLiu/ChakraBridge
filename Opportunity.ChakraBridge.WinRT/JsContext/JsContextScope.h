#pragma once
#include "JsRuntime\JsRuntime.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT {

    ref class JsContext;

    /// <summary>
    /// A helper class to set the previous context back when invoking <see cref="~JsContextScope()"/>.
    /// </summary>
    public ref class JsContextScope sealed
    {
    private:
        JsContextRef previous;
        JsContext^ current;
        const bool disposeContext;
    internal:
        JsContextScope(JsContext^ jsContext, bool disposeContext);

    public:
        /// <summary>
        /// Release references to contexts.
        /// </summary>
        virtual ~JsContextScope();
    };
};