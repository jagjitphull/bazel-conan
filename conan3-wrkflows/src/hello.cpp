#include "hello.h"
#ifdef __has_include
#  if __has_include(<fmt/core.h>)
#    include <fmt/core.h>
#    define HAVE_FMT 1
#  endif
#endif

namespace hello {
  std::string say_hello() {
#ifdef HAVE_FMT
    return fmt::format("hello from {}!", "hello/1.0.0");
#else
    return "hello from hello/1.0.0";
#endif
  }
}

