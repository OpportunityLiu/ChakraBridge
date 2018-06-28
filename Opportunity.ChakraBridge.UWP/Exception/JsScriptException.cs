namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.Serialization;

    /// <summary>
    ///     A script exception.
    /// </summary>
    internal sealed class JsScriptException : JsException
    {
        /// <summary>
        ///     Initializes a new instance of the <see cref="JsScriptException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="error">The JavaScript error object.</param>
        public JsScriptException(JsErrorCode code, JsValueReference error) :
            this(code, error, "JavaScript Exception")
        {
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsScriptException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="error">The JavaScript error object.</param>
        /// <param name="message">The error message.</param>
        public JsScriptException(JsErrorCode code, JsValueReference error, string message) :
            base(code, message)
        {
            this.Error = error;
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsScriptException"/> class.
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        /// <param name="innerException">The exception caused this exception.</param>
        public JsScriptException(JsErrorCode code, string message, Exception innerException) :
            base(code, message, innerException)
        {
        }

        /// <summary>
        ///     Gets a JavaScript object representing the script error.
        /// </summary>
        public JsValueReference Error { get; }

        /// <summary>
        ///     Gets a script exception by <see cref="Native.JsGetAndClearException(out JsValueReference)"/>.
        /// </summary>
        /// <returns>
        ///     A script exception of current context.
        /// </returns>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message used if no messege find from returned Js error object.</param>
        /// <exception cref="JsUsageException">The runtime of the current context is not in an exception state.</exception>
        public static JsScriptException CreateFromContext(JsErrorCode code, string message)
        {
            Native.JsGetAndClearException(out var errorObject).ThrowIfError();
            try
            {
                var errorMessage = errorObject.GetProperty(JsPropertyId.FromString("message")).ToString();
                var errorStack = errorObject.GetProperty(JsPropertyId.FromString("stack")).ToString();
                var errorNumber = errorObject.GetProperty(JsPropertyId.FromString("number")).ToInt32();
                return new JsScriptException(code, errorObject, errorMessage)
                {
                    Data =
                    {
                        ["Stack"] = errorStack,
                        ["Number"] = errorNumber,
                    }
                };
            }
            catch
            {
                return new JsScriptException(code, errorObject, message);
            }
        }
    }
}