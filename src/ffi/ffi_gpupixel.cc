#include <stdint.h>
#include <cstring>
#include <climits>
#include "libyuv/convert.h"
#include "libyuv/convert_argb.h"
#include "libyuv/planar_functions.h"
#include "libyuv/rotate.h"
#include "utils/util.h"

extern "C" {

// Convert YUV420 to RGBA, output to rgba_out, ensure rgba_out has enough space (width*height*4 bytes)
void gpupixel_yuv420_to_rgba(const uint8_t* y, const uint8_t* u, const uint8_t* v,
                             int width, int height,
                             int y_row_stride, int u_row_stride, int v_row_stride,
                             int y_pixel_stride, int u_pixel_stride, int v_pixel_stride,
                             uint8_t* rgba_out) {
  if (!y || !u || !v || !rgba_out) return;
  
  // If pixel stride is 1, use standard conversion
  if (y_pixel_stride == 1 && u_pixel_stride == 1 && v_pixel_stride == 1) {
    // Create temporary NV12/NV21 buffer (Y plane and interleaved UV plane)
    uint8_t* nv21_data = new uint8_t[width * height * 3 / 2];
    if (!nv21_data) {
      return;
    }

    // Copy Y plane
    for (int i = 0; i < height; i++) {
      memcpy(nv21_data + i * width, y + i * y_row_stride, width);
    }

    // Build NV21 format (VU interleaved) UV plane
    uint8_t* nv21_vu_data = nv21_data + width * height;
    int uv_height = height / 2;
    int uv_width = width / 2;

    for (int i = 0; i < uv_height; i++) {
      for (int j = 0; j < uv_width; j++) {
        int offset = i * u_row_stride + j;
        nv21_vu_data[i * width + j * 2] = v[offset];      // V
        nv21_vu_data[i * width + j * 2 + 1] = u[offset];  // U
      }
    }

    // Use libyuv to convert NV21 to RGBA
    libyuv::NV21ToABGR(nv21_data,                   // Y
                       width,                       // Y stride
                       nv21_data + width * height,  // VU
                       width,                       // VU stride
                       rgba_out,                    // RGBA
                       width * 4,                   // RGBA stride
                       width, height);

    delete[] nv21_data;
  } else {
    // Non-standard pixel stride, handle manually
    // Create temporary buffers
    uint8_t* y_plane = new uint8_t[width * height];
    uint8_t* u_plane = new uint8_t[width * height / 4];
    uint8_t* v_plane = new uint8_t[width * height / 4];

    if (!y_plane || !u_plane || !v_plane) {
      delete[] y_plane;
      delete[] u_plane;
      delete[] v_plane;
      return;
    }

    // Extract contiguous plane data from stride data
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        y_plane[i * width + j] = y[i * y_row_stride + j * y_pixel_stride];
      }
    }

    int uv_height = height / 2;
    int uv_width = width / 2;

    for (int i = 0; i < uv_height; i++) {
      for (int j = 0; j < uv_width; j++) {
        u_plane[i * uv_width + j] = u[i * u_row_stride + j * u_pixel_stride];
        v_plane[i * uv_width + j] = v[i * v_row_stride + j * v_pixel_stride];
      }
    }

    // Use libyuv to convert I420(YUV420) to RGBA
    libyuv::I420ToABGR(y_plane,              // Y
                       width,                // Y stride
                       u_plane,              // U
                       uv_width,             // U stride
                       v_plane,              // V
                       uv_width,             // V stride
                       rgba_out,             // RGBA
                       width * 4,            // RGBA stride
                       width, height);

    delete[] y_plane;
    delete[] u_plane;
    delete[] v_plane;
  }
}

