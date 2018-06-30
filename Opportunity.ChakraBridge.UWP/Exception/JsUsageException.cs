namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.Serialization;

    /// <summary>
    /// An API usage exception occurred.
    /// </summary>
    internal sealed class JsUsageException : JsException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="JsUsageException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        public JsUsageException(JsErrorCode code) :
            this(code, "A fatal exception has occurred in a JavaScript runtime")
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="JsUsageException"/> class. 
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        public JsUsageException(JsErrorCode code, string message) :
            base(code, message)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="JsUsageException"/> class.
        /// </summary>
        /// <param name="code">The error code returned.</param>
        /// <param name="message">The error message.</param>
        /// <param name="innerException">The exception caused this exception.</param>
        private JsUsageException(JsErrorCode code, string message, Exception innerException) :
            base(code, message, innerException)
        {
        }
    }
}