#include "FFT.h"

// FFTLEN=1024
LINKERSECTION(".extra")
/*static*/ const ui16 CFftTables::arrHannWindow[512] = {
    0x0000, 0x0000, 0x0002, 0x0005, 0x0009, 0x000f, 0x0016, 0x001e, 0x0027,
    0x0032, 0x003d, 0x004a, 0x0058, 0x0068, 0x0079, 0x008a, 0x009e, 0x00b2,
    0x00c8, 0x00de, 0x00f6, 0x0110, 0x012a, 0x0146, 0x0163, 0x0181, 0x01a0,
    0x01c1, 0x01e3, 0x0206, 0x022a, 0x0250, 0x0276, 0x029e, 0x02c7, 0x02f2,
    0x031d, 0x034a, 0x0378, 0x03a7, 0x03d7, 0x0409, 0x043c, 0x0470, 0x04a5,
    0x04db, 0x0513, 0x054b, 0x0585, 0x05c0, 0x05fd, 0x063a, 0x0679, 0x06b8,
    0x06f9, 0x073b, 0x077f, 0x07c3, 0x0809, 0x084f, 0x0897, 0x08e0, 0x092b,
    0x0976, 0x09c3, 0x0a10, 0x0a5f, 0x0aaf, 0x0b00, 0x0b52, 0x0ba6, 0x0bfa,
    0x0c50, 0x0ca6, 0x0cfe, 0x0d57, 0x0db1, 0x0e0c, 0x0e68, 0x0ec6, 0x0f24,
    0x0f84, 0x0fe4, 0x1046, 0x10a9, 0x110c, 0x1171, 0x11d7, 0x123e, 0x12a6,
    0x1310, 0x137a, 0x13e5, 0x1451, 0x14bf, 0x152d, 0x159c, 0x160d, 0x167e,
    0x16f1, 0x1764, 0x17d9, 0x184e, 0x18c5, 0x193c, 0x19b5, 0x1a2e, 0x1aa8,
    0x1b24, 0x1ba0, 0x1c1e, 0x1c9c, 0x1d1b, 0x1d9c, 0x1e1d, 0x1e9f, 0x1f22,
    0x1fa6, 0x202b, 0x20b1, 0x2138, 0x21bf, 0x2248, 0x22d2, 0x235c, 0x23e7,
    0x2474, 0x2501, 0x258f, 0x261e, 0x26ad, 0x273e, 0x27cf, 0x2862, 0x28f5,
    0x2989, 0x2a1e, 0x2ab3, 0x2b4a, 0x2be1, 0x2c79, 0x2d12, 0x2dac, 0x2e46,
    0x2ee1, 0x2f7d, 0x301a, 0x30b8, 0x3156, 0x31f5, 0x3295, 0x3336, 0x33d7,
    0x347a, 0x351c, 0x35c0, 0x3664, 0x3709, 0x37af, 0x3855, 0x38fd, 0x39a4,
    0x3a4d, 0x3af6, 0x3ba0, 0x3c4a, 0x3cf5, 0x3da1, 0x3e4d, 0x3efb, 0x3fa8,
    0x4056, 0x4105, 0x41b5, 0x4265, 0x4316, 0x43c7, 0x4479, 0x452b, 0x45de,
    0x4692, 0x4746, 0x47fb, 0x48b0, 0x4966, 0x4a1c, 0x4ad3, 0x4b8a, 0x4c42,
    0x4cfa, 0x4db3, 0x4e6d, 0x4f26, 0x4fe1, 0x509b, 0x5156, 0x5212, 0x52ce,
    0x538b, 0x5448, 0x5505, 0x55c3, 0x5681, 0x5740, 0x57ff, 0x58be, 0x597e,
    0x5a3e, 0x5afe, 0x5bbf, 0x5c80, 0x5d42, 0x5e04, 0x5ec6, 0x5f88, 0x604b,
    0x610e, 0x61d2, 0x6295, 0x635a, 0x641e, 0x64e2, 0x65a7, 0x666c, 0x6732,
    0x67f7, 0x68bd, 0x6983, 0x6a49, 0x6b10, 0x6bd6, 0x6c9d, 0x6d64, 0x6e2b,
    0x6ef3, 0x6fba, 0x7082, 0x714a, 0x7212, 0x72da, 0x73a2, 0x746b, 0x7533,
    0x75fc, 0x76c4, 0x778d, 0x7856, 0x791f, 0x79e8, 0x7ab1, 0x7b7a, 0x7c43,
    0x7d0c, 0x7dd6, 0x7e9f, 0x7f68, 0x8031, 0x80fb, 0x81c4, 0x828d, 0x8356,
    0x841f, 0x84e9, 0x85b2, 0x867b, 0x8744, 0x880d, 0x88d5, 0x899e, 0x8a67,
    0x8b2f, 0x8bf8, 0x8cc0, 0x8d88, 0x8e50, 0x8f18, 0x8fe0, 0x90a8, 0x916f,
    0x9236, 0x92fd, 0x93c4, 0x948b, 0x9552, 0x9618, 0x96de, 0x97a4, 0x986a,
    0x992f, 0x99f4, 0x9ab9, 0x9b7e, 0x9c42, 0x9d07, 0x9dca, 0x9e8e, 0x9f51,
    0xa014, 0xa0d7, 0xa199, 0xa25b, 0xa31d, 0xa3de, 0xa49f, 0xa560, 0xa620,
    0xa6e0, 0xa7a0, 0xa85f, 0xa91e, 0xa9dc, 0xaa9a, 0xab58, 0xac15, 0xacd2,
    0xad8e, 0xae4a, 0xaf05, 0xafc0, 0xb07b, 0xb135, 0xb1ee, 0xb2a7, 0xb360,
    0xb418, 0xb4d0, 0xb587, 0xb63d, 0xb6f3, 0xb7a9, 0xb85e, 0xb912, 0xb9c6,
    0xba79, 0xbb2c, 0xbbde, 0xbc90, 0xbd41, 0xbdf1, 0xbea1, 0xbf50, 0xbfff,
    0xc0ad, 0xc15a, 0xc207, 0xc2b3, 0xc35e, 0xc409, 0xc4b3, 0xc55d, 0xc606,
    0xc6ae, 0xc755, 0xc7fc, 0xc8a2, 0xc947, 0xc9ec, 0xca90, 0xcb33, 0xcbd6,
    0xcc77, 0xcd18, 0xcdb9, 0xce58, 0xcef7, 0xcf95, 0xd032, 0xd0cf, 0xd16a,
    0xd205, 0xd29f, 0xd339, 0xd3d1, 0xd469, 0xd500, 0xd596, 0xd62b, 0xd6bf,
    0xd753, 0xd7e6, 0xd878, 0xd909, 0xd999, 0xda28, 0xdab6, 0xdb44, 0xdbd1,
    0xdc5c, 0xdce7, 0xdd71, 0xddfa, 0xde83, 0xdf0a, 0xdf90, 0xe016, 0xe09a,
    0xe11e, 0xe1a0, 0xe222, 0xe2a3, 0xe322, 0xe3a1, 0xe41f, 0xe49c, 0xe518,
    0xe593, 0xe60d, 0xe686, 0xe6fe, 0xe775, 0xe7eb, 0xe860, 0xe8d4, 0xe947,
    0xe9b9, 0xea2a, 0xea9a, 0xeb08, 0xeb76, 0xebe3, 0xec4f, 0xecba, 0xed23,
    0xed8c, 0xedf3, 0xee5a, 0xeebf, 0xef24, 0xef87, 0xefe9, 0xf04a, 0xf0aa,
    0xf109, 0xf167, 0xf1c4, 0xf220, 0xf27a, 0xf2d4, 0xf32c, 0xf383, 0xf3d9,
    0xf42e, 0xf482, 0xf4d5, 0xf527, 0xf577, 0xf5c6, 0xf615, 0xf662, 0xf6ae,
    0xf6f9, 0xf742, 0xf78b, 0xf7d2, 0xf818, 0xf85d, 0xf8a1, 0xf8e4, 0xf925,
    0xf966, 0xf9a5, 0xf9e3, 0xfa20, 0xfa5b, 0xfa96, 0xfacf, 0xfb07, 0xfb3e,
    0xfb74, 0xfba8, 0xfbdc, 0xfc0e, 0xfc3f, 0xfc6f, 0xfc9d, 0xfccb, 0xfcf7,
    0xfd22, 0xfd4b, 0xfd74, 0xfd9b, 0xfdc1, 0xfde6, 0xfe0a, 0xfe2c, 0xfe4d,
    0xfe6d, 0xfe8c, 0xfeaa, 0xfec6, 0xfee1, 0xfefb, 0xff14, 0xff2b, 0xff41,
    0xff56, 0xff6a, 0xff7d, 0xff8e, 0xff9e, 0xffad, 0xffba, 0xffc7, 0xffd2,
    0xffdc, 0xffe4, 0xffec, 0xfff2, 0xfff7, 0xfffb, 0xfffd, 0xfffe};

