# ptags

``ptags`` is a single header framework for adding performance tracking tags to
your source code. Using the tags ``PTAG_START()`` and ``PTAG_STOP()`` in your
code will log the start and stop times, the calling function name, and the
elapsed system time between the two tags.

## Quickstart
To add ``ptags`` to your project, simply include the header ``ptags.h`` and add
add a call to ``ptags::Ptag::InitPtags()``

### Example main.cc
```
#include <chrono>
#include <thread>
#include <iostream>

#include "ptags.h"

int main(int argc, char* argv[]) {
  ptags::Ptag::InitPTags();

  PTAG_START();

  std::this_thread::sleep_for(std::chrono::seconds(2));

  PTAG_STOP();

  return 0;
}
```
