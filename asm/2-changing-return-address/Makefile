all: change_return_address_in_cpp smash_return_address

change_return_address_in_cpp:
	g++ overwrite_return_address_hard_coded_x86_asm.cpp -o change_return_address_in_cpp

smash_return_address:
  # needs to be compiled with no stack canary:
	g++ overwrite_return_address_from_user_x86_asm.cpp -fno-stack-protector -o smash_return_address

