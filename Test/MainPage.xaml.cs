using Opportunity.ChakraBridge.UWP;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
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
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            using (var runtime = JsRuntime.Create())
            {
                runtime.MemoryEvent += this.Runtime_MemoryEvent;
                runtime.CollectingGarbage += this.Runtime_CollectingGarbage;
                while (true)
                {
                    using (runtime.CreateContext().Use(true))
                    {
                        JsContext.ProjectWinRTNamespace("Windows");
                        var n = JsSourceContextExtension.None;
                        var r = (JsFunction)JsContext.RunScript(@"a = function aa(){this.args = arguments; return this;}");
                    }
                    runtime.CollectGarbage();
                    runtime.CollectGarbage();
                    runtime.CollectGarbage();
                }
            }
        }

        private JsValue func(JsFunction callee, JsObject caller, bool isConstructCall, IList<JsValue> arguments)
        {
            return caller;
        }

        private void ObjectCollectingCallback(JsObject obj)
        {

        }

        private void Runtime_CollectingGarbage(JsRuntime sender, object args)
        {
            Debug.WriteLine("Charka GC");
        }

        private void Runtime_MemoryEvent(JsRuntime sender, JsMemoryEventArgs args)
        {
            Debug.WriteLine($"{args.EventType} {args.AllocationSize} bytes");
            //args.IsRejected = true;
        }

        private JsValue PostCallback(JsFunction callee, bool isConstructCall, IList<JsValue> arguments)
        {
            return JsValue.Undefined;
        }
    }
}
