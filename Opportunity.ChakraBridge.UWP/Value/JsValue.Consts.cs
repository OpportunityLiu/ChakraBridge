namespace Opportunity.ChakraBridge.UWP
{
    partial class JsValue
    {
        /// <summary>
        /// Gets the value of <c>true</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsBoolean True => new JsBoolean(RawValue.True);

        /// <summary>
        /// Gets the value of <c>false</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsBoolean False => new JsBoolean(RawValue.False);

        /// <summary>
        /// Gets the value of <c>undefined</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsUndefined Undefined => new JsUndefined(RawValue.Undefined);

        /// <summary>
        /// Gets the value of <c>null</c> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsNull Null => new JsNull(RawValue.Null);

        /// <summary>
        /// Gets the global object in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        public static JsObject GlobalObject => new JsObject(RawValue.GlobalObject);
    }
}
