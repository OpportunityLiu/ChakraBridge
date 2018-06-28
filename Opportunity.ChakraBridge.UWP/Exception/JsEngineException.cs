namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.Serialization;

    /// <summary>
    ///     An exception that occurred in the workings of the JavaScript engine itself.
    /// </summary>
    internal sealed class JsEngineException : JsException
    {
        /// <summary>
        ///     Initializes a new instance of the <see cref="JsEngineException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        public JsEngineException(JsErrorCode code) :
            this(code, "A fatal exception has occurred in a JavaScript runtime")
        {
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsEngineException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        public JsEngineException(JsErrorCode code, string message) :
            base(code, message)
        {
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsEngineException"/> class.
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        /// <param name="innerException">The exception caused this exception.</param>
        public JsEngineException(JsErrorCode code, string message, Exception innerException) :
            base(code, message, innerException)
        {
        }
    }
}