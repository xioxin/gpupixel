#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#include "gpupixel/filter/filter.h"
#include "gpupixel/filter/face_reshape_filter.h"
#include "gpupixel/filter/lipstick_filter.h"
#include "gpupixel/filter/blusher_filter.h"

using namespace gpupixel;

extern "C" {

// Create Filter, pass in filter class name
intptr_t gpupixel_filter_create(const char* filter_class_name) {
  auto filter = Filter::Create(filter_class_name);
  if (!filter) return 0;
  auto* ptr = new std::shared_ptr<Filter>(filter);
  return reinterpret_cast<intptr_t>(ptr);
}

// Destroy Filter
void gpupixel_filter_destroy(intptr_t filter_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  delete ptr;
}

// Release Filter resources
void gpupixel_filter_finalize(intptr_t filter_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    (*ptr)->ReleaseFramebuffer(false);
  }
}

// Set float property
void gpupixel_filter_set_float_property(intptr_t filter_ptr, const char* key, float value) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    (*ptr)->SetProperty(key, value);
  }
}

// Get float property
float gpupixel_filter_get_float_property(intptr_t filter_ptr, const char* key) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    float value = 0.0f;
    if ((*ptr)->GetProperty(key, value)) {
      return value;
    }
  }
  return 0.0f;
}

// Set int property
void gpupixel_filter_set_int_property(intptr_t filter_ptr, const char* key, int value) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    (*ptr)->SetProperty(key, value);
  }
}

// Get int property
int gpupixel_filter_get_int_property(intptr_t filter_ptr, const char* key) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    int value = 0;
    if ((*ptr)->GetProperty(key, value)) {
      return value;
    }
  }
  return 0;
}

// Set string property
void gpupixel_filter_set_string_property(intptr_t filter_ptr, const char* key, const char* value) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr && value) {
    (*ptr)->SetProperty(key, value);
  }
}

// Get string property - caller is responsible for releasing the returned string
char* gpupixel_filter_get_string_property(intptr_t filter_ptr, const char* key) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    std::string value;
    if ((*ptr)->GetProperty(key, value)) {
      char* result = new char[value.length() + 1];
      std::strcpy(result, value.c_str());
      return result;
    }
  }
  return nullptr;
}

// Release string returned by get_string_property
void gpupixel_filter_free_string(char* str) {
  delete[] str;
}

// Set float array property
void gpupixel_filter_set_float_array_property(intptr_t filter_ptr,
                                              const char* key,
                                              const float* values, int count) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    (*ptr)->SetProperty(key, std::vector<float>(values, values + count));
  }
}

// Set face landmarks
void gpupixel_filter_set_face_landmarks(intptr_t filter_ptr,
                                      const float* landmarks, int count) {
  auto* ptr = reinterpret_cast<std::shared_ptr<Filter>*>(filter_ptr);
  if (ptr && *ptr) {
    std::vector<float> landmarks_vec(landmarks, landmarks + count);

    // Try to cast to FaceReshapeFilter
    auto face_reshape_filter =
        std::dynamic_pointer_cast<FaceReshapeFilter>(*ptr);
    if (face_reshape_filter) {
      face_reshape_filter->SetFaceLandmarks(landmarks_vec);
      return;
    }

    // Try to cast to LipstickFilter
    auto lipstick_filter = std::dynamic_pointer_cast<LipstickFilter>(*ptr);
    if (lipstick_filter) {
      lipstick_filter->SetFaceLandmarks(landmarks_vec);
      return;
    }

    // Try to cast to BlusherFilter
    auto blusher_filter = std::dynamic_pointer_cast<BlusherFilter>(*ptr);
    if (blusher_filter) {
      blusher_filter->SetFaceLandmarks(landmarks_vec);
      return;
    }
  }
}

}  // extern "C"
