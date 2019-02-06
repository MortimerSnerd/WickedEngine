#pragma once
#include "CommonInclude.h"
#include "wiGraphicsDevice.h"

class wiDepthTarget
{
private:
	std::unique_ptr<wiGraphicsTypes::Texture2D>		texture;
	std::unique_ptr<wiGraphicsTypes::Texture2D>		texture_resolvedMSAA;
	bool resolvedMSAAUptodate;
public:

	wiDepthTarget();
	~wiDepthTarget();
	
	void Initialize(int width, int height, UINT MSAAC);
	void InitializeCube(int size, bool independentFaces = false);
	void Clear(GRAPHICSTHREAD threadID);
	void CopyFrom(const wiDepthTarget&, GRAPHICSTHREAD threadID);

	wiGraphicsTypes::Texture2D* GetTexture() const { return texture.get(); }
	wiGraphicsTypes::Texture2D* GetTextureResolvedMSAA(GRAPHICSTHREAD threadID);
	wiGraphicsTypes::TextureDesc GetDesc() const { return GetTexture()->GetDesc(); }
};

