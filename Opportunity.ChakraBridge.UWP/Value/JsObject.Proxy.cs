using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading;
using Opportunity.ChakraBridge.UWP;

[assembly: DebuggerDisplay(@"Count = {Length}", Target = typeof(JsObject.Property[]))]

namespace Opportunity.ChakraBridge.UWP
{
    //[DebuggerTypeProxy(typeof(ObjectProxy))]
    partial class JsObject
    {
        [DebuggerDisplay(@"{GetDebugDisp(),nq}", Name = @"[{Key.ToString(),nq}]", Type = @"{Value.GetType().ToString(),nq}")]
        internal sealed class Property
        {
            public Property(JsPropertyId key, object value)
            {
                this.Key = key;
                this.Value = value;
            }

            [DebuggerBrowsable(DebuggerBrowsableState.Never)]
            public readonly JsPropertyId Key;

            [DebuggerBrowsable(DebuggerBrowsableState.RootHidden)]
            public readonly object Value;

            private string GetDebugDisp()
            {
                if (this.Value is JsValue v)
                    return v.GetDebugDisp();
                if (this.Value is Exception ex)
                    System.Runtime.ExceptionServices.ExceptionDispatchInfo.Capture(ex).Throw();
                return this.Value.ToString();
            }
        }

        internal Property[] Properties
        {
            get
            {
                var nk = this.GetOwnPropertyNames();
                var sk = this.GetOwnPropertySymbols();
                var nkc = nk.Count;
                var skc = sk.Count;
                var r = new Property[nk.Count + sk.Count];
                for (var i = 0; i < nkc; i++)
                {
                    var k = nk[i].ToString();
                    var p = JsPropertyId.FromString(k);
                    r[i] = new Property(p, GetValue(p));
                }
                for (var i = 0; i < skc; i++)
                {
                    var s = (JsSymbol)sk[i];
                    var p = JsPropertyId.FromSymbol(s);
                    r[i + nkc] = new Property(p, GetValue(p));
                }
                return r;

                object GetValue(JsPropertyId id)
                {
                    try
                    {
                        return CreateTyped(RawProperty.GetProperty(this.Reference, id));
                    }
                    catch (Exception ex)
                    {
                        return ex;
                    }
                }
            }
        }

        //internal class ObjectProxy
        //{
        //    private readonly JsObject obj;

        //    public ObjectProxy(JsObject obj)
        //    {
        //        this.obj = obj;
        //    }

        //    public bool IsExtensionAllowed => this.obj.IsExtensionAllowed;
        //    public JsContext Context => this.obj.Context;
        //    public JsObjectBeforeCollectCallback ObjectCollectingCallback => this.obj.ObjectCollectingCallback;
        //    public JsObject Prototype => this.obj.Prototype;
        //    public JsValueRef Reference => this.obj.Reference;
        //    public JsValueType ValueType => this.obj.ValueType;
        //}
    }
}
