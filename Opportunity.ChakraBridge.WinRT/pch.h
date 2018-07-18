#pragma once

#define NOMINMAX
#include <collection.h>
#include <ppltasks.h>
#include "JsEnum.h"
#include "Value\Declare.h"
#include "Native\ThrowHelper.h"

unsigned char* GetPointerOfBuffer(Windows::Storage::Streams::IBuffer^ buffer, unsigned int *length);