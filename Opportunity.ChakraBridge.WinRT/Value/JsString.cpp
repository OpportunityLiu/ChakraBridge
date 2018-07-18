#include "pch.h"
#include "JsNull.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsNullImpl::JsNullImpl(JsValueRef ref)
    :JsValueImpl(ref) {}
