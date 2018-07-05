
(function () {
    let rt = Symbol("RtConsole")
    function Console(console) {
        this[rt] = console;
        Object.preventExtensions(this);
    }
    Console.prototype[rt] = null;
    Console.prototype.assert = function assert(cond, ...data) { if (cond) this[rt].onLogging('assert', data); };
    Console.prototype.error = function error(...data) { this[rt].onLogging('error', data); };
    Console.prototype.debug = function debug(...data) { this[rt].onLogging('debug', data); };
    Console.prototype.log = function log(...data) { this[rt].onLogging('log', data); };
    Console.prototype.info = function info(...data) { this[rt].onLogging('info', data); };
    Console.prototype.warn = function warn(...data) { this[rt].onLogging('warn', data); };
    Console.prototype.exception = Console.prototype.error;
    return Console;
})()

