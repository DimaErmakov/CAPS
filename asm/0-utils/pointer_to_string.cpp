#include <iostream>

void ptr_to_str(const char * hex_str) {
  union ptr_str {
    void* ptr;
    unsigned long long ptr_as_int;
    // 8 bytes + null
    char str[9];
  };

  ptr_str ps;
  // convert hex to decimal:
  ps.ptr_as_int = strtol(hex_str, NULL, 16);

  // do not output newline:
  std::cout << ps.str;
}

int main(int argc, char**argv) {
  const char*ptr_address_hex = argv[1];
  
  if(argc >= 2)
    ptr_to_str(ptr_address_hex);
}
