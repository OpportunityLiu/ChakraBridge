using Opportunity.ChakraBridge.WinRT;
using Opportunity.ChakraBridge.UWP.Browser;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Foundation.Metadata;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Web.Http;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace Test
{
    public class ib { public string get() => "124"; }

    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="e"></param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            try
            {
                var js = new FileInfo(@"test.js");
                using (var runtime = JsRuntime.Create())
                {
                    var rt = JsRuntime.GetRuntimes().ToList();
                    //runtime.AllocatingMemory += this.Runtime_MemoryEvent;
                    runtime.CollectingGarbage += this.Runtime_CollectingGarbage;
                    using (var c = runtime.CreateContext())
                    {
                        using (c.Use(false))
                        {
                            var o = JsObject.Create();
                            o.ObjectCollectingCallback = aaa;
                            var func = JsFunction.Of(saf, "");
                            JsValue.GlobalObject["ss"] = func;
                            var funcr = JsContext.RunScript("ss(1,'as',{}, null)");
                            var a = (IJsArray)JsContext.RunScript("[1,'2', {}]");
                            var ex = JsError.CreateRangeError("");
                            var exm = ex.Message;
                            var b = ((IDictionary<string, IJsValue>)a).ToArray();
                            var cd = (IJsObject)JsContext.RunScript("({},,)");
                            //var testobj = JsValue.Create(new System.Net.Http.HttpClient());
                            //var x = JsString.Create("12");
                            //var xx = JsValue.Equals(x, x);
                            //var a = JsValue.ToJsObject(x);
                            //var aa = a.ToString();
                            //var r = (IJsString)JsContext.RunScript("''");
                        }
                    }
                }

            }
            catch (Exception)
            {
            }
        }

        private IJsValue saf(IJsFunction callee, IJsObject caller, bool isConstructCall, IReadOnlyList<IJsValue> arguments)
        {
            return null;
        }

        private void aaa(IJsObject obj)
        {

        }

        private bool sss(out string scriptBuffer)
        {
            scriptBuffer = @"//The JavaScript ES6 Promise code goes here
new Promise(
 function(resolve, reject) {resolve('basic:success');}
).then(function () {return new Promise(
    function(resolve, reject) {resolve('second:success')}
)});";
            return true;
        }

        private void Con_Logging(Console sender, ConsoleLoggingEventArgs args)
        {
        }

        //private JsValue func(JsFunction callee, JsObject caller, bool isConstructCall, IList<JsValue> arguments)
        //{
        //    return caller;
        //}

        //private void ObjectCollectingCallback(JsObject obj)
        //{

        //}

        private void Runtime_CollectingGarbage(JsRuntime sender, object args)
        {
            Debug.WriteLine("Charka GC");
        }

        private void Runtime_MemoryEvent(JsRuntime sender, JsMemoryEventArgs args)
        {
            Debug.WriteLine($"{args.EventType} {args.AllocationSize} bytes");
            //args.IsRejected = true;
        }

        //private JsValue PostCallback(JsFunction callee, bool isConstructCall, IList<JsValue> arguments)
        //{
        //    return JsValue.Undefined;
        //}
    }
}
