#include <stdint.h>
#include <memory>
#include "gpupixel/source/source.h"
#include "gpupixel/sink/sink.h"
#include "gpupixel/filter/filter.h"

using namespace gpupixel;

extern "C" {

// Add Sink to Source
intptr_t gpupixel_source_add_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter) {
  if (source_id == 0 || sink_id == 0) return 0;

  // Get Source pointer based on is_source_filter parameter
  std::shared_ptr<Source> source;

  if (is_source_filter) {
    // If source is Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (!filter_ptr || !*filter_ptr) return 0;
    source = std::static_pointer_cast<Source>(*filter_ptr);
  } else {
    // If it's regular Source type
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (!source_ptr || !*source_ptr) return 0;
    source = *source_ptr;
  }

  std::shared_ptr<Sink> sink;

  // Handle sink_id based on is_sink_filter parameter
  if (is_sink_filter) {
    // If it's Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(sink_id);
    if (!filter_ptr || !*filter_ptr) return 0;
    sink = std::static_pointer_cast<Sink>(*filter_ptr);
  } else {
    // If it's regular Sink type
    auto* sink_ptr = reinterpret_cast<std::shared_ptr<Sink>*>(sink_id);
    if (!sink_ptr || !*sink_ptr) return 0;
    sink = *sink_ptr;
  }

  // Add connection
  source->AddSink(sink);

  // Return original sink_id
  return sink_id;
}

// Remove Sink from Source
void gpupixel_source_remove_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter) {
  // Get Source pointer based on is_source_filter parameter
  std::shared_ptr<Source> source;

  if (is_source_filter) {
    // If source is Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (!filter_ptr || !*filter_ptr) return;
    source = std::static_pointer_cast<Source>(*filter_ptr);
  } else {
    // If it's regular Source type
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (!source_ptr || !*source_ptr) return;
    source = *source_ptr;
  }

  std::shared_ptr<Sink> sink;

  // Handle sink_id based on is_sink_filter parameter
  if (is_sink_filter) {
    // If it's Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(sink_id);
    if (!filter_ptr || !*filter_ptr) return;
    sink = std::static_pointer_cast<Sink>(*filter_ptr);
  } else {
    // If it's regular Sink type
    auto* sink_ptr = reinterpret_cast<std::shared_ptr<Sink>*>(sink_id);
    if (!sink_ptr || !*sink_ptr) return;
    sink = *sink_ptr;
  }

  // Remove connection
  source->RemoveSink(sink);
}

// Remove all Sinks from Source
void gpupixel_source_remove_all_sinks(intptr_t source_id, int is_source_filter) {
  if (is_source_filter) {
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (filter_ptr && *filter_ptr) {
      auto source = std::static_pointer_cast<Source>(*filter_ptr);
      source->RemoveAllSinks();
    }
  } else {
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (source_ptr && *source_ptr) {
      (*source_ptr)->RemoveAllSinks();
    }
  }
}

// Check if Source has Sink
int gpupixel_source_has_sink(intptr_t source_id, intptr_t sink_id, int is_sink_filter, int is_source_filter) {
  // Get Source pointer based on is_source_filter parameter
  std::shared_ptr<Source> source;

  if (is_source_filter) {
    // If source is Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (!filter_ptr || !*filter_ptr) return 0;
    source = std::static_pointer_cast<Source>(*filter_ptr);
  } else {
    // If it's regular Source type
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (!source_ptr || !*source_ptr) return 0;
    source = *source_ptr;
  }

  // Get Sink pointer based on is_sink_filter parameter
  std::shared_ptr<Sink> sink;

  if (is_sink_filter) {
    // If it's Filter type
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(sink_id);
    if (!filter_ptr || !*filter_ptr) return 0;
    sink = std::static_pointer_cast<Sink>(*filter_ptr);
  } else {
    // If it's regular Sink type
    auto* sink_ptr = reinterpret_cast<std::shared_ptr<Sink>*>(sink_id);
    if (!sink_ptr || !*sink_ptr) return 0;
    sink = *sink_ptr;
  }

  // Check connection
  return source->HasSink(sink) ? 1 : 0;
}

// Set framebuffer scale
void gpupixel_source_set_framebuffer_scale(intptr_t source_id, float scale, int is_source_filter) {
  if (is_source_filter) {
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (filter_ptr && *filter_ptr) {
      auto source = std::static_pointer_cast<Source>(*filter_ptr);
      source->SetFramebufferScale(scale);
    }
  } else {
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (source_ptr && *source_ptr) {
      (*source_ptr)->SetFramebufferScale(scale);
    }
  }
}

// Get rotated framebuffer width
int gpupixel_source_get_rotated_framebuffer_width(intptr_t source_id, int is_source_filter) {
  if (is_source_filter) {
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (filter_ptr && *filter_ptr) {
      auto source = std::static_pointer_cast<Source>(*filter_ptr);
      return source->GetRotatedFramebufferWidth();
    }
  } else {
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (source_ptr && *source_ptr) {
      return (*source_ptr)->GetRotatedFramebufferWidth();
    }
  }
  return 0;
}

// Get rotated framebuffer height
int gpupixel_source_get_rotated_framebuffer_height(intptr_t source_id, int is_source_filter) {
  if (is_source_filter) {
    auto* filter_ptr = reinterpret_cast<std::shared_ptr<Filter>*>(source_id);
    if (filter_ptr && *filter_ptr) {
      auto source = std::static_pointer_cast<Source>(*filter_ptr);
      return source->GetRotatedFramebufferHeight();
    }
  } else {
    auto* source_ptr = reinterpret_cast<std::shared_ptr<Source>*>(source_id);
    if (source_ptr && *source_ptr) {
      return (*source_ptr)->GetRotatedFramebufferHeight();
    }
  }
  return 0;
}

} // extern "C"
