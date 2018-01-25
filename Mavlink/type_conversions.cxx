#include "type_conversions.hxx"

uint32_t to_u32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
	return ((uint32_t)b3 << 24 | (uint32_t)b2 << 16 | (uint32_t)b1 << 8 | (uint32_t)b0);
}

int32_t to_i32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
	return ((int32_t)b3 << 24 | (int32_t)b2 << 16 | (int32_t)b1 << 8 | (int32_t)b0);
}

uint16_t to_u16(uint8_t b0, uint8_t b1) {
	return ((uint16_t)b1 << 8 | (uint16_t)b0);
}

int16_t to_i16(uint8_t b0, uint8_t b1) {
	return ((int16_t)b1 << 8 | (int16_t)b0);
}
