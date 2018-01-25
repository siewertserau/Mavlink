#ifndef _TYPE_CONVERSIONS_H_
#define _TYPE_CONVERSIONS_H_

#include <inttypes.h>

uint32_t to_u32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
int32_t to_i32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
uint16_t to_u16(uint8_t b0, uint8_t b1);
int16_t to_i16(uint8_t b0, uint8_t b1);

#endif