// Rotate RGBA format image
void gpupixel_rotate_rgba(const uint8_t* rgba_in, int width, int height,
                         uint8_t* rgba_out, int out_width, int out_height,
                         int rotation_degrees) {
  if (!rgba_in || !rgba_out) return;

  // Handle based on rotation angle
  switch (rotation_degrees) {
    case 0:  // No rotation
      memcpy(rgba_out, rgba_in, width * height * 4);
      break;

    case 90:  // Clockwise 90 degrees
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          // Source pixel position in RGBA array
          int srcIdx = (y * width + x) * 4;

          // Target pixel position: (height - 1 - y, x) -> (x, height - 1 - y)
          // After 90° rotation, x coordinate becomes row(y), y coordinate becomes column(x)
          int dstX = height - 1 - y;
          int dstY = x;
          int dstIdx = (dstY * out_width + dstX) * 4;

          // Copy all four RGBA channels
          rgba_out[dstIdx] = rgba_in[srcIdx];          // R
          rgba_out[dstIdx + 1] = rgba_in[srcIdx + 1];  // G
          rgba_out[dstIdx + 2] = rgba_in[srcIdx + 2];  // B
          rgba_out[dstIdx + 3] = rgba_in[srcIdx + 3];  // A
        }
      }
      break;

    case 180:  // 180 degrees rotation
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          // Source pixel position in RGBA array
          int srcIdx = (y * width + x) * 4;

          // Target pixel position: (width - 1 - x, height - 1 - y)
          int dstX = width - 1 - x;
          int dstY = height - 1 - y;
          int dstIdx = (dstY * out_width + dstX) * 4;

          // Copy all four RGBA channels
          rgba_out[dstIdx] = rgba_in[srcIdx];          // R
          rgba_out[dstIdx + 1] = rgba_in[srcIdx + 1];  // G
          rgba_out[dstIdx + 2] = rgba_in[srcIdx + 2];  // B
          rgba_out[dstIdx + 3] = rgba_in[srcIdx + 3];  // A
        }
      }
      break;

    case 270:  // Clockwise 270 degrees (counterclockwise 90 degrees)
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          // Source pixel position in RGBA array
          int srcIdx = (y * width + x) * 4;

          // Target pixel position: (y, width - 1 - x)
          int dstX = y;
          int dstY = width - 1 - x;
          int dstIdx = (dstY * out_width + dstX) * 4;

          // Copy all four RGBA channels
          rgba_out[dstIdx] = rgba_in[srcIdx];          // R
          rgba_out[dstIdx + 1] = rgba_in[srcIdx + 1];  // G
          rgba_out[dstIdx + 2] = rgba_in[srcIdx + 2];  // B
          rgba_out[dstIdx + 3] = rgba_in[srcIdx + 3];  // A
        }
      }
      break;

    default:
      // Unsupported rotation angle, copy directly
      memcpy(rgba_out, rgba_in, width * height * 4);
      break;
  }
}


// direction: 0 = horizontal, 1 = vertical, 2 = both
void gpupixel_flip_rgba(uint8_t* rgba_in, int width, int height, int direction) {
    if (!rgba_in) return;

    int row_bytes = width * 4; // Bytes per row

    if (direction == 0 || direction == 2) { // Horizontal flip
        for (int y = 0; y < height; y++) {
        const uint8_t* row_start = rgba_in + y * row_bytes;
        for (int x = 0; x < width / 2; x++) {
            int left_idx = x * 4;
            int right_idx = (width - 1 - x) * 4;
            // Swap left and right pixels
            for (int c = 0; c < 4; c++) {
            std::swap(const_cast<uint8_t&>(row_start[left_idx + c]),
                        const_cast<uint8_t&>(row_start[right_idx + c]));
            }
        }
        }
    }

    if (direction == 1 || direction == 2) { // Vertical flip
        for (int y = 0; y < height / 2; y++) {
        const uint8_t* top_row = rgba_in + y * row_bytes;
        const uint8_t* bottom_row = rgba_in + (height - 1 - y) * row_bytes;
        // Swap top and bottom rows
        for (int x = 0; x < row_bytes; x++) {
            std::swap(const_cast<uint8_t&>(top_row[x]),
                    const_cast<uint8_t&>(bottom_row[x]));
        }
        }
    }
}


// Set resource directory path
void gpupixel_set_resource_path(const char* path) {
  if (!path) return;
  
  // Call C++ method to set resource root directory
  gpupixel::Util::SetResourcePath(fs::path(path).string());
}

} // extern "C"
