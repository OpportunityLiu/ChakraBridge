namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.Serialization;

    /// <summary>
    /// A script exception.
    /// </summary>
    internal sealed class JsScriptException : JsException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="JsScriptException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="error">The JavaScript error object.</param>
        public JsScriptException(JsErrorCode code, JsError error) :
            this(code, error, "JavaScript Exception")
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="JsScriptException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="error">The JavaScript error object.</param>
        /// <param name="message">The error message.</param>
        public JsScriptException(JsErrorCode code, JsError error, string message) :
            base(code, message)
        {
            this.Error = error;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="JsScriptException"/> class.
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        /// <param name="innerException">The exception caused this exception.</param>
        public JsScriptException(JsErrorCode code, string message, Exception innerException) :
            base(code, message, innerException)
        {
        }

        /// <summary>
        /// Gets a JavaScript object representing the script error.
        /// </summary>
        public JsError Error { get; }

        /// <summary>
        /// Returns the exception that caused the runtime of the current context to be in the 
        /// exception state and resets the exception state for that runtime.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If the runtime of the current context is not in an exception state, this API will throw
        /// <c>JsErrorInvalidArgument</c>. If the runtime is disabled, this will return an exception
        /// indicating that the script was terminated, but it will not clear the exception (the 
        /// exception will be cleared if the runtime is re-enabled using 
        /// <c>EnableRuntimeExecution</c>).
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>The exception for the runtime of the current context.</returns>
        internal static JsError GetAndClearException()
        {
            Native.JsGetAndClearException(out var reference).ThrowIfError();
            return new JsError(reference);
        }

        /// <summary>
        /// Sets the runtime of the current context to an exception state.
        /// </summary>
        /// <remarks>
        /// <para>
        /// If the runtime of the current context is already in an exception state, this API will 
        /// throw <c>JsErrorInExceptionState</c>.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="error">
        /// The JavaScript exception to set for the runtime of the current context.
        /// </param>
        internal static void SetException(JsValueRef error)
        {
            Native.JsSetException(error).ThrowIfError();
        }

        /// <summary>
        /// Gets a script exception by <see cref="Native.JsGetAndClearException(out JsValueRef)"/>.
        /// </summary>
        /// <returns>
        /// A script exception of current context.
        /// </returns>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message used if no messege find from returned Js error object.</param>
        /// <exception cref="JsUsageException">The runtime of the current context is not in an exception state.</exception>
        public static JsScriptException CreateFromContext(JsErrorCode code, string message)
        {
            var error = GetAndClearException();
            try
            {
                var errorMessage = error.Message;
                if (string.IsNullOrWhiteSpace(errorMessage))
                    errorMessage = message;
                return new JsScriptException(code, error, errorMessage);
            }
            catch
            {
                return new JsScriptException(code, error, message);
            }
        }
    }
}