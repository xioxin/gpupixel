#include <stdint.h>
#include <memory>
#include "gpupixel/source/source_raw_data.h"

using namespace gpupixel;

extern "C" {

// Create SourceRawData
intptr_t gpupixel_source_raw_data_create() {
  auto source_raw_data = SourceRawData::Create();
  if (!source_raw_data) return 0;
  auto* ptr = new std::shared_ptr<SourceRawData>(source_raw_data);
  return reinterpret_cast<intptr_t>(ptr);
}

// Destroy SourceRawData
void gpupixel_source_raw_data_destroy(intptr_t raw_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceRawData>*>(raw_ptr);
  delete ptr;
}

// Release framebuffer
void gpupixel_source_raw_data_finalize(intptr_t raw_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceRawData>*>(raw_ptr);
  if (ptr && *ptr) {
    (*ptr)->ReleaseFramebuffer(false);
  }
}

// Process data (example only, specific parameters and types need to be adjusted according to actual interface)
void gpupixel_source_raw_data_process(intptr_t raw_ptr, const uint8_t* data, int width, int height, int stride, int type) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceRawData>*>(raw_ptr);
  if (ptr && *ptr) {
    // Assume there's a ProcessData method here, parameter types must match C++ implementation
    (*ptr)->ProcessData(data, width, height, stride, (GPUPIXEL_FRAME_TYPE)type);
  }
}

// Set rotation mode
void gpupixel_source_raw_data_set_rotation(intptr_t raw_ptr, int rotation) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceRawData>*>(raw_ptr);
  if (ptr && *ptr) {
    (*ptr)->SetRotation((RotationMode)rotation);
  }
}

} // extern "C"
