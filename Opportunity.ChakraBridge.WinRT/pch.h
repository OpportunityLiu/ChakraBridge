#pragma once

#define NOMINMAX
#include <collection.h>
#include <ppltasks.h>
#include "JsEnum.h"
#include "Native\ThrowHelper.h"
#include "Value\Declare.h"

unsigned char* GetPointerOfBuffer(Windows::Storage::Streams::IBuffer^ buffer, unsigned int *length);