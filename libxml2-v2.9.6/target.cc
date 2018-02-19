// Copyright 2016 Google Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <string>

#include "libxml/parser.h"
#include "libxml/xmlsave.h"

#ifdef TRACKPF
#include "trackpf.h"
size_t pfcount = 0;
size_t tcount = 0;
#endif

void ignore (void * ctx, const char * msg, ...) {}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  xmlSetGenericErrorFunc(NULL, &ignore);

  // Test default empty options value and some random combination.
  std::string data_string(reinterpret_cast<const char*>(data), size);
  const std::size_t data_hash = std::hash<std::string>()(data_string);
  const int max_option_value = std::numeric_limits<int>::max();
  const int random_option_value = data_hash % max_option_value;
  const int options[] = {0, random_option_value};

  for (const auto option_value : options) {
    if (auto doc = xmlReadMemory(data_string.c_str(), data_string.length(),
                                 "noname.xml", NULL, option_value)) {
      auto buf = xmlBufferCreate();
      assert(buf);
      auto ctxt = xmlSaveToBuffer(buf, NULL, 0);
      xmlSaveDoc(ctxt, doc);
      xmlSaveClose(ctxt);
      xmlFreeDoc(doc);
      xmlBufferFree(buf);
    }
#ifdef TRACKPF
    else {
	pfcount++;
    }
    tcount++;
#endif
  }

  return 0;
}
