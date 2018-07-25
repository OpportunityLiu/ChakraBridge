#pragma once
#include "alias.h"
#include "Value\Declare.h"

namespace Opportunity::ChakraBridge::WinRT::Browser
{
    ref class Console;

    /// <summary>
    /// Provide args for logging event.
    /// </summary>
    public interface class IConsoleLoggingEventArgs
    {
        /// <summary>
        /// The name of function be invoked.
        /// </summary>
        DECL_R_PROPERTY(string^, Verb);

        /// <summary>
        /// Arguments of invoke.
        /// </summary>
        DECL_R_PROPERTY(vector_view<IJsValue>^, Data);

        /// <summary>
        /// Js call stack of the invoke.
        /// </summary>
        DECL_R_PROPERTY(string^, CallStack);
    };

    ref struct ConsoleLoggingEventArgs sealed : IConsoleLoggingEventArgs
    {
        virtual property string^ Verb;
        virtual property vector_view<IJsValue>^ Data;
        virtual property string^ CallStack;
    };

    public interface struct IConsole
    {
        event typed_event_handler<Console, IConsoleLoggingEventArgs>^ Logging;
    };

    /// <summary>
    /// WinRT implementation of js Console object.
    /// </summary>
    public ref class Console sealed: [Default] IConsole
    {
    private:
        Console() { }

    internal:
        static RawValue Console::OnLogging(const RawValue& callee, const RawValue& caller, const bool isConstructCall, const RawValue*const arguments, const unsigned short argumentCount, const nullptr_t&);

    public:
        /// <summary>
        /// Raises when any method of console has been invoked.
        /// </summary>
        virtual event typed_event_handler<Console, IConsoleLoggingEventArgs>^ Logging;

        /// <summary>
        /// Get or create a console object, and put it to <see cref="JsValue::GlobalObject"/> of current context.
        /// </summary>
        /// <returns>
        /// The created console object.
        /// </returns>
        static Console^ GetOrCreate();
    };
}
