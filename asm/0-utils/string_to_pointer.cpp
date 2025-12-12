#include <iostream>

void str_to_ptr(const char * cstr) {
  union ptr_str {
    void* ptr;
    // 8 bytes + null
    char str[9];
  };

  ptr_str ps;
  sprintf(ps.str, "%s", cstr);

  std::cout << ps.ptr << std::endl;
}

int main(int argc, char**argv) {
  const char*str_bytes_address_packed = argv[1];

  if(argc >= 2)
    str_to_ptr(str_bytes_address_packed);
}
