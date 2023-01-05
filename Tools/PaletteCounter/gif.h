#pragma pack(push, 1)
struct GIFHeader
{
    char Signature [6];			// GIF file type header
    unsigned short int Width;	// Image width
	unsigned short int Height;	// Image height
	unsigned char ColourInfo;
    unsigned char Background, PixelAspect;
};
#pragma pack(pop)

struct ColourTable
{
	unsigned char Colour[256][3];
	CString strFiles;
	unsigned int iCount;

	ColourTable() { for (int i = 0; i < 256; i++ ) for (int j = 0; j < 3; j++ ) Colour[i][j] = 0; iCount = 1;}

	bool operator==(const ColourTable &c);
	const ColourTable operator=(const ColourTable &c);

	bool operator<(const ColourTable &c);
	bool operator>(const ColourTable &c);
};

bool ColourTable::operator==(const ColourTable &c)
{
	for (int i = 0; i < 256; i++ )
	{
		for (int j = 0; j < 3; j++ )
		{
			if ( Colour[i][j] != c.Colour[i][j] )
				return false;
		}
	}
	return true;
}

const ColourTable ColourTable::operator=(const ColourTable &c)
{
	for (int i = 0; i < 256; i++ )
	{
		for (int j = 0; j < 3; j++ )
		{
			Colour[i][j] = c.Colour[i][j];
		}
	}
	strFiles = c.strFiles;
	iCount = c.iCount;
	return *this;
}


bool ColourTable::operator<(const ColourTable &c)
{
	return (iCount < c.iCount);
}

bool ColourTable::operator>(const ColourTable &c)
{
	return (iCount > c.iCount);
}
