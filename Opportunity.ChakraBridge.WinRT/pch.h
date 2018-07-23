#pragma once

#define NOMINMAX
#include <collection.h>
#include <ppltasks.h>
#include "JsEnum.h"
#include "Native\Helper.h"
#include "Value\Declare.h"
#include "JsContext\JsContext.h"
#include "JsRuntime\JsRuntime.h"
#include <memory>
#include <type_traits>

unsigned char* GetPointerOfBuffer(Windows::Storage::Streams::IBuffer^ buffer, unsigned int *length = nullptr, unsigned int *capacity = nullptr);

inline bool IsNullOrEmpty(Platform::String^ s)
{
    return s == nullptr || s->IsEmpty();
}