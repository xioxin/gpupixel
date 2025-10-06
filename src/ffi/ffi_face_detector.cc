#include <stdint.h>
#include <memory>
#include <vector>
#include <list>
#include <cstring>
#include "gpupixel/face_detector/face_detector.h"

using namespace gpupixel;

// Global detector list for lifecycle management
static std::list<std::shared_ptr<FaceDetector>> face_detector_list_;

extern "C" {

// Create FaceDetector instance, return pointer (intptr_t for Dart FFI convenience)
intptr_t gpupixel_face_detector_create() {
	auto detector = FaceDetector::Create();
	face_detector_list_.push_back(detector);
	return reinterpret_cast<intptr_t>(detector.get());
}

// Destroy FaceDetector instance
void gpupixel_face_detector_destroy(intptr_t detector_ptr) {
	for (auto it = face_detector_list_.begin(); it != face_detector_list_.end(); ++it) {
		if (reinterpret_cast<intptr_t>(it->get()) == detector_ptr) {
			face_detector_list_.erase(it);
			break;
		}
	}
}

// Detection interface, return float* result and length (output via out_len)
float* gpupixel_face_detector_detect(intptr_t detector_ptr,
																		 const uint8_t* data,
																		 int width,
																		 int height,
																		 int stride,
																		 int format,
																		 int frameType,
																		 int* out_len) {
	if (!detector_ptr || !data || !out_len) return nullptr;
	auto detector = reinterpret_cast<FaceDetector*>(detector_ptr);
	std::vector<float> landmarks = detector->Detect(data, width, height, stride,
																									(GPUPIXEL_MODE_FMT)format,
																									(GPUPIXEL_FRAME_TYPE)frameType);
	*out_len = static_cast<int>(landmarks.size());
	if (landmarks.empty()) return nullptr;
	float* result = new float[landmarks.size()];
	std::memcpy(result, landmarks.data(), landmarks.size() * sizeof(float));
	return result;
}


} // extern "C"
