namespace Opportunity.ChakraBridge.UWP
{
    using System.Collections.Generic;

    partial class JsRuntime
    {
        internal readonly JsFuncTable<JsNativeFunction> FuncTable = new JsFuncTable<JsNativeFunction>();
        internal readonly JsFuncTable<JsObjectBeforeCollectCallback> ObjectBeforeCollectTable = new JsFuncTable<JsObjectBeforeCollectCallback>();
    }
}
