namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;

    [DebuggerDisplay(@"[{Disp,nq}]")]
    internal readonly struct JsPropertyIdRef
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private string Disp => this.Value.GetDebugDisp(Native.JS_INVALID_PROPERTYID, "(INVALID_PROPERTYID)");

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public bool IsValid => this.Value != Native.JS_INVALID_PROPERTYID;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public static JsPropertyIdRef Invalid => new JsPropertyIdRef(Native.JS_INVALID_PROPERTYID);

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public readonly IntPtr Value;

        public JsPropertyIdRef(IntPtr value) => this.Value = value;
    }
}