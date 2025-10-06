#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Create Filter, pass in filter class name
intptr_t gpupixel_filter_create(const char* filter_class_name);

// Destroy Filter
void gpupixel_filter_destroy(intptr_t filter_ptr);

// Release Filter resources
void gpupixel_filter_finalize(intptr_t filter_ptr);

// Set float property
void gpupixel_filter_set_float_property(intptr_t filter_ptr, const char* key, float value);

// Get float property
float gpupixel_filter_get_float_property(intptr_t filter_ptr, const char* key);

// Set int property
void gpupixel_filter_set_int_property(intptr_t filter_ptr, const char* key, int value);

// Get int property
int gpupixel_filter_get_int_property(intptr_t filter_ptr, const char* key);

// Set string property
void gpupixel_filter_set_string_property(intptr_t filter_ptr, const char* key, const char* value);

// Get string property - caller is responsible for releasing the returned string
char* gpupixel_filter_get_string_property(intptr_t filter_ptr, const char* key);

// Set face landmarks
void gpupixel_filter_set_face_landmarks(intptr_t filter_ptr,
                                      const float* landmarks, int count);

#ifdef __cplusplus
}
#endif
