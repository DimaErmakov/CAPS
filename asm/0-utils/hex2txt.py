import sys

def hex_string_to_raw_text(s):
  if sys.version_info.major >= 3:
    raise NotImplementedError('this code is no longer valid after python2 . use ptr2str or hex_shell2str from utils instead')

  result = ''
  for i in range(0, len(s), 2):
    hex_char = s[i:i+2]
    int_char = int(hex_char, 16)
    raw_char = chr(int_char)
    result += raw_char
  return result

def hex_string_to_backslash_form(s):
  result = ''
  for i in range(0, len(s), 2):
    hex_char = s[i:i+2]
    result += '\\x'+hex_char
  return result

def main(argv):
  if len(argv)!=2 or (len(argv)==2 and argv[0] not in ('raw','backslash')):
    print( 'usage: {raw,backslash} <hex codes, e.g. 48c7...>' )
  else:
    mode = argv[0]
    shell_code_hex = argv[1]

    if mode == 'raw':
      # do not print newline:
      sys.stdout.write(hex_string_to_raw_text(shell_code_hex))
    else:
      print( hex_string_to_backslash_form(shell_code_hex) )

if __name__=='__main__':
  main(sys.argv[1:])
