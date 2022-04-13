#pragma once

typedef unsigned char Byte;
typedef unsigned short Word;

struct Registers
{
	struct
	{
		union
		{
			struct
			{
				Byte F;
				Byte A;
			};
			Word AF;
		};
	};

	struct
	{
		union
		{
			struct
			{
				Byte C;
				Byte B;
			};
			Word BC;
		};
	};

	struct
	{
		union
		{
			struct
			{
				Byte E;
				Byte D;
			};
			Word DE;
		};
	};

	struct
	{
		union
		{
			struct
			{
				Byte L;
				Byte H;
			};
			Word HL;
		};
	};

	Word SP;
	Word PC;

	Byte* r[8];
	Word* rp[4];
	Word* rp2[4];

} extern reg;