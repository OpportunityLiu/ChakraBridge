namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Runtime.InteropServices.WindowsRuntime;
    using Windows.Foundation.Metadata;
    using Windows.Storage.Streams;

    partial class JsContext
    {
        /// <summary>
        /// Parses a script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to parse.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [DefaultOverload]
        [Overload("ParseScript")]
        public static JsFunction ParseScript(string script)
            => ParseScript(script, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Parses a serialized script and returns a <c>Function</c> representing the script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>A <c>Function</c> representing the script code.</returns>
        [Overload("ParseSerializedScript")]
        public static JsFunction ParseScript(IBuffer buffer)
            => ParseScript(null, buffer, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Executes a script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="script">The script to run.</param>
        /// <returns>The result of the script, if any.</returns>
        [DefaultOverload]
        [Overload("RunScript")]
        public static JsValue RunScript(string script)
            => RunScript(script, JsSourceContextExtension.None, string.Empty);

        /// <summary>
        /// Runs a serialized script.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="buffer">The serialized script.</param>
        /// <returns>The result of the script, if any.</returns>
        [Overload("RunSerializedScript")]
        public static JsValue RunScript(IBuffer buffer)
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
        public unsafe static IBuffer SerializeScript(string script)
        {
            if (script == null)
                throw new ArgumentNullException(nameof(script));
            var bufferSize = 0UL;
            Native.JsSerializeScript(script, null, ref bufferSize).ThrowIfError();
            var buf = new Windows.Storage.Streams.Buffer(checked((uint)bufferSize));
            Native.JsSerializeScript(script, buf.GetPointer(), ref bufferSize).ThrowIfError();
            buf.Length = checked((uint)bufferSize);
            return buf;
        }
    }
}
