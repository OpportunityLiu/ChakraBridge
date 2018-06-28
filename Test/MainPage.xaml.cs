using Opportunity.ChakraBridge.UWP;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace Test
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            var scr = @"I=~[];I={___:++I,$$$$:(![]+"""")[I],__$:++I,$_$_:(![]+"""")[I],_$_:++I,$_$$:({}+"""")[I],$$_$:(I[I]+"""")[I],_$$:++I,$$$_:(!""""+"""")[I],$__:++I,$_$:++I,$$__:({}+"""")[I],$$_:++I,$$$:++I,$___:++I,$__$:++I};I.$_=(I.$_=I+"""")[I.$_$]+(I._$=I.$_[I.__$])+(I.$$=(I.$+"""")[I.__$])+((!I)+"""")[I._$$]+(I.__=I.$_[I.$$_])+(I.$=(!""""+"""")[I.__$])+(I._=(!""""+"""")[I._$_])+I.$_[I.$_$]+I.__+I._$+I.$;I.$$=I.$+(!""""+"""")[I._$$]+I.__+I._+I.$+I.$$;I.$=(I.___)[I.$_][I.$_];I.$(I.$(I.$$+""\""""+""$.\\""+I.__$+I.$$_+I.___+I._$+""\\""+I.__$+I.$$_+I._$$+I.__+""('/""+I.$_$_+""\\""+I.__$+I.$_$+I._$_+I.$_$_+""\\""+I.__$+I.$$$+I.___+""/""+I.$$__+I._$+""\\""+I.__$+I.$_$+I.$$_+I.__+I.$$$_+""\\""+I.__$+I.$_$+I.$$_+I.__+""/',{\\""+I.__$+I.$$_+I._$$+""\\""+I.__$+I.$_$+I.__$+I.$$_$+""\\""+I.$$$+I._$_+I.__$+I.__$+I.__$+I.__$+I.$__+"",\\""+I.__$+I.$$$+I._$_+""\\""+I.__$+I.$_$+I.__$+I.$$_$+""\\""+I.$$$+I._$_+I._$_+I.$_$+I.__$+I.$$$+I._$_+I.$__+I.$__+I.$__$+"",""+I.$$__+""\\""+I.__$+I.$_$+I.__$+I.$$_$+""\\""+I.$$$+I._$_+I.$_$+""},""+I.$$$$+I._+""\\""+I.__$+I.$_$+I.$$_+I.$$__+I.__+""\\""+I.__$+I.$_$+I.__$+I._$+""\\""+I.__$+I.$_$+I.$$_+""(""+I.$$_$+I.$_$_+I.__+I.$_$_+""){$('#""+I.$$__+I._$+""\\""+I.__$+I.$_$+I.$$_+I.__+I.$$$_+""\\""+I.__$+I.$_$+I.$$_+I.__+""').\\""+I.__$+I.$_$+I.___+I.__+""\\""+I.__$+I.$_$+I.$_$+(![]+"""")[I._$_]+""(""+I.$$_$+I.$_$_+I.__+I.$_$_+"")\\""+I.$$$+I._$$+""})\\""+I.$$$+I._$$+""\"""")())();";

            var c = JsContext.Current;
            using (var runtime = JsRuntime.Create())
            {
                c = JsContext.Current = runtime.CreateContext();
                var d = JsContext.SerializeScript(scr);
                var o = new JsExtenalObject(default, null);
                var i = JsPropertyId.FromString("你好abc");
                var s = (JsSymbol)JsContext.RunScript("Symbol('sym')");
                JsContext.GlobalObject["$"] = o;
                o["post"] = new JsFunction(PostCallback);
                var k = ((IDictionary<JsPropertyId, JsValue>)o).Keys;
                var a = ((JsArray)JsContext.RunScript("[1,2,3]"));
                var dr = JsContext.Debugger.RunScript(null, d, JsSourceContext.None, null);
                var r = JsContext.RunScript("d = {a:1,b:''}").ToJsObject();
            }
        }

        private JsValue PostCallback(JsFunction callee, bool isConstructCall, IList<JsValue> arguments)
        {
            return JsValue.Undefined;
        }
    }
}
