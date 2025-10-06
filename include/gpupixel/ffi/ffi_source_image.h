#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Create SourceImage from file
intptr_t gpupixel_source_image_create_from_file(const char* file_path);

// Create SourceImage from buffer
intptr_t gpupixel_source_image_create_from_buffer(int width, int height, int channel_count, const uint8_t* data);

// Destroy SourceImage
void gpupixel_source_image_destroy(intptr_t image_ptr);

// Get image width
int gpupixel_source_image_get_width(intptr_t image_ptr);

// Get image height
int gpupixel_source_image_get_height(intptr_t image_ptr);

// Get RGBA image data buffer
const uint8_t* gpupixel_source_image_get_rgba_buffer(intptr_t image_ptr);

// Execute render
void gpupixel_source_image_render(intptr_t image_ptr);

// Release SourceImage framebuffer
void gpupixel_source_image_finalize(intptr_t image_ptr);

#ifdef __cplusplus
}
#endif
