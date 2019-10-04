/*
 * Copyright (c) 2014-2017 Pavel Kalvoda <me@pavelkalvoda.com>
 *
 * libcbor is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "loaders.h"
#include <math.h>

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

uint8_t _cbor_load_uint8(cbor_data source)
{
	return (uint8_t) *source;
}

uint16_t _cbor_load_uint16(const unsigned char *source)
{
#ifdef HAVE_ENDIAN_H
	return be16toh(*(uint16_t *) source);
#else
	#ifdef IS_BIG_ENDIAN
		return *(uint16_t *) source;
	#else
 		return ((uint16_t) *(source + 0) << 8) +
		        (uint8_t) *(source + 1);
	#endif
#endif
}

uint32_t _cbor_load_uint32(const unsigned char *source)
{
#ifdef HAVE_ENDIAN_H
	return be32toh(*(uint32_t *) source);
#else
	#ifdef IS_BIG_ENDIAN
		return *(uint32_t *) source;
	#else
		return ((uint32_t) *(source + 0) << 0x18) +
		       ((uint32_t) *(source + 1) << 0x10) +
		       ((uint16_t) *(source + 2) << 0x08) +
		        (uint8_t) *(source + 3);
	#endif
#endif
}

uint64_t _cbor_load_uint64(const unsigned char *source)
{
#ifdef HAVE_ENDIAN_H
	return be64toh(*(uint64_t *) source);
#else
	#ifdef IS_BIG_ENDIAN
		return *(uint64_t *) source;
	#else
		return ((uint64_t) *(source + 0) << 0x38) +
		       ((uint64_t) *(source + 1) << 0x30) +
		       ((uint64_t) *(source + 2) << 0x28) +
		       ((uint64_t) *(source + 3) << 0x20) +
		       ((uint32_t) *(source + 4) << 0x18) +
		       ((uint32_t) *(source + 5) << 0x10) +
		       ((uint16_t) *(source + 6) << 0x08) +
		        (uint8_t) *(source + 7);
	#endif
#endif
}

/* As per http://tools.ietf.org/html/rfc7049#appendix-D */
float _cbor_decode_half(unsigned char *halfp)
{
	int half = (halfp[0] << 8) + halfp[1];
	int exp = (half >> 10) & 0x1f;
	int mant = half & 0x3ff;
	double val;
	if (exp == 0) val = ldexp(mant, -24);
	else if (exp != 31) val = ldexp(mant + 1024, exp - 25);
	else val = mant == 0 ? INFINITY : NAN;
	return (float) (half & 0x8000 ? -val : val);
}

double _cbor_load_half(cbor_data source)
{
	/* Discard const */
	return _cbor_decode_half((unsigned char *) source);
}

float _cbor_load_float(cbor_data source)
{
	union _cbor_float_helper helper = {.as_uint = _cbor_load_uint32(source)};
	return helper.as_float;
}

double _cbor_load_double(cbor_data source)
{
	union _cbor_double_helper helper = {.as_uint = _cbor_load_uint64(source)};
	return helper.as_double;
}
