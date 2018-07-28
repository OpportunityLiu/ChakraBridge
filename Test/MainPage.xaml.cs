using Opportunity.ChakraBridge.WinRT;
using Opportunity.ChakraBridge.WinRT.Browser;
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
    public class JsArrayDebugView
    {
        private readonly IJsArray array;

        public JsArrayDebugView(object array)
        {
            this.array = (IJsArray)array;
        }

        [DebuggerBrowsable(DebuggerBrowsableState.RootHidden)]
        public IJsValue[] Items => ((IList<IJsValue>)this.array).ToArray();
    }

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
        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            try
            {
                var js = new FileInfo(@"test.js");
                using (var runtime = JsRuntime.Create())
                {
                    runtime.AllocatingMemory += this.Runtime_MemoryEvent;
                    runtime.CollectingGarbage += this.Runtime_CollectingGarbage;
                    using (runtime.CreateContext().Use(true))
                    {
                        try
                        {
                            var obj = JsExternalObject.Create(this);
                            obj.ExternalData = 12;
                            obj.ExternalData = 100;
                            JsContext.StartDebugging();
                            var func = JsFunction.Create(saf);
                            var console = Opportunity.ChakraBridge.WinRT.Browser.Console.GetOrCreate();
                            console.Logging += Con_Logging;
                            var c2 = Opportunity.ChakraBridge.WinRT.Browser.Console.GetOrCreate();
                            JsValue.GlobalObject["func"] = func;
                            JsContext.RunScript("console.log(1,'2',{});func(1,'2',{});new func();", js.Name);
                        }
                        catch (Exception)
                        {

                            var error = JsContext.LastError;
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

        private void Con_Logging(Opportunity.ChakraBridge.WinRT.Browser.Console sender, IConsoleLoggingEventArgs args)
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

        private void Runtime_MemoryEvent(JsRuntime sender, IJsAllocatingMemoryEventArgs args)
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
