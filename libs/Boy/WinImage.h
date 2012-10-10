#pragma once

#include "Image.h"
#include "WinD3DInterface.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "SDL.h"

namespace Boy
{
	class WinImage : public Image
	{
	public:

		WinImage(ResourceLoader *loader, const std::string &path);
		WinImage(WinD3DInterface *d3dInterface, SDL_Surface* source);
		virtual ~WinImage();

		void setSize(int width, int height);

		const std::string &getPath() { return mPath; }

		void setTexture(IDirect3DTexture9 *tex, bool isScaled);

		inline IDirect3DTexture9 *getTexture() { return mTexture; }

		inline bool isTextureScaled() { return mIsTextureScaled; }

		// implementation of Image:
		virtual int getWidth();
		virtual int getHeight();

	private:

		// implementation of Resource:
		virtual bool init(bool includeSounds);
		virtual void destroy(bool includeSounds);

	private:

		IDirect3DTexture9 *mTexture;

		int mWidth;
		int mHeight;

		bool mIsTextureScaled;
	};
};
