namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.InteropServices.WindowsRuntime;
    using Windows.Foundation.Metadata;

    partial class JsContext
    {
        /// <summary>
        /// Parses a script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to parse.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [DefaultOverload]
        public JsFunction ParseScript(string script)
            => ParseScript(script, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Parses a serialized script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [Overload("ParseSerializedScript")]
        public JsFunction ParseScript([ReadOnlyArray] byte[] buffer)
            => ParseScript(null, buffer, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <returns>The result of the script, if any.</returns>
        [DefaultOverload]
        public JsValue RunScript(string script)
            => RunScript(script, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Runs a serialized script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>The result of the script, if any.</returns>
        public JsValue RunScript([ReadOnlyArray] byte[] buffer)
            => RunScript(null, buffer, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Serializes a parsed script to a buffer than can be reused.
        /// </summary>
        /// <remarks>
        /// <para>
        /// SerializeScript parses a script and then stores the parsed form of the script in a 
        /// runtime-independent format. The serialized script then can be deserialized in any
        /// runtime without requiring the script to be re-parsed.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <param name="script">The script to serialize.</param>
        /// <returns>
        /// The size of the buffer, in bytes, required to hold the serialized script.
        /// </returns>
        public byte[] SerializeScript(string script)
        {
            if (script == null)
                throw new ArgumentNullException(nameof(script));
            var bufferSize = 0UL;
            Native.JsSerializeScript(script, null, ref bufferSize).ThrowIfError();
            var rBuf = new byte[bufferSize];
            Native.JsSerializeScript(script, rBuf, ref bufferSize).ThrowIfError();
            return rBuf;
        }
    }
}
