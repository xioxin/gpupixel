#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Convert YUV420 to RGBA, output to rgba_out, ensure rgba_out has enough space (width*height*4 bytes)
void gpupixel_yuv420_to_rgba(const uint8_t* y, const uint8_t* u, const uint8_t* v,
							 int width, int height,
							 int y_row_stride, int u_row_stride, int v_row_stride,
							 int y_pixel_stride, int u_pixel_stride, int v_pixel_stride,
							 uint8_t* rgba_out);

// Rotate RGBA format image
void gpupixel_rotate_rgba(const uint8_t* rgba_in, int width, int height,
                         uint8_t* rgba_out, int out_width, int out_height,
                         int rotation_degrees);

void gpupixel_flip_rgba(uint8_t* rgba_in, int width, int height, int direction);

void gpupixel_set_resource_path(const char* path);

#ifdef __cplusplus
}
#endif
