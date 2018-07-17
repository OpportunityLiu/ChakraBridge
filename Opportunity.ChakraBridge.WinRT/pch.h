#pragma once

#define NOMINMAX
#include <collection.h>
#include <ppltasks.h>

unsigned char* GetPointerOfBuffer(Windows::Storage::Streams::IBuffer^ buffer, unsigned int *length);