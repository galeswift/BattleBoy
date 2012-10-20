/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __SUPPORT_DATA_SUPPORT_TGALIB_H__
#define __SUPPORT_DATA_SUPPORT_TGALIB_H__

namespace   BoyLib {

class TGAFileData
{
public:
    TGAFileData(const char* pszFileName, const char* pszMode)
        : m_pBuffer(0)
        , m_uSize(0)
    {
        m_pBuffer = getFileData(pszFileName, pszMode, &m_uSize);
    }
    ~TGAFileData()
    {
        delete m_pBuffer;
    }

	unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
	{
		unsigned char * pBuffer = NULL;

		do 
		{
			// read the file from hardware
			FILE *fp;
			errno_t error = fopen_s(&fp, pszFileName, pszMode);
			assert(fp);

			fseek(fp,0,SEEK_END);
			*pSize = ftell(fp);
			fseek(fp,0,SEEK_SET);
			pBuffer = new unsigned char[*pSize];
			*pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
			fclose(fp);
		} while (0);
		return pBuffer;
	}

    bool reset(const char* pszFileName, const char* pszMode)
    {
        delete m_pBuffer;
        m_uSize = 0;
        m_pBuffer = getFileData(pszFileName, pszMode, &m_uSize);
        return (m_pBuffer) ? true : false;
    }

	unsigned char* Buffer() { return m_pBuffer; }
	unsigned long Size() { return m_uSize; }

private:
    unsigned char * m_pBuffer;
    unsigned long m_uSize;
};

enum {
	TGA_OK,
	TGA_ERROR_FILE_OPEN,
	TGA_ERROR_READING_FILE,
	TGA_ERROR_INDEXED_COLOR,
	TGA_ERROR_MEMORY,
	TGA_ERROR_COMPRESSED_FILE,
};

/** TGA format */
typedef struct sImageTGA {
	int status;
	unsigned char type, pixelDepth;
	
	/** map width */
	signed short width;
	
	/** map height */
	signed short height;
	
	/** raw data */
	unsigned char *imageData;
	int flipped;
} tImageTGA;

typedef struct sColorInfo
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}tColorInfo;

/// load the image header fields. We only keep those that matter!
bool tgaLoadHeader(unsigned char *Buffer, unsigned long bufSize, tImageTGA *psInfo);

/// loads the image pixels. You shouldn't call this function directly
bool tgaLoadImageData(unsigned char *Buffer, unsigned long bufSize, tImageTGA *psInfo);

/// this is the function to call when we want to load an image
tImageTGA * tgaLoad(const char *pszFilename);

// /converts RGB to greyscale
void tgaRGBtogreyscale(tImageTGA *psInfo);

/// releases the memory used for the image
void tgaDestroy(tImageTGA *psInfo);
}//namespace   BoyLib 

#endif // __SUPPORT_DATA_SUPPORT_TGALIB_H__
