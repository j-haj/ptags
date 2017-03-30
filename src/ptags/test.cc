#include <iostream>

#include "ptags.h"

int main() {
  ptags::Ptag::InitPTags();
  PTAG_START();
  std::cout << "Test...\n";
  PTAG_STOP();
  return 0;
}
