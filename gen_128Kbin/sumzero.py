# -*- coding: utf-8 -*-
import argparse
import struct

#define
MEM_S  = 128 
MAX_MEM_SIZE = MEM_S*1024

# main
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='hashApp')
    parser.add_argument('input', help='Input file path')
    parser.add_argument('output', help='Output file path')

    args = parser.parse_args()
    filePath = args.input
    filePath2 = args.output

    cnt = 0
    a_data = 0
    b_data = 0

    with open(filePath, 'rb') as f:
     chunk = f.read(MAX_MEM_SIZE)
     #print(chunk)

    with open(filePath2, 'ab') as f:
        f.write(chunk)
    size = len(chunk)

    with open(filePath2, 'rb') as f:
        while True:
            chunk = f.read(2)
            a_data += int.from_bytes(chunk, byteorder='little')
            if len(chunk) == 0:
                break

    a_data = a_data & 0x000000FFFF
    b_data = a_data.to_bytes(2, 'little')
    
    print("{0} : {1}".format(filePath, hex(a_data)))

    f=open(filePath2, mode ='ab')

    for i in range(MAX_MEM_SIZE):
      # chunk size skip
      if(cnt<=size):
       cnt+=1
      elif(cnt==MAX_MEM_SIZE-1):
       f.write(b_data)
       cnt+=2

      else:
        f.write(struct.pack("b", 0))
        cnt+=1
    f.close
