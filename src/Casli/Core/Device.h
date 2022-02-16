#include "CoreAPI.h"
#include "IBuffer.h";

class Texture2D;
class RenderTargetView;
class InputLayout;

class CORE_API Device
{
public:
	Device();
	~Device();

	void CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, IBuffer **buffer);
	void CreateRenderTargetView(void *gFbo, int width, int height, RenderTargetView **pRTView);
	void CreateDepthStencilState();
	void CreateInputLayout(const INPUT_ELEMENT_DESC *pInputElementDescs, unsigned int NumElements, InputLayout **ppInputLayout);
	bool CreateTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, Texture2D *pTexture);
};