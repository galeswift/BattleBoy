#include "WinImage.h"

#include <assert.h>
#include "ResourceLoader.h"

using namespace Boy;

#include "BoyLib/CrtDbgNew.h"

WinImage::WinImage(ResourceLoader *loader, const std::string &path) : Image(loader,path)
{
	mTexture = NULL;
	mWidth = -1;
	mHeight = -1;
}

WinImage::WinImage(WinD3DInterface *d3dInterface, SDL_Surface* source) : Image(NULL,"")
{
	if( source && d3dInterface )
	{
		// remember the original image size:
		setSize(source->w,source->h);

		IDirect3DTexture9 *tex = d3dInterface->loadTexture(source);

		// set the texture:
		if (tex!=NULL)
		{	
			setTexture(tex, false);
		}
	}
	else
	{
		mTexture = NULL;
		mWidth = -1;
		mHeight = -1;
	}
}

WinImage::~WinImage()
{
	assert(mTexture == NULL);
}

int WinImage::getWidth()
{
	return mWidth;
}

int WinImage::getHeight()
{
	return mHeight;
}

bool WinImage::init(bool includeSounds)
{
	return mLoader->load(this);
}

void WinImage::destroy(bool includeSounds)
{
	if (mTexture!=NULL)
	{
		mTexture->Release();
		mTexture = NULL;
	}
}

void WinImage::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void WinImage::setTexture(IDirect3DTexture9 *tex, bool isScaled)
{
	mTexture = tex;
	mIsTextureScaled = isScaled;
}