// Since we only use 3/4 of N_WAVE, we define only
// this many samples, in order to conserve data space.
LINKERSECTION(".extra")
/*static*/ const short CFftTables::Sinewave512[512 - 512 / 4 + 1] = {
    0,      402,    804,    1206,   1608,   2009,   2410,   2811,   3212,
    3612,   4011,   4410,   4808,   5205,   5602,   5998,   6393,   6786,
    7179,   7571,   7962,   8351,   8739,   9126,   9512,   9896,   10278,
    10659,  11039,  11417,  11793,  12167,  12539,  12910,  13279,  13645,
    14010,  14372,  14732,  15090,  15446,  15800,  16151,  16499,  16846,
    17189,  17530,  17869,  18204,  18537,  18868,  19195,  19519,  19841,
    20159,  20475,  20787,  21096,  21403,  21705,  22005,  22301,  22594,
    22884,  23170,  23452,  23731,  24007,  24279,  24547,  24811,  25072,
    25329,  25582,  25832,  26077,  26319,  26556,  26790,  27019,  27245,
    27466,  27683,  27896,  28105,  28310,  28510,  28706,  28898,  29085,
    29268,  29447,  29621,  29791,  29956,  30117,  30273,  30424,  30571,
    30714,  30852,  30985,  31113,  31237,  31356,  31470,  31580,  31685,
    31785,  31880,  31971,  32057,  32137,  32213,  32285,  32351,  32412,
    32469,  32521,  32567,  32609,  32646,  32678,  32705,  32728,  32745,
    32757,  32765,  32767,  32765,  32757,  32745,  32728,  32705,  32678,
    32646,  32609,  32567,  32521,  32469,  32412,  32351,  32285,  32213,
    32137,  32057,  31971,  31880,  31785,  31685,  31580,  31470,  31356,
    31237,  31113,  30985,  30852,  30714,  30571,  30424,  30273,  30117,
    29956,  29791,  29621,  29447,  29268,  29085,  28898,  28706,  28510,
    28310,  28105,  27896,  27683,  27466,  27245,  27019,  26790,  26556,
    26319,  26077,  25832,  25582,  25329,  25072,  24811,  24547,  24279,
    24007,  23731,  23452,  23170,  22884,  22594,  22301,  22005,  21705,
    21403,  21096,  20787,  20475,  20159,  19841,  19519,  19195,  18868,
    18537,  18204,  17869,  17530,  17189,  16846,  16499,  16151,  15800,
    15446,  15090,  14732,  14372,  14010,  13645,  13279,  12910,  12539,
    12167,  11793,  11417,  11039,  10659,  10278,  9896,   9512,   9126,
    8739,   8351,   7962,   7571,   7179,   6786,   6393,   5998,   5602,
    5205,   4808,   4410,   4011,   3612,   3212,   2811,   2410,   2009,
    1608,   1206,   804,    402,    0,      -402,   -804,   -1206,  -1608,
    -2009,  -2410,  -2811,  -3212,  -3612,  -4011,  -4410,  -4808,  -5205,
    -5602,  -5998,  -6393,  -6786,  -7179,  -7571,  -7962,  -8351,  -8739,
    -9126,  -9512,  -9896,  -10278, -10659, -11039, -11417, -11793, -12167,
    -12539, -12910, -13279, -13645, -14010, -14372, -14732, -15090, -15446,
    -15800, -16151, -16499, -16846, -17189, -17530, -17869, -18204, -18537,
    -18868, -19195, -19519, -19841, -20159, -20475, -20787, -21096, -21403,
    -21705, -22005, -22301, -22594, -22884, -23170, -23452, -23731, -24007,
    -24279, -24547, -24811, -25072, -25329, -25582, -25832, -26077, -26319,
    -26556, -26790, -27019, -27245, -27466, -27683, -27896, -28105, -28310,
    -28510, -28706, -28898, -29085, -29268, -29447, -29621, -29791, -29956,
    -30117, -30273, -30424, -30571, -30714, -30852, -30985, -31113, -31237,
    -31356, -31470, -31580, -31685, -31785, -31880, -31971, -32057, -32137,
    -32213, -32285, -32351, -32412, -32469, -32521, -32567, -32609, -32646,
    -32678, -32705, -32728, -32745, -32757, -32765, -32767};

template <>
int CFft<512>::_logbits() {
  return 9;
}

template <>
int CFft<512>::_cos(int a) {
  return Sinewave512[a + 512 / 4];
}

template <>
int CFft<512>::_sin(int a) {
  return Sinewave512[a];
}

template <>
int CFft<1024>::_logbits() {
  return 10;
}

template <>
int CFft<1024>::_cos(int a) {
  if (a & 1)
    return (Sinewave512[a / 2 + 512 / 4] + Sinewave512[a / 2 + 512 / 4 + 1]) /
           2;
  return Sinewave512[a / 2 + 512 / 4];
}

template <>
int CFft<1024>::_sin(int a) {
  if (a & 1) return (Sinewave512[a / 2] + Sinewave512[a / 2 + 1]) / 2;
  return Sinewave512[a / 2];
}

// Credits for fft code goes here:

/************************************************************************

    FFT Audio Analysis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// Fixed-point in-place Fast Fourier Transform
// All data are fixed-point short integers, in which -32768
// to +32768 represent -1.0 to +1.0 respectively. Integer
// arithmetic is used for speed, instead of the more natural
// floating-point.
//
// For the forward FFT (time -> freq), fixed scaling is
// performed to prevent arithmetic overflow, and to map a 0dB
// sine/cosine wave (i.e. amplitude = 32767) to two -6dB freq
// coefficients.
//
// Written by:  Tom Roberts  11/8/89
// Made portable:  Malcolm Slaney 12/15/94 malcolm@interval.com
// Enhanced:  Dimitrios P. Bouras  14 Jun 2006 dbouras@ieee.org
// Ported to PIC18F:  Simon Inns 20110104
// Ported to stm32:  Pedro Sim�es 2012-03-10	pmos69@hotmail.com

/*
  fix_fft() - perform forward fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m
*/
