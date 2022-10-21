#- base64 encode -#
b=bytes()
assert(b.tob64() == '')
b=bytes('00')
assert(b.tob64() == 'AA==')
b=bytes('1122334455')
assert(b.tob64() == 'ESIzRFU=')

#- base64 decode -#
b=bytes().fromb64('')
assert(str(b) == str(bytes('')))
b=bytes().fromb64('AA==')
assert(str(b) == str(bytes('00')))
b=bytes().fromb64('ESIzRFU=')
assert(str(b) == str(bytes('1122334455')))
