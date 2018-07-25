#include "pch.h"
#include "Console.h"
#include <functional>

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge::WinRT::Browser;

#define RT_EXT_CONSOLE_NAME L"__rt_external_console__"

IJsValue^ Console::OnLogging(IJsFunction^ callee, IJsObject^ caller, bool isConstructCall, vector_view<IJsValue>^ arguments)
{
    auto con = safe_cast<Console^>(to_impl(caller)->Lookup("rtConsole")->ToInspectable());
    auto args = ref new ConsoleLoggingEventArgs();
    args->Verb = arguments->GetAt(0)->ToString();
    args->Data = safe_cast<vector<IJsValue>^>(arguments->GetAt(1))->GetView();
    args->CallStack = ref new string(arguments->GetAt(2)->ToString()->Data() + sizeof("Error\n   at Console.prototype[getStack] (Unknown script code:12:52)\n   at Console.prototype.error (Unknown script code:20:52)\n"));

    con->Logging(con, args);
    return nullptr;
}

Console^ Console::GetOrCreate()
{
    try
    {
        auto oldConsole = dynamic_cast<JsObjectImpl^>(to_impl(JsValue::GlobalObject)->Lookup(L"console"));
        if (oldConsole != nullptr)
        {
            auto con = dynamic_cast<JsExternalObjectImpl^>(oldConsole->Lookup(RT_EXT_CONSOLE_NAME));
            if (con != nullptr)
                return safe_cast<Console^>(con->ExternalData);
        }
    }
    catch (...) {}
    auto con = ref new Console();
    auto jsCon = JsExternalObject::Create(con);
    auto callback = JsFunction::Create(ref new JsNativeFunction(OnLogging));
    auto func = safe_cast<JsFunctionImpl^>(JsContext::RunScript(LR"(
    (function() {
        let logging = Symbol('LoggingCallback');
        let getStack = Symbol('GetStack');
        class Console
        {
            constructor(console, callback)
            {
                this.)" RT_EXT_CONSOLE_NAME LR"( = console;
                Object.defineProperty(this, logging, { get() { return callback }, configurable: false, enumerable: false });
            }
            assert(cond, ...data)
            {
            if (cond)
                this[logging]('assert', data, this[getStack]());
            }
            clear(...data) { this[logging]('clear', data, this[getStack]()); }
            count(...data) { this[logging]('count', data, this[getStack]()); }
            debug(...data) { this[logging]('debug', data, this[getStack]()); }
            dir(...data) { this[logging]('dir', data, this[getStack]()); }
            dirxml(...data) { this[logging]('dirxml', data, this[getStack]()); }
            error(...data) { this[logging]('error', data, this[getStack]()); }
            exception(...data) { this[logging]('error', data, this[getStack]()); }
            group(...data) { this[logging]('group', data, this[getStack]()); }
            groupCollapsed(...data) { this[logging]('groupCollapsed', data, this[getStack]()); }
            groupEnd(...data) { this[logging]('groupEnd', data, this[getStack]()); }
            info(...data) { this[logging]('info', data, this[getStack]()); }
            log(...data) { this[logging]('log', data, this[getStack]()); }
            profile(...data) { this[logging]('profile', data, this[getStack]()); }
            profileEnd(...data) { this[logging]('profileEnd', data, this[getStack]()); }
            table(...data) { this[logging]('table', data, this[getStack]()); }
            time(...data) { this[logging]('time', data, this[getStack]()); }
            timeEnd(...data) { this[logging]('timeEnd', data, this[getStack]()); }
            timeStamp(...data) { this[logging]('timeStamp', data, this[getStack]()); }
            trace(...data) { this[logging]('trace', data, this[getStack]()); }
            warn(...data) { this[logging]('warn', data, this[getStack]()); }

            [getStack]() { try { throw new Error(); } catch (ex) { return ex.stack; } }
        }
        Object.preventExtensions(Console);
        Object.preventExtensions(Console.prototype);
        return Console;
    })()
)"));
    auto console = get_ref(func).New(get_ref(jsCon), get_ref(callback));
    RawValue::GlobalObject()[L"console"] = console;
    return con;
}
