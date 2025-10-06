#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Create SourceRawData
intptr_t gpupixel_source_raw_data_create(void);

// Destroy SourceRawData
void gpupixel_source_raw_data_destroy(intptr_t raw_ptr);

// Release framebuffer
void gpupixel_source_raw_data_finalize(intptr_t raw_ptr);

// Process data (parameter types must match C++ implementation)
void gpupixel_source_raw_data_process(intptr_t raw_ptr, const uint8_t* data, int width, int height, int stride, int type);

// Set rotation mode
void gpupixel_source_raw_data_set_rotation(intptr_t raw_ptr, int rotation);

#ifdef __cplusplus
}
#endif
