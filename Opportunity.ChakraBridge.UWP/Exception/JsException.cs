namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.Serialization;

    /// <summary>
    ///     An exception returned from the Chakra engine.
    /// </summary>
    internal class JsException : Exception
    {
        /// <summary>
        ///     Initializes a new instance of the <see cref="JsException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        public JsException(JsErrorCode code) :
            this(code, "A fatal exception has occurred in a JavaScript runtime")
        {
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        public JsException(JsErrorCode code, string message) :
            this(code, message, null)
        {
        }

        /// <summary>
        ///     Initializes a new instance of the <see cref="JsException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        /// <param name="innerException">The exception caused this exception.</param>
        public JsException(JsErrorCode code, string message, Exception innerException) :
            base(message, innerException)
        {
            ErrorCode = code;
            Data["JsErrorCode"] = code.ToString();
        }

        /// <summary>
        ///     The error code.
        /// </summary>
        public JsErrorCode ErrorCode { get; }
    }
}