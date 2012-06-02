/* Copyright (c) 2012, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include "lzma-util.h"
#include "LzmaDec.h"

using namespace vm;

namespace vm {

uint8_t*
decodeLZMA(System* s, Allocator* a, uint8_t* in, unsigned inSize,
           unsigned* outSize)
{
  const unsigned PropHeaderSize = 5;
  const unsigned HeaderSize = 13;

  int32_t outSize32;
  memcpy(&outSize32, in + PropHeaderSize, 4);
  expect(s, outSize32 >= 0);
  SizeT outSizeT = outSize32;

  uint8_t* out = static_cast<uint8_t*>(a->allocate(outSize32));

  SizeT inSizeT = inSize;
  LzmaAllocator allocator(a);

  ELzmaStatus status;
  int result = LzmaDecode
    (out, &outSizeT, in + HeaderSize, &inSizeT, in, PropHeaderSize,
     LZMA_FINISH_END, &status, &(allocator.allocator));

  expect(s, result == SZ_OK);
  expect(s, status == LZMA_STATUS_FINISHED_WITH_MARK);

  *outSize = outSize32;

  return out;
}

} // namespace vm

