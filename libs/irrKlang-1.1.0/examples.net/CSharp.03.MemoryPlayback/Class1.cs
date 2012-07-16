// This example will show how to play sounds directly from memory using irrKlang.
// This is useful for embedding sounds directly in executables as well for 
// making irrKlang work together with different APIs like advanced decoders or
// middleware.

using System;
using IrrKlang;

namespace CSharp._03._MemoryPlayback
{
	class Class1
	{
		// the following huge array simply represents the plain sound data we
		// want to play back. It is just the content of a .wav file from the 
		// game 'Hell Troopers'. Usually this sound is somewhere provided by
		// some external software, but to keep it simple we just use this array as memory.

		// test.wav, converted to this array by bin2h tool, available at bin2h.irrlicht3d.org
		private static byte[] SoundDataArray = {
			0x52,0x49,0x46,0x46,0x54,0xf,0x0,0x0,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,0x12,
			0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x40,0x1f,0x0,0x0,0x40,0x1f,0x0,0x0,0x1,0x0,0x8,
			0x0,0x0,0x0,0x66,0x61,0x63,0x74,0x4,0x0,0x0,0x0,0x22,0xf,0x0,0x0,0x64,0x61,0x74,
			0x61,0x22,0xf,0x0,0x0,0x7f,0x80,0x7f,0x7f,0x80,0x80,0x80,0x80,0x80,0x7f,0x80,0x80,0x81,
			0x80,0x80,0x7e,0x7f,0x7f,0x7f,0x7f,0x80,0x7f,0x81,0x80,0x80,0x80,0x80,0x7e,0x7e,0x7e,0x7e,
			0x7e,0x7e,0x80,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7f,0x80,0x80,0x7f,
			0x80,0x7f,0x80,0x81,0x81,0x80,0x81,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,0x83,0x84,
			0x85,0x86,0x87,0x86,0x87,0x87,0x85,0x83,0x71,0x5a,0x4f,0x6e,0x6d,0x81,0x8a,0x8c,0x8f,0x91,
			0x97,0x84,0x76,0x6e,0x5f,0x68,0x77,0x83,0x85,0x89,0x90,0x94,0x95,0x8f,0x7b,0x70,0x6b,0x72,
			0x76,0x80,0x83,0x84,0x8c,0x8f,0x90,0x87,0x7f,0x72,0x6f,0x70,0x77,0x7b,0x7d,0x82,0x84,0x85,
			0x87,0x86,0x7e,0x79,0x76,0x77,0x79,0x7e,0x80,0x7f,0x85,0x87,0x89,0x85,0x80,0x7c,0x7b,0x7b,
			0x7f,0x7f,0x83,0x84,0x88,0x8a,0x88,0x85,0x7d,0x7c,0x7d,0x80,0x83,0x8b,0x94,0x9d,0xa6,0x8d,
			0x31,0x50,0x52,0x73,0x92,0x7e,0x8f,0x81,0xbb,0xb8,0x9f,0x87,0x5b,0x45,0x4c,0x5f,0x80,0x7c,
			0x8a,0x8f,0x97,0xa7,0xa8,0x95,0x6a,0x5b,0x57,0x63,0x78,0x83,0x84,0x87,0x95,0x9a,0x9a,0x92,
			0x81,0x60,0x5f,0x65,0x75,0x7f,0x84,0x83,0x82,0x8e,0x91,0x8c,0x7b,0x73,0x6a,0x70,0x7b,0x86,
			0x88,0x87,0x88,0x87,0x89,0x87,0x80,0x75,0x74,0x75,0x7e,0x84,0x86,0x84,0x84,0x84,0x83,0x83,
			0x7c,0x78,0x7a,0x7e,0x86,0x90,0x93,0x94,0x93,0x9b,0x87,0x5e,0x3c,0x3b,0x3e,0x49,0x98,0x99,
			0x9d,0x9d,0xab,0xa0,0x97,0x8e,0x51,0x3e,0x49,0x58,0x6f,0x99,0xa7,0x9d,0xa0,0xa2,0x95,0x8a,
			0x80,0x5a,0x50,0x5d,0x70,0x7e,0x9b,0x9d,0x94,0x8a,0x86,0x84,0x7b,0x71,0x5f,0x5d,0x69,0x88,
			0x96,0x99,0x91,0x88,0x7e,0x80,0x81,0x80,0x80,0x82,0x87,0x8f,0x9d,0xa2,0xa7,0xad,0xae,0x31,
			0x31,0x47,0x31,0x78,0x87,0xbc,0xab,0xba,0xc1,0xa3,0xb2,0x9d,0x31,0x39,0x31,0x52,0x5f,0x81,
			0xb2,0xa4,0xb7,0xab,0x9a,0x9b,0x80,0x6a,0x37,0x3e,0x51,0x6b,0x98,0xb0,0xad,0xaa,0x95,0x8f,
			0x7c,0x7e,0x6c,0x53,0x53,0x64,0x77,0x91,0xa4,0xa1,0x97,0x7d,0x70,0x70,0x73,0x71,0x76,0x77,
			0x79,0x8b,0x94,0x97,0x90,0x86,0x74,0x6d,0x70,0x79,0x7d,0x86,0x87,0x89,0x92,0x98,0x9c,0x9d,
			0xa0,0xa7,0xb3,0xa2,0x31,0x50,0x31,0x57,0x7f,0xb7,0xc0,0xa1,0xc2,0x9f,0xb3,0x65,0x4a,0x66,
			0x31,0x33,0x31,0xa0,0xa1,0xc6,0xc0,0xb9,0xa0,0xa5,0x7c,0x71,0x52,0x46,0x31,0x42,0x83,0x8e,
			0xc6,0xb7,0xa9,0x78,0x70,0x6d,0x61,0x70,0x6e,0x6d,0x7b,0x82,0x8e,0x96,0x9f,0x93,0x7c,0x7e,
			0x79,0x90,0xa2,0xaa,0xb8,0xbd,0x97,0x36,0x31,0x31,0x31,0x47,0x6b,0xa0,0xaa,0xc6,0xbf,0xad,
			0xc1,0xb0,0x6d,0x6b,0x31,0x31,0x31,0x6e,0x7e,0xc0,0xc5,0xbc,0xbd,0xaf,0x91,0x7e,0x6e,0x45,
			0x36,0x3c,0x54,0x65,0x8f,0xb5,0xbe,0xae,0x97,0x78,0x5a,0x5b,0x5a,0x6e,0x79,0x7e,0x88,0x93,
			0x9f,0xa0,0xa0,0x89,0x79,0x77,0x7b,0x84,0xb0,0xbe,0xc2,0xa0,0x43,0x45,0x31,0x31,0x44,0x69,
			0xa5,0xaf,0xbd,0x9f,0xa5,0xc6,0x89,0x9d,0x42,0x31,0x31,0x31,0x47,0x64,0xba,0xc7,0xbe,0xc1,
			0xa4,0x96,0x8a,0x84,0x63,0x55,0x42,0x36,0x40,0x6b,0x7d,0xae,0xb2,0xb2,0xa7,0x9d,0x95,0x7d,
			0x7b,0x60,0x5f,0x62,0x6b,0x80,0x99,0xc4,0xc3,0xc3,0xb5,0x7b,0x41,0x31,0x31,0x31,0x39,0x84,
			0xa4,0xbb,0xbd,0xbd,0xc6,0xb0,0xaa,0x6f,0x45,0x31,0x31,0x31,0x31,0x86,0xaa,0xc4,0xc6,0xc3,
			0xb6,0xa8,0x85,0x76,0x64,0x3e,0x31,0x31,0x37,0x52,0xa3,0xc3,0xc6,0xc5,0xc1,0xb5,0x85,0x76,
			0x65,0x62,0x65,0x79,0x8b,0xa3,0xb3,0x89,0x87,0x3a,0x31,0x37,0x45,0x60,0x87,0x99,0x92,0x9d,
			0xb0,0x8c,0xa3,0x87,0x7c,0x73,0x4f,0x45,0x41,0x65,0x75,0x9f,0xb2,0xa7,0xaa,0xa0,0x94,0x83,
			0x7b,0x67,0x59,0x5a,0x4f,0x57,0x69,0x91,0xa2,0xbc,0xba,0xaf,0x9d,0x93,0x8a,0x92,0x9d,0xb3,
			0x7f,0x5a,0x31,0x31,0x31,0x31,0x60,0xb2,0xc2,0xc4,0xb6,0xbc,0x7b,0x89,0x92,0x6b,0x6a,0x58,
			0x33,0x31,0x4d,0x6e,0x84,0xc9,0xc4,0xc4,0xb2,0x98,0x73,0x65,0x5b,0x58,0x56,0x62,0x58,0x66,
			0x88,0x9a,0xb8,0xc3,0xc5,0xbe,0xad,0xac,0xa6,0x84,0x6a,0x31,0x31,0x31,0x31,0x31,0x83,0xab,
			0xc7,0xc3,0xc6,0xb8,0xa9,0x9b,0x69,0x66,0x55,0x38,0x31,0x31,0x3c,0x4f,0xa2,0xb8,0xc5,0xc3,
			0xc5,0xa3,0x87,0x74,0x52,0x53,0x58,0x57,0x5f,0x70,0x7b,0x8f,0xb6,0xc2,0xc4,0xc4,0xc3,0x99,
			0x83,0x5a,0x31,0x31,0x31,0x31,0x4a,0x9a,0xb9,0xb8,0xc8,0xc1,0xc3,0xc0,0xb1,0x90,0x6a,0x48,
			0x31,0x31,0x31,0x33,0x61,0x9a,0xb8,0xc6,0xc6,0xc4,0xb0,0xa1,0x97,0x7c,0x6e,0x63,0x54,0x53,
			0x66,0x7b,0x94,0xc5,0xc6,0xc4,0x97,0x4f,0x31,0x31,0x31,0x31,0x5f,0x7a,0x8a,0xa0,0xaa,0x9d,
			0xaf,0xb5,0xb6,0xba,0x8f,0x73,0x52,0x31,0x31,0x3f,0x57,0x69,0x9c,0xa9,0xab,0xac,0xa7,0xa6,
			0xa4,0xa5,0x94,0x8a,0x83,0x6e,0x71,0x8a,0xa5,0xaf,0xad,0x99,0x72,0x31,0x31,0x31,0x45,0x69,
			0xaa,0xbf,0xc6,0xb3,0x9c,0x85,0x7a,0x74,0x8a,0x86,0x80,0x6e,0x58,0x41,0x41,0x4c,0x73,0x8e,
			0xad,0xb0,0xaf,0x94,0x89,0x84,0x81,0x89,0x91,0x95,0x94,0x8b,0x8d,0x99,0xa7,0xb0,0x9a,0x88,
			0x68,0x31,0x31,0x31,0x38,0x55,0x94,0xaa,0xb7,0xb4,0xa8,0x9a,0x8a,0x8a,0x8c,0x8c,0x87,0x6f,
			0x5e,0x3f,0x3c,0x3e,0x5b,0x70,0x88,0xa8,0xb0,0xb1,0xaa,0xa4,0x99,0x97,0x95,0x9b,0xa0,0xac,
			0xac,0xac,0x87,0x6e,0x57,0x31,0x31,0x31,0x3a,0x55,0x8a,0xa0,0xab,0xb5,0xb1,0x9f,0x9c,0x95,
			0x8d,0x87,0x78,0x6b,0x5f,0x47,0x42,0x44,0x55,0x64,0x87,0x95,0xa2,0xb0,0xb1,0xae,0xa5,0xa1,
			0x9a,0x9b,0x9f,0xa8,0xad,0xaf,0x9e,0x85,0x54,0x39,0x31,0x31,0x31,0x40,0x75,0x92,0xaf,0xb3,
			0xb3,0x9d,0x96,0x8e,0x87,0x86,0x81,0x7b,0x72,0x5d,0x50,0x47,0x48,0x52,0x6f,0x80,0x92,0xa6,
			0xad,0xa9,0xa5,0x9f,0x97,0x97,0x9a,0xa4,0xa8,0xb0,0xaa,0x9f,0x71,0x5c,0x40,0x31,0x31,0x36,
			0x4d,0x66,0x8f,0x95,0x9f,0x98,0x8f,0x8f,0x8a,0x8f,0x90,0x94,0x86,0x7b,0x75,0x57,0x54,0x4e,
			0x57,0x5b,0x7e,0x8a,0x9a,0xad,0xb1,0xaf,0xa7,0xa3,0x9a,0x9a,0x9b,0x9d,0x9f,0x9f,0x84,0x77,
			0x4f,0x41,0x33,0x31,0x31,0x46,0x63,0x7d,0x94,0xa2,0x98,0x9f,0x92,0x8a,0x84,0x84,0x81,0x82,
			0x83,0x7e,0x7c,0x6e,0x6a,0x65,0x6e,0x75,0x86,0xa3,0xb0,0xb9,0xb5,0xac,0x91,0x85,0x7b,0x75,
			0x77,0x84,0x8c,0x8b,0x83,0x73,0x66,0x4e,0x4a,0x4f,0x5b,0x66,0x83,0x8b,0x92,0x8e,0x88,0x79,
			0x76,0x75,0x7e,0x86,0x91,0x93,0x92,0x86,0x7e,0x76,0x6b,0x6b,0x76,0x80,0x89,0x9a,0x9d,0x9b,
			0x90,0x86,0x75,0x71,0x6e,0x74,0x78,0x80,0x8c,0x91,0x8f,0x8b,0x84,0x79,0x75,0x72,0x75,0x78,
			0x81,0x84,0x87,0x84,0x82,0x7a,0x78,0x76,0x78,0x7b,0x7e,0x84,0x86,0x86,0x83,0x81,0x7a,0x79,
			0x78,0x7a,0x7d,0x83,0x86,0x88,0x88,0x86,0x83,0x7e,0x7c,0x7a,0x7c,0x7e,0x82,0x83,0x84,0x84,
			0x83,0x80,0x7e,0x7d,0x7d,0x7d,0x7f,0x81,0x80,0x7f,0x7e,0x7c,0x79,0x78,0x79,0x7c,0x7e,0x7f,
			0x81,0x80,0x80,0x7e,0x7d,0x7d,0x7e,0x7e,0x81,0x82,0x82,0x82,0x81,0x80,0x7d,0x7d,0x7e,0x7f,
			0x81,0x83,0x83,0x84,0x83,0x81,0x7f,0x7e,0x7d,0x7e,0x7e,0x80,0x81,0x81,0x80,0x7e,0x7e,0x7e,
			0x7e,0x7e,0x7f,0x80,0x81,0x80,0x80,0x7e,0x7f,0x7e,0x80,0x7f,0x81,0x82,0x82,0x82,0x81,0x80,
			0x7e,0x7d,0x7c,0x7d,0x80,0x81,0x82,0x82,0x81,0x81,0x80,0x80,0x80,0x81,0x80,0x80,0x7f,0x7e,
			0x7d,0x7d,0x7c,0x7c,0x7e,0x7f,0x80,0x81,0x81,0x81,0x80,0x80,0x80,0x80,0x80,0x81,0x81,0x81,
			0x81,0x80,0x7e,0x7e,0x7d,0x7e,0x7e,0x7e,0x7f,0x7f,0x80,0x80,0x80,0x80,0x81,0x80,0x81,0x80,
			0x80,0x80,0x80,0x7f,0x7f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7f,0x7f,0x80,0x7f,0x80,0x80,
			0x80,0x80,0x81,0x81,0x81,0x80,0x7e,0x7e,0x7e,0x7c,0x7c,0x7d,0x7e,0x7e,0x80,0x80,0x80,0x80,
			0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7f,0x7f,0x7f,
			0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x80,0x80,0x80,0x7e,0x7e,0x7d,0x7e,0x7e,
			0x7e,0x7f,0x7f,0x7e,0x7e,0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x7f,0x80,0x7f,
			0x7e,0x7f,0x80,0x7f,0x7f,0x7f,0x7f,0x7f,0x80,0x7f,0x80,0x7f,0x80,0x80,0x81,0x80,0x80,0x7f,
			0x80,0x7d,0x7e,0x7e,0x7e,0x7e,0x7f,0x80,0x80,0x80,0x80,0x80,0x81,0x80,0x80,0x80,0x7f,0x7f,
			0x7f,0x7f,0x7e,0x7f,0x7f,0x80,0x80,0x80,0x80,0x7f,0x7f,0x7e,0x7f,0x80,0x7f,0x80,0x80,0x80,
			0x80,0x80,0x80,0x80,0x7e,0x7e,0x7e,0x7e,0x7e,0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x7f,0x7f,
			0x7f,0x7f,0x7e,0x80,0x7f,0x80,0x80,0x80,0x7f,0x80,0x7f,0x7f,0x80,0x7f,0x80,0x80,0x80,0x80,
			0x80,0x7f,0x7f,0x7f,0x7f,0x7f,0x80,0x80,0x80,0x7f,0x7f,0x7f,0x7f,0x7f,0x80,0x7f,0x80,0x80,
			0x80,0x80,0x80,0x80,0x80,0x80,0x7f,0x7f,0x7e,0x80,0x7f,0x80,0x80,0x80,0x80,0x7f,0x7e,0x7f,
			0x7f,0x80,0x80,0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x7f,0x80,0x80,0x7f,0x7e,0x7f,0x7f,0x7f,
			0x7f,0x7f,0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x7f,0x7e,0x7e,0x7e,0x7f,0x7e,0x7e,
			0x7f,0x80,0x7f,0x7f,0x7e,0x7f,0x7f,0x80,0x80,0x7f,0x7f,0x7f,0x80,0x80,0x80,0x7f,0x80,0x80,
			0x7f,0x7f,0x7f,0x7e,0x7e,0x80,0x80,0x80,0x80,0x80,0x81,0x80,0x80,0x7f,0x7f,0x7e,0x7e,0x7e,
			0x7e,0x80,0x78,0x5f,0x94,0x8a,0x90,0x7c,0x78,0x5b,0xa7,0xaa,0x89,0x68,0x68,0x7f,0xa1,0x95,
			0x7b,0x61,0x43,0x97,0xaa,0xae,0x76,0x4e,0x5b,0xac,0x97,0x76,0x4f,0x6e,0x88,0x99,0x90,0x88,
			0x31,0x7a,0xbb,0xac,0x86,0x6e,0x6e,0xa9,0x98,0x7c,0x45,0x52,0x71,0xab,0x9e,0x8a,0x7d,0x86,
			0x90,0x97,0x8a,0x5f,0x51,0x57,0x8c,0x80,0x83,0x7c,0x8f,0x94,0x99,0x8e,0x85,0x80,0x87,0x85,
			0x81,0x7c,0x87,0x87,0x64,0x6a,0x6d,0x75,0x78,0x74,0x87,0x7c,0x92,0x89,0x96,0x89,0x91,0x86,
			0x87,0x71,0x7b,0x80,0x67,0x76,0x66,0x6d,0x75,0x7d,0x79,0x83,0x8f,0x93,0x9c,0x8d,0x82,0x7c,
			0x7b,0x7f,0x73,0x73,0x79,0x70,0x80,0x7a,0x8c,0x7a,0x84,0x83,0x83,0x84,0x83,0x8c,0x8b,0x79,
			0x80,0x89,0x6b,0x79,0x7c,0x81,0x7d,0x81,0x86,0x8f,0x84,0x87,0x91,0x94,0x89,0x7e,0x88,0x91,
			0x86,0x87,0x7e,0x81,0x80,0x86,0x78,0x7b,0x81,0x7a,0x70,0x78,0x75,0x6d,0x66,0x6a,0x77,0x78,
			0x79,0x7d,0x86,0x8a,0x8e,0x91,0x97,0x91,0x94,0x9d,0x97,0x96,0x9c,0x99,0xa2,0xa4,0xb0,0x6c,
			0x47,0x80,0x5d,0x50,0x38,0x62,0x58,0x56,0x6b,0x7b,0x7c,0x7e,0x88,0x75,0x79,0x6a,0x59,0x52,
			0x70,0x6d,0x68,0x7f,0xac,0xaf,0xa4,0xc0,0xc3,0xbd,0xb3,0xbd,0xc3,0xbc,0x73,0x67,0x7b,0x52,
			0x31,0x31,0x5a,0x4d,0x60,0x87,0x95,0x9e,0xa1,0x99,0x86,0x81,0x76,0x47,0x3e,0x4b,0x3e,0x4a,
			0x6d,0x87,0x9a,0xc8,0xc1,0xbb,0xc3,0xc3,0xbf,0xbe,0xc3,0x7e,0x31,0x89,0x38,0x31,0x31,0x51,
			0x70,0x77,0xa7,0xad,0xb7,0xb5,0xb6,0x7c,0x71,0x77,0x5a,0x31,0x42,0x46,0x31,0x4a,0x71,0x99,
			0x9f,0xc9,0xc1,0xc7,0xc3,0xc5,0xbe,0xb7,0xc4,0x31,0x31,0x45,0x31,0x31,0x31,0x7e,0x84,0xb3,
			0xc8,0xbc,0xc1,0xc0,0x93,0x60,0x59,0x4a,0x31,0x31,0x3a,0x47,0x45,0x71,0x8f,0xb9,0xba,0xc2,
			0xc3,0xc5,0xc3,0xc3,0xb7,0xbe,0xbf,0x65,0x31,0x68,0x31,0x31,0x31,0xb3,0x8f,0xb5,0xba,0xc5,
			0xbf,0xc1,0x8e,0x34,0x52,0x31,0x31,0x31,0x5f,0x5a,0x6a,0x97,0xad,0xc1,0xc6,0xc3,0xc4,0xc3,
			0xc3,0xb0,0xb0,0xc0,0x75,0x31,0x6a,0x49,0x31,0x31,0x73,0x95,0xb5,0xcd,0xbf,0xbb,0xbe,0x8e,
			0x3f,0x47,0x3f,0x31,0x31,0x3f,0x60,0x7b,0x8c,0xaa,0xb9,0xbe,0xbe,0xc2,0xc3,0xbf,0xae,0xad,
			0xbf,0x73,0x31,0x5b,0x33,0x31,0x31,0x7a,0x9d,0xc0,0xc9,0xba,0xba,0xb6,0x79,0x31,0x3f,0x37,
			0x31,0x31,0x52,0x76,0x8c,0xac,0xa7,0xb2,0xbb,0xad,0xb1,0xc2,0xc0,0xbd,0xab,0xc3,0x5f,0x31,
			0x31,0x39,0x31,0x31,0x8e,0xb2,0xc0,0xc9,0xba,0xb5,0xc2,0xb3,0x31,0x31,0x31,0x31,0x31,0x60,
			0x78,0x82,0xab,0xac,0xba,0xb9,0xab,0xb8,0xc5,0xbe,0xbb,0xad,0xc5,0xa7,0x31,0x41,0x59,0x31,
			0x31,0x4b,0xa5,0xa8,0xcc,0xbc,0xb6,0xc1,0x95,0x3c,0x31,0x39,0x31,0x31,0x4c,0x85,0x8c,0xad,
			0xa5,0xad,0xaf,0xab,0x9c,0xc3,0xc0,0xbf,0xaf,0xc1,0x5d,0x31,0x31,0x31,0x31,0x31,0x8e,0xaa,
			0xc6,0xc6,0xba,0xb3,0xc6,0xab,0x31,0x31,0x31,0x31,0x31,0x65,0x81,0x90,0xba,0xa8,0xb2,0xb4,
			0xa1,0x9c,0xb6,0xc3,0xb3,0xbb,0xc1,0xa1,0x31,0x5e,0x54,0x31,0x31,0x53,0x9b,0xaa,0xcc,0xb6,
			0xb0,0xc0,0x86,0x35,0x39,0x38,0x31,0x35,0x56,0x80,0x99,0xb2,0x9f,0xa8,0xae,0xa5,0x9d,0xc6,
			0xc0,0xc2,0xb1,0xc3,0x9d,0x31,0x52,0x31,0x31,0x31,0xa4,0x9d,0xb3,0xbe,0xbe,0xbf,0xc2,0x85,
			0x31,0x3a,0x31,0x31,0x33,0x73,0x87,0x9d,0xaf,0xa4,0xb5,0xa5,0x9f,0xb3,0xc2,0xbf,0xb2,0xbe,
			0xa3,0x31,0x31,0x5f,0x31,0x31,0x51,0xa2,0xaa,0xcb,0xbe,0xb3,0xc1,0xc3,0x35,0x31,0x3e,0x31,
			0x31,0x4d,0x72,0x9b,0xad,0xa9,0xb8,0xc1,0xac,0xa6,0xbe,0xbd,0xb8,0xbb,0xb8,0x6b,0x31,0x6d,
			0x31,0x31,0x31,0x7f,0x9b,0xc0,0xc8,0xc3,0xba,0xb1,0x61,0x31,0x3f,0x31,0x31,0x34,0x63,0x8d,
			0xaa,0xb5,0xb0,0xbd,0xa5,0x99,0xa4,0xc5,0xb7,0xb2,0xc3,0xa1,0x31,0x5f,0x64,0x31,0x31,0x54,
			0x8f,0xa5,0xbf,0xc2,0xbc,0xc3,0x8f,0x3a,0x44,0x3a,0x31,0x31,0x4f,0x81,0x9b,0xad,0xae,0xbe,
			0xb6,0xa9,0x9b,0xb9,0xb1,0xa5,0xc8,0xb5,0x31,0x4c,0x9d,0x31,0x31,0x99,0x94,0x84,0xc5,0xb8,
			0xbb,0xc7,0xa5,0x31,0x4d,0x54,0x31,0x31,0x40,0x78,0x8f,0xa4,0xb2,0xc1,0xbd,0xac,0xa5,0xb6,
			0xa7,0xaf,0xc5,0xb8,0x31,0x62,0xb1,0x31,0x31,0xb2,0x90,0x76,0xc1,0xb3,0xb9,0xca,0x9d,0x31,
			0x5d,0x5f,0x31,0x31,0x70,0x76,0x8c,0xaa,0xac,0xbd,0xbb,0xaa,0xac,0xc3,0xaa,0xab,0xc7,0xad,
			0x31,0x83,0x31,0x31,0x31,0xbd,0x79,0x83,0xba,0xb3,0xc0,0xc6,0x87,0x31,0x6c,0x52,0x31,0x31,
			0x73,0x77,0x91,0xaf,0xb2,0xc5,0xa7,0xa3,0xb1,0xa9,0x9e,0xba,0x91,0x31,0xa8,0x75,0x31,0x31,
			0x81,0xb9,0xa4,0xcd,0xb9,0xb2,0xc6,0x6c,0x31,0x6c,0x38,0x31,0x33,0x62,0x74,0x96,0xa8,0xba,
			0xc5,0xc1,0xa4,0xa7,0xb2,0xa1,0x94,0xc7,0x89,0x31,0xab,0x6b,0x31,0x31,0x8f,0x68,0xac,0xcd,
			0xb6,0xaf,0xc8,0x66,0x31,0x6d,0x36,0x31,0x3c,0x65,0x72,0x94,0xab,0xb8,0xc5,0xbd,0xa8,0xb0,
			0xa8,0x8f,0x8a,0xc5,0x91,0x31,0xa5,0x7f,0x31,0x31,0x86,0x71,0xa8,0xcd,0xad,0xad,0xb7,0x69,
			0x31,0x75,0x3f,0x31,0x3d,0x65,0x7a,0x8d,0xa8,0xb6,0xc5,0xbf,0xa5,0xaf,0xaa,0x97,0x90,0xc5,
			0x8b,0x31,0xab,0x6d,0x31,0x31,0x93,0x69,0xac,0xb1,0xb3,0xb1,0xb5,0x64,0x31,0x70,0x39,0x31,
			0x3f,0x6a,0x7b,0x8c,0xa2,0xbd,0xc4,0xa7,0xa1,0xab,0xa9,0x91,0x8c,0xc3,0xa8,0x31,0x90,0x9e,
			0x31,0x31,0xbe,0x77,0x8b,0xb6,0xaf,0xb2,0xc0,0x81,0x42,0x77,0x4d,0x31,0x31,0x5d,0x79,0x87,
			0xa8,0xbe,0xc3,0xaa,0x9f,0xad,0xa7,0x97,0x91,0xae,0xc8,0x31,0x31,0xb1,0x31,0x31,0x80,0xc2,
			0x62,0xcd,0xac,0xaf,0xc5,0xb8,0x31,0x5c,0x83,0x31,0x31,0x6f,0x70,0x6e,0x8a,0xa4,0xc3,0xb8,
			0xa3,0xae,0xb7,0xab,0x8a,0x9c,0xb3,0xa2,0x31,0xa2,0xa3,0x31,0x31,0x65,0xc6,0x81,0xcd,0xaf,
			0xb8,0xc1,0x7b,0x36,0x5d,0x4a,0x31,0x31,0x64,0x71,0x80,0x8e,0xb8,0xc5,0xb8,0x8f,0xa4,0xb9,
			0x87,0x8a,0xa6,0xb7,0xc7,0x31,0x31,0xa8,0x31,0x31,0x6b,0xc3,0x63,0xc8,0xb6,0xb4,0xc1,0xba,
			0x49,0x75,0x95,0x31,0x31,0x37,0x69,0x5b,0x8a,0x93,0xae,0xb0,0x92,0xad,0xb2,0xa6,0x86,0x9d,
			0xa7,0xbb,0xc4,0x31,0x31,0xc6,0x31,0x31,0x31,0xb3,0x3f,0xc8,0xb5,0xb8,0xc1,0xb3,0x76,0x8d,
			0x94,0x31,0x31,0x51,0x4c,0x55,0x79,0x9c,0xa8,0xad,0x91,0x97,0xa9,0x8a,0x79,0xa2,0xa7,0x99,
			0xb8,0xc5,0x31,0x31,0x75,0x31,0x31,0x88,0xa1,0x31,0xcb,0xad,0xba,0xc5,0xba,0x62,0x96,0x9c,
			0x31,0x31,0x5e,0x46,0x44,0x8c,0x8c,0xa0,0xa5,0xa2,0xa2,0xaa,0x92,0x8e,0x98,0x94,0x8d,0x96,
			0xc6,0xb1,0x3e,0x87,0xbb,0x31,0x31,0x4d,0x63,0x50,0xa2,0xad,0xb8,0xbb,0x9a,0x73,0xa3,0x68,
			0x31,0x48,0x5b,0x45,0x51,0x7c,0x94,0xa8,0xa7,0xa4,0xac,0x90,0x7e,0x7a,0x96,0x82,0x85,0x9f,
			0xb2,0xc0,0x94,0x31,0xbe,0x8f,0x31,0x31,0x71,0x41,0x5d,0xa5,0xb1,0xc1,0xb2,0x87,0x9d,0x94,
			0x31,0x3e,0x57,0x45,0x48,0x7b,0x8b,0xa0,0x9f,0x9c,0x9f,0xa0,0x83,0x7b,0x86,0x83,0x79,0x90,
			0xa2,0xaf,0xc5,0xac,0x31,0x31,0xa0,0x31,0x31,0x57,0x8c,0x38,0x9a,0xb5,0xb6,0xc9,0xad,0x9c,
			0xab,0x65,0x31,0x41,0x4f,0x36,0x4a,0x6f,0x7d,0xa2,0xaa,0xa0,0xba,0xaa,0x86,0x81,0x81,0x6c,
			0x6a,0x75,0x93,0x9b,0xac,0xc1,0xc4,0x5a,0x31,0xc2,0x31,0x31,0x37,0x7e,0x3f,0x7d,0xa6,0xb7,
			0xc8,0xc1,0xa5,0xad,0xb8,0x3e,0x41,0x54,0x3c,0x31,0x66,0x78,0x82,0x99,0xb0,0xb3,0xb7,0xa0,
			0x7f,0x84,0x7b,0x57,0x67,0x7b,0x88,0x97,0xb1,0xc3,0xc3,0xb9,0x6f,0x93,0xa3,0x31,0x31,0x31,
			0x37,0x31,0x69,0xb6,0xbf,0xc8,0xc3,0xc2,0xb7,0x84,0x4f,0x50,0x47,0x31,0x31,0x54,0x7a,0x8e,
			0x9f,0xb6,0xc5,0xbe,0x9d,0x95,0x6d,0x5c,0x57,0x5d,0x62,0x7d,0x91,0xa0,0xc2,0xc3,0xc4,0xc1,
			0x9f,0x45,0xb6,0x69,0x31,0x31,0x3c,0x31,0x48,0xb5,0xbd,0xc2,0xc3,0xc5,0xc4,0xa8,0x64,0x4e,
			0x4f,0x33,0x31,0x38,0x5c,0x80,0x97,0xb9,0xc6,0xc3,0xad,0x9c,0x92,0x65,0x47,0x4f,0x51,0x5a,
			0x72,0x8a,0xb3,0xbf,0xc3,0xc4,0xc3,0xc3,0x91,0x31,0xb4,0x53,0x31,0x31,0x4c,0x5e,0x55,0xa5,
			0xc4,0xc2,0xc8,0xc5,0xc4,0xbe,0x5b,0x3b,0x4c,0x31,0x31,0x3a,0x5f,0x6f,0x98,0xaf,0xc6,0xc3,
			0xc1,0xa6,0xa5,0x62,0x4a,0x3b,0x49,0x3e,0x4d,0x79,0x93,0xb3,0xbd,0xc4,0xc3,0xc4,0xc3,0xc2,
			0xa4,0x31,0x9a,0x65,0x31,0x31,0x41,0x31,0x4c,0xc6,0xbe,0xbf,0xc2,0xc5,0xc0,0xab,0x6b,0x47,
			0x54,0x37,0x31,0x36,0x70,0x6f,0x8f,0xbb,0xc5,0xc5,0xc1,0xb2,0x92,0x80,0x54,0x3f,0x36,0x41,
			0x45,0x62,0x94,0xa7,0xb5,0xc5,0xc5,0xc3,0xc3,0xc4,0xb6,0xa2,0x66,0x44,0xb0,0x31,0x31,0x31,
			0x5e,0x34,0x7a,0xc7,0xb6,0xc3,0xbf,0xc5,0xaf,0x89,0x47,0x51,0x4f,0x31,0x31,0x4c,0x71,0x7e,
			0x9d,0xc0,0xc5,0xbf,0xb7,0xb2,0x7e,0x65,0x5b,0x44,0x3c,0x41,0x51,0x63,0x8f,0x9a,0xaa,0xc3,
			0xc3,0xbb,0xbe,0xb8,0xac,0xaf,0xa1,0x66,0x31,0xb8,0x31,0x31,0x31,0x89,0x3c,0x7f,0xb6,0xa4,
			0xbc,0xc7,0xbb,0xa4,0x92,0x4d,0x4c,0x5f,0x3c,0x31,0x53,0x71,0x75,0x91,0x9f,0xbd,0xc0,0xb4,
			0xaf,0xa6,0x75,0x50,0x5d,0x47,0x38,0x3a,0x69,0x6c,0x87,0xa5,0xaf,0xc0,0xbb,0xb8,0xb3,0xad,
			0x9a,0x9a,0xa7,0x8c,0x65,0x45,0x36,0x31,0x39,0x8b,0x71,0x34,0xaf,0xbc,0xb5,0xc0,0xa0,0xa5,
			0xa0,0x9a,0x5d,0x6d,0x4a,0x37,0x4e,0x71,0x6d,0x86,0x94,0x9f,0xb1,0xa8,0xa8,0xa7,0x9b,0x77,
			0x6b,0x5a,0x61,0x4b,0x38,0x69,0x71,0x7b,0x92,0xad,0xa5,0xaa,0xb6,0xaa,0x9e,0x9a,0x97,0x94,
			0xa0,0xa0,0x87,0x31,0xb8,0x31,0x31,0x3f,0x8c,0x36,0x4d,0xb9,0x92,0xc8,0x9a,0xb0,0x99,0xad,
			0x63,0x75,0x7b,0x3c,0x4e,0x55,0x73,0x5b,0x87,0x90,0x9f,0xa5,0xa2,0xad,0xa1,0x9b,0x7b,0x7d,
			0x70,0x3f,0x57,0x66,0x49,0x61,0x86,0x86,0x8c,0xb3,0xb0,0xa6,0xaa,0xa0,0x92,0x91,0x8c,0x89,
			0x96,0x96,0x89,0x31,0xc1,0x79,0x31,0x44,0x72,0x35,0x49,0x89,0x84,0x87,0xbc,0xab,0xb2,0xb0,
			0x80,0x6d,0x89,0x74,0x49,0x5c,0x65,0x58,0x66,0x7d,0x8f,0x96,0x97,0xa5,0xa8,0x9f,0x90,0x84,
			0x80,0x71,0x4f,0x49,0x71,0x3e,0x52,0x76,0x89,0x81,0xa7,0xb6,0xa4,0xa6,0xa3,0x9f,0x8a,0x89,
			0x86,0x8e,0x95,0x93,0x98,0x55,0xaa,0xa2,0x3b,0x54,0x62,0x45,0x39,0x79,0x86,0x70,0x91,0x9a,
			0xaf,0xa8,0x93,0x76,0x91,0x84,0x61,0x68,0x69,0x5b,0x5d,0x79,0x7f,0x87,0x8c,0x9a,0xa2,0x9d,
			0x98,0x94,0x91,0x81,0x73,0x70,0x6b,0x4f,0x6a,0x55,0x65,0x72,0x87,0x89,0x91,0xa7,0x9d,0xaa,
			0x9c,0x95,0x8d,0x89,0x80,0x81,0x8e,0x8f,0x92,0x83,0x65,0xc9,0x31,0x5b,0x5b,0x75,0x3c,0x55,
			0x9a,0x6d,0x97,0x80,0xa2,0x95,0x9d,0x81,0x8f,0x94,0x65,0x73,0x71,0x70,0x5d,0x6b,0x78,0x7b,
			0x80,0x83,0x94,0x91,0x94,0x8e,0x96,0x90,0x7e,0x7e,0x7b,0x71,0x65,0x5d,0x6c,0x73,0x60,0x74,
			0x84,0x86,0x88,0xa4,0x9f,0x95,0x97,0x93,0x8e,0x82,0x7c,0x7d,0x81,0x86,0x94,0x98,0x8c,0x5d,
			0xaf,0x3c,0x54,0x61,0x7a,0x4c,0x4f,0x8c,0x7c,0x8e,0x86,0x91,0x92,0x99,0x8d,0x8e,0x94,0x70,
			0x72,0x76,0x77,0x66,0x72,0x77,0x77,0x7a,0x80,0x8b,0x8a,0x8e,0x8e,0x91,0x8f,0x84,0x84,0x81,
			0x7c,0x72,0x76,0x76,0x6e,0x67,0x79,0x79,0x6e,0x81,0x88,0x87,0x89,0x99,0x93,0x8f,0x91,0x8b,
			0x87,0x83,0x7e,0x7d,0x82,0x80,0x8c,0x97,0x68,0x6a,0xa0,0x5a,0x66,0x74,0x79,0x5b,0x71,0x81,
			0x73,0x86,0x7e,0x91,0x8e,0x95,0x85,0x8b,0x91,0x7b,0x79,0x7b,0x7a,0x70,0x74,0x77,0x79,0x79,
			0x7b,0x89,0x87,0x8a,0x8a,0x8e,0x8e,0x85,0x84,0x81,0x7f,0x79,0x77,0x77,0x76,0x76,0x7a,0x7c,
			0x74,0x78,0x84,0x7d,0x7a,0x8d,0x8a,0x81,0x88,0x8c,0x89,0x80,0x84,0x7e,0x7f,0x80,0x7d,0x80,
			0x83,0x83,0x84,0x8e,0x8e,0x91,0x73,0x7f,0x7c,0x69,0x6a,0x78,0x73,0x65,0x73,0x7c,0x75,0x7a,
			0x7e,0x89,0x89,0x89,0x89,0x8c,0x86,0x80,0x7f,0x81,0x7c,0x76,0x77,0x79,0x77,0x76,0x79,0x80,
			0x81,0x81,0x86,0x89,0x86,0x85,0x86,0x86,0x83,0x7e,0x7e,0x7b,0x78,0x76,0x76,0x79,0x79,0x7b,
			0x7f,0x80,0x80,0x80,0x86,0x7b,0x80,0x7b,0x76,0x7f,0x7b,0x7d,0x7c,0x85,0x81,0x82,0x87,0x87,
			0x84,0x87,0x87,0x85,0x89,0x8e,0x91,0x87,0x75,0x94,0x65,0x62,0x66,0x74,0x65,0x5f,0x84,0x79,
			0x80,0x81,0x8c,0x8f,0x8f,0x8a,0x8b,0x8f,0x7b,0x7a,0x7b,0x7a,0x70,0x6d,0x77,0x77,0x75,0x79,
			0x80,0x85,0x85,0x87,0x8f,0x8e,0x88,0x86,0x86,0x80,0x7b,0x79,0x77,0x75,0x73,0x74,0x76,0x7b,
			0x7b,0x83,0x85,0x86,0x87,0x88,0x89,0x86,0x84,0x80,0x80,0x7c,0x78,0x79,0x79,0x77,0x78,0x7b,
			0x7c,0x7d,0x82,0x83,0x85,0x86,0x86,0x86,0x86,0x83,0x81,0x7e,0x7d,0x7b,0x79,0x78,0x79,0x79,
			0x79,0x7b,0x7e,0x80,0x80,0x84,0x84,0x86,0x84,0x84,0x83,0x80,0x7e,0x7c,0x7b,0x7b,0x7b,0x7a,
			0x7b,0x7c,0x7d,0x7f,0x80,0x81,0x84,0x83,0x83,0x84,0x83,0x81,0x80,0x7e,0x7c,0x7c,0x7b,0x7b,
			0x7b,0x7b,0x7c,0x7f,0x80,0x80,0x81,0x82,0x82,0x82,0x81,0x80,0x80,0x80,0x7e,0x7e,0x7c,0x7c,
			0x7c,0x7c,0x7e,0x7e,0x7e,0x7e,0x80,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x80,0x80,0x7f,0x7e,
			0x7e,0x7d,0x7c,0x7d,0x7e,0x7e,0x7e,0x7f,0x80,0x80,0x80,0x80,0x81,0x81,0x80,0x80,0x80,0x7f,
			0x7e,0x7e,0x7c,0x7c,0x7d,0x7d,0x7e,0x7e,0x80,0x80,0x81,0x81,0x81,0x80,0x80,0x80,0x7f,0x7e,
			0x7e,0x7e,0x7e,0x7d,0x7e,0x7e,0x7e,0x7e,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x7f,
			0x7e,0x7f,0x7f,0x7f,0x80,0x7f,0x7f,0x7f,0x80,0x80,0x80,0x80,0x7f,0x7f,0x7e,0x7f,0x7e,0x7f,
			0x7f,0x7e,0x7e,0x7f,0x7e,0x7e,0x7e,0x80,0x80,0x7f,0x7f,0x80,0x7f,0x7f,0x7f,0x80,0x80,0x80,
			0x80,0x80,0x80,0x7f,0x80,0x80,0x80,0x7f,0x7f,0x80,0x7e,0x7f,0x7e,0x7f,0x7f,0x7f,0x7f,0x7f,
			0x7e,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x80,0x80,0x80,0x80,0x80,0x80,0x7e,0x7e,0x7f,0x7e,0x7f,
			0x7e,0x7f,0x7e,0x7e,0x7e,0x7f,0x7f,0x7f,0x7f};


		// irrKlang 3D sound engine example 03, 
		// demonstrating playing sounds directly from memory
		[STAThread]
		static void Main(string[] args)
		{
			// start the sound engine with default parameters
			ISoundEngine engine = new ISoundEngine();

			// To make irrKlang know about the memory we want to play, we register
			// the memory chunk as a sound source. We specify the name "testsound", so
			// we can use the name later for playing back the sound. Note that you
			// could also specify a better fitting name like "ok.wav".
			// The method AddSoundSourceFromMemory() also returns a pointer to the created sound source,
			// it can be used as parameter for play2D() later, if you don't want to
			// play sounds via string names.

			ISoundSource source = engine.AddSoundSourceFromMemory(SoundDataArray, "testsound");

			// now play the sound until user presses 'q'

			Console.Out.WriteLine("\nPlaying sounds directly from memory");
			Console.Out.WriteLine("Press any key to play some sound, press 'q' to quit.");

			do
			{
				// play the sound we added to memory
				engine.Play2D("testsound");
			}
			while(_getch() != 'q'); // user pressed 'q' key, cancel
		}
		
		// some simple function for reading keys from the console
		[System.Runtime.InteropServices.DllImport("msvcrt")]
		static extern int _getch();
	}
}
