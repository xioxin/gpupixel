#include <stdint.h>
#include <memory>
#include <climits>
#include <cstring>
#include "gpupixel/sink/sink_raw_data.h"

using namespace gpupixel;

extern "C" {

// Create SinkRawData
intptr_t gpupixel_sink_raw_data_create() {
  auto sink_raw_data = SinkRawData::Create();
  if (!sink_raw_data) return 0;
  auto* ptr = new std::shared_ptr<SinkRawData>(sink_raw_data);
  return reinterpret_cast<intptr_t>(ptr);
}

// Destroy SinkRawData
void gpupixel_sink_raw_data_destroy(intptr_t sink_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  delete ptr;
}

// Resource cleanup (can be extended for special cleanup requirements)
void gpupixel_sink_raw_data_finalize(intptr_t sink_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  if (ptr && *ptr) {
    // Extensible special cleanup logic
  }
}

// Get width
int gpupixel_sink_raw_data_get_width(intptr_t sink_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  return ptr && *ptr ? (*ptr)->GetWidth() : 0;
}

// Get height
int gpupixel_sink_raw_data_get_height(intptr_t sink_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  return ptr && *ptr ? (*ptr)->GetHeight() : 0;
}

// Get RGBA buffer data - returns buffer size, data output via out_buffer, caller is responsible for releasing
int gpupixel_sink_raw_data_get_rgba_buffer(intptr_t sink_ptr, uint8_t** out_buffer) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  if (!ptr || !*ptr || !out_buffer) {
    if (out_buffer) *out_buffer = nullptr;
    return 0;
  }

  const uint8_t* buffer = (*ptr)->GetRgbaBuffer();
  if (!buffer) {
    *out_buffer = nullptr;
    return 0;
  }

  int width = (*ptr)->GetWidth();
  int height = (*ptr)->GetHeight();

  // Check if width and height are valid
  if (width <= 0 || height <= 0) {
    *out_buffer = nullptr;
    return 0;
  }

  // Check for potential overflow
  if (width > INT_MAX / height || width * height > INT_MAX / 4) {
    *out_buffer = nullptr;
    return 0;
  }

  int size = width * height * 4;  // RGBA = 4 bytes per pixel

  *out_buffer = new uint8_t[size];
  if (!*out_buffer) {
    return 0;
  }

  memcpy(*out_buffer, buffer, size);
  return size;
}

// Get I420 buffer data - returns buffer size, data output via out_buffer, caller is responsible for releasing
int gpupixel_sink_raw_data_get_i420_buffer(intptr_t sink_ptr, uint8_t** out_buffer) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SinkRawData>*>(sink_ptr);
  if (!ptr || !*ptr || !out_buffer) {
    if (out_buffer) *out_buffer = nullptr;
    return 0;
  }

  const uint8_t* buffer = (*ptr)->GetI420Buffer();
  if (!buffer) {
    *out_buffer = nullptr;
    return 0;
  }

  int width = (*ptr)->GetWidth();
  int height = (*ptr)->GetHeight();

  // Check if width and height are valid
  if (width <= 0 || height <= 0) {
    *out_buffer = nullptr;
    return 0;
  }

  // Check for potential overflow
  if (width > INT_MAX / height || width * height > INT_MAX / 2) {
    *out_buffer = nullptr;
    return 0;
  }

  int size = width * height * 3 / 2;  // I420 size = width * height * 1.5

  *out_buffer = new uint8_t[size];
  if (!*out_buffer) {
    return 0;
  }

  memcpy(*out_buffer, buffer, size);
  return size;
}

// Release buffer data
void gpupixel_sink_raw_data_free_buffer(uint8_t* buffer) {
  delete[] buffer;
}

} // extern "C"
