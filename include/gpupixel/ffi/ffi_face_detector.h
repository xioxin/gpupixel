#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Create FaceDetector instance, return pointer (intptr_t for Dart FFI convenience)
intptr_t gpupixel_face_detector_create(void);

// Destroy FaceDetector instance
void gpupixel_face_detector_destroy(intptr_t detector_ptr);

// Detection interface, return float* result and length (output via out_len)
float* gpupixel_face_detector_detect(intptr_t detector_ptr,
									 const uint8_t* data,
									 int width,
									 int height,
									 int stride,
									 int format,
									 int frameType,
									 int* out_len);

#ifdef __cplusplus
}
#endif
