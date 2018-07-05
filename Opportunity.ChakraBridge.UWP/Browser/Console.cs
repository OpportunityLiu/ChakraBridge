using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace Opportunity.ChakraBridge.UWP.Browser
{
    /// <summary>
    /// WinRT implementation of js Console object.
    /// </summary>
    public sealed class Console
    {
        private Console() { }

        /// <summary>
        /// Raises when any method of console has been invoked.
        /// </summary>
        public event TypedEventHandler<Console, ConsoleLoggingEventArgs> Logging;

        /// <summary>
        /// Get or create a console object, and put it to <see cref="JsValue.GlobalObject"/> of current context.
        /// </summary>
        /// <returns>
        /// The created console object.
        /// </returns>
        public static Console GetOrCreate()
        {
            try
            {
                if (JsValue.GlobalObject.Get("console") is JsObject oldCon &&
                    oldCon.Get("rtConsole") is JsObject jsStore &&
                    jsStore.ToInspectable() is Console c)
                    return c;
            }
            catch { }
            var con = new Console();
            var jsConStore = JsValue.Create(con);
            var callback = JsFunction.Create(OnLogging);
            var func = (JsFunction)JsContext.RunScript(@"
(function () {
    let logging = Symbol('LoggingCallback');
    let getStack = Symbol('GetStack');
    function Console(console, callback) {
        this.rtConsole = console;
        this[logging] = callback;
        Object.preventExtensions(this);
    }

    Console.prototype[logging] = null;
    Console.prototype[getStack] = function(){ try{ throw new Error();}catch(ex){return ex.stack;} }

    Console.prototype.assert = function (cond, ...data) { if (cond) this[logging]('assert', data, this[getStack]()); };
    Console.prototype.clear = function (...data) { this[logging]('clear', data, this[getStack]()); };
    Console.prototype.count = function (...data) { this[logging]('count', data, this[getStack]()); };
    Console.prototype.debug = function (...data) { this[logging]('debug', data, this[getStack]()); };
    Console.prototype.dir = function (...data) { this[logging]('dir', data, this[getStack]()); };
    Console.prototype.dirxml = function (...data) { this[logging]('dirxml', data, this[getStack]()); };
    Console.prototype.error = function (...data) { this[logging]('error', data, this[getStack]()); };
    Console.prototype.exception = Console.prototype.error;
    Console.prototype.group = function (...data) { this[logging]('group', data, this[getStack]()); };
    Console.prototype.groupCollapsed = function (...data) { this[logging]('groupCollapsed', data, this[getStack]()); };
    Console.prototype.groupEnd = function (...data) { this[logging]('groupEnd', data, this[getStack]()); };
    Console.prototype.info = function (...data) { this[logging]('info', data, this[getStack]()); };
    Console.prototype.log = function (...data) { this[logging]('log', data, this[getStack]()); };
    Console.prototype.profile = function (...data) { this[logging]('profile', data, this[getStack]()); };
    Console.prototype.profileEnd = function (...data) { this[logging]('profileEnd', data, this[getStack]()); };
    Console.prototype.table = function (...data) { this[logging]('table', data, this[getStack]()); };
    Console.prototype.time = function (...data) { this[logging]('time', data, this[getStack]()); };
    Console.prototype.timeEnd = function (...data) { this[logging]('timeEnd', data, this[getStack]()); };
    Console.prototype.timeStamp = function (...data) { this[logging]('timeStamp', data, this[getStack]()); };
    Console.prototype.trace = function (...data) { this[logging]('trace', data, this[getStack]()); };
    Console.prototype.warn = function (...data) { this[logging]('warn', data, this[getStack]()); };

    Object.preventExtensions(Console);
    Object.preventExtensions(Console.prototype);
    return Console;
})()

");
            var console = func.New(jsConStore, callback);
            JsValue.GlobalObject.Set("console", console);
            return con;
        }

        private static JsValue OnLogging(JsFunction callee, JsObject caller, bool isConstructCall, IList<JsValue> arguments)
        {
            var con = (Console)caller.Get("rtConsole").ToInspectable();
            var l = con.Logging;
            if (l is null)
                return null;
            l(con, new ConsoleLoggingEventArgs
            {
                Verb = arguments[0].ToString(),
                Data = (JsArray)arguments[1],
                CallStack = arguments[2].ToString().Substring("Error\n   at Console.prototype[getStack] (Unknown script code:12:52)\n   at Console.prototype.error (Unknown script code:20:52)\n".Length),
            });
            return null;
        }
    }

    /// <summary>
    /// Provide args for <see cref="Console.Logging"/> event.
    /// </summary>
    public sealed class ConsoleLoggingEventArgs
    {
        /// <summary>
        /// The name of function be invoked.
        /// </summary>
        public string Verb { get; internal set; }

        /// <summary>
        /// Arguments of invoke.
        /// </summary>
        public IReadOnlyList<JsValue> Data { get; internal set; }

        /// <summary>
        /// Js call stack of the invoke.
        /// </summary>
        public string CallStack { get; internal set; }
    }
}
