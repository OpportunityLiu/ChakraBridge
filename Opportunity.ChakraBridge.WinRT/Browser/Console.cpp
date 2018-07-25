#include "pch.h"
#include "Console.h"
#include <functional>

using namespace Opportunity::ChakraBridge::WinRT;
using namespace Opportunity::ChakraBridge::WinRT::Browser;

#define RT_EXT_CONSOLE_NAME L"__rt_external_console__"

RawValue Console::OnLogging(const RawValue& callee, const RawValue& caller, const bool isConstructCall, const RawValue*const arguments, const unsigned short argumentCount, const nullptr_t&)
{
    _ASSERTE(argumentCount == 3);
    auto con = reinterpret_cast<Console^>(caller[RT_EXT_CONSOLE_NAME]().ToInspectable());
    auto args = ref new ConsoleLoggingEventArgs();
    args->Verb = arguments[0].ToString();
    args->Data = (ref new JsArrayImpl(arguments[1]))->ArrayGetView();
    auto callstack = arguments[2].ToString();
    auto callstackptr = callstack.Data();
    auto lf = 0;
    while (*callstackptr != L'\0' && lf < 3)
    {
        if (*callstackptr == L'\n')
            lf++;
        callstackptr++;
    }
    args->CallStack = ref new string(callstackptr, unsigned int(callstack.Data() + callstack.Length() - callstackptr));

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
            auto con = safe_cast<Console^>(to_impl(oldConsole)->Lookup(RT_EXT_CONSOLE_NAME)->ToInspectable());
            if (con != nullptr)
                return con;
        }
    }
    catch (...) {}
    auto con = ref new Console();
    auto jsCon = RawValue(reinterpret_cast<IInspectable*>(con));
    auto callback = RawValue::CreateFunction<nullptr_t, OnLogging>(nullptr);
    const auto func = RawContext::RunScript(LR"=(
    (function() {
        let logging = Symbol('LoggingCallback');
        let getStack = Symbol('GetStack');
        class Console
        {
            constructor(console, callback)
            {
                Object.defineProperty(this, ')=" RT_EXT_CONSOLE_NAME LR"=(', { get() { return console }, configurable: false, enumerable: false });
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
)=", JS_SOURCE_CONTEXT_NONE, __FILEW__);
    auto console = func.New(jsCon, callback);
    RawValue::GlobalObject()[L"console"] = console;
    return con;
}
