#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Add Sink to Source
intptr_t gpupixel_source_add_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter);

// Remove Sink from Source
void gpupixel_source_remove_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter);

// Remove all Sinks from Source
void gpupixel_source_remove_all_sinks(intptr_t source_id, int is_source_filter);

// Check if Source has Sink
int gpupixel_source_has_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter);

// Set framebuffer scale
void gpupixel_source_set_framebuffer_scale(intptr_t source_id, float scale, int is_source_filter);

// Get rotated framebuffer width
int gpupixel_source_get_rotated_framebuffer_width(intptr_t source_id, int is_source_filter);

// Get rotated framebuffer height
int gpupixel_source_get_rotated_framebuffer_height(intptr_t source_id, int is_source_filter);

#ifdef __cplusplus
}
#endif
