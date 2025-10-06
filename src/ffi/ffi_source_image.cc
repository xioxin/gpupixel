#include <stdint.h>
#include <memory>
#include <string>
#include "gpupixel/source/source_image.h"

using namespace gpupixel;

extern "C" {

// Create SourceImage from file
intptr_t gpupixel_source_image_create_from_file(const char* file_path) {
  auto source_image = SourceImage::Create(file_path);
  if (!source_image) return 0;
  auto* ptr = new std::shared_ptr<SourceImage>(source_image);
  return reinterpret_cast<intptr_t>(ptr);
}

// Create SourceImage from buffer
intptr_t gpupixel_source_image_create_from_buffer(int width, int height, int channel_count, const uint8_t* data) {
  auto source_image = SourceImage::CreateFromBuffer(width, height, channel_count, data);
  if (!source_image) return 0;
  auto* ptr = new std::shared_ptr<SourceImage>(source_image);
  return reinterpret_cast<intptr_t>(ptr);
}

// Destroy SourceImage
void gpupixel_source_image_destroy(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  delete ptr;
}

// Get image width
int gpupixel_source_image_get_width(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  return ptr && *ptr ? (*ptr)->GetWidth() : 0;
}

// Get image height
int gpupixel_source_image_get_height(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  return ptr && *ptr ? (*ptr)->GetHeight() : 0;
}

// Get RGBA image data buffer
const uint8_t* gpupixel_source_image_get_rgba_buffer(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  if (!ptr || !*ptr) {
    return nullptr;
  }
  return (*ptr)->GetRgbaImageBuffer();
}

// Execute render
void gpupixel_source_image_render(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  if (ptr && *ptr) {
    (*ptr)->Render();
  }
}

// Release SourceImage framebuffer
void gpupixel_source_image_finalize(intptr_t image_ptr) {
  auto* ptr = reinterpret_cast<std::shared_ptr<SourceImage>*>(image_ptr);
  if (ptr && *ptr) {
    (*ptr)->ReleaseFramebuffer(false);
  }
}

} // extern "C"
