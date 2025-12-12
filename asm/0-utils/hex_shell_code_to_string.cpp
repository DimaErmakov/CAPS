#include <iostream>
#include <cstring>

int hex_to_dec(char c) {
  if (c >= '0' && c <= '9')
    return c-'0';
  if (c >= 'a' && c <= 'f')
    return 10 + c-'a';
  if (c >= 'A' && c <= 'F')
    return 10 + c-'A';
  return 0;
}

void print_as_str(const char*hex_str, int len) {
  // +1 for null char:
  char* buff = new char[len/2 + 1];

  int i_buff=0;
  for (int i_hex=0; i_hex<len; i_hex+=2, ++i_buff) {
    unsigned char c = hex_to_dec(hex_str[i_hex])*16 + hex_to_dec(hex_str[i_hex+1]);
    buff[i_buff] = c;
  }
  // null terminator:
  buff[i_buff] = '\0';

  // no newline
  std::cout << buff;
}

int main(int argc, const char**argv) {
  if (argc == 2) {
    const char*hex_str = argv[1];
    int n = strlen(hex_str);
    if (n % 2 != 0) {
      std::cerr << "error: hex string must be made ofan even number of digits since one char is 2 hex digits" << std::endl;
      return 1;
    }
    print_as_str(hex_str, n);
  }
  else
    std::cerr << "example usage: hex_shell2str 48c7..." << std::endl;
  return 0;
}
