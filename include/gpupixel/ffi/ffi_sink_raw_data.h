#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Create SinkRawData
intptr_t gpupixel_sink_raw_data_create(void);

// Destroy SinkRawData
void gpupixel_sink_raw_data_destroy(intptr_t sink_ptr);

// Resource cleanup (can be extended for special cleanup requirements)
void gpupixel_sink_raw_data_finalize(intptr_t sink_ptr);

// Get width
int gpupixel_sink_raw_data_get_width(intptr_t sink_ptr);

// Get height
int gpupixel_sink_raw_data_get_height(intptr_t sink_ptr);

// Get RGBA buffer data - returns buffer size, data output via out_buffer, caller is responsible for releasing
int gpupixel_sink_raw_data_get_rgba_buffer(intptr_t sink_ptr, uint8_t** out_buffer);

// Get I420 buffer data - returns buffer size, data output via out_buffer, caller is responsible for releasing
int gpupixel_sink_raw_data_get_i420_buffer(intptr_t sink_ptr, uint8_t** out_buffer);

// Release buffer data
void gpupixel_sink_raw_data_free_buffer(uint8_t* buffer);

#ifdef __cplusplus
}
#endif
