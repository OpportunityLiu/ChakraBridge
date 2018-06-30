namespace Opportunity.ChakraBridge.UWP
{
    using System;
    using System.Diagnostics;

    [DebuggerDisplay(@"[{Disp,nq}]")]
    internal readonly struct JsRuntimeHandle : IEquatable<JsRuntimeHandle>
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        private string Disp => this.Value.GetDebugDisp(Native.JS_INVALID_RUNTIME_HANDLE, "(INVALID_RUNTIME_HANDLE)");

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        internal readonly IntPtr Value;

        public bool Equals(JsRuntimeHandle other) => this.Value == other.Value;

        public override bool Equals(object obj) => obj is JsRuntimeHandle other && Equals(other);

        public override int GetHashCode() => this.Value.GetHashCode();

        internal JsRuntimeHandle(IntPtr value) => this.Value = value;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public bool IsValid => this.Value != Native.JS_INVALID_RUNTIME_HANDLE;

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        public static JsRuntimeHandle Invalid { get; } = new JsRuntimeHandle(Native.JS_INVALID_RUNTIME_HANDLE);

        public static bool operator ==(JsRuntimeHandle h1, JsRuntimeHandle h2) => h1.Value == h2.Value;
        public static bool operator !=(JsRuntimeHandle h1, JsRuntimeHandle h2) => h1.Value != h2.Value;

    }
}
