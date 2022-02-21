#include "CoreAPI.h"
#include "IBuffer.h";

class Texture2D;
class SamplerState;
class RenderTargetView;
class InputLayout;
class DepthStencilView;

class CORE_API Device
{
public:
	Device();
	~Device();

	void CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, IBuffer **buffer);
	void CreateRenderTargetView(void *gFbo, int width, int height, RenderTargetView **pRTView);
	void CreateDepthStencilView(Texture2D *pDepthBuffer, DEPTH_STENCIL_VIEW_DESC *desc, DepthStencilView **ppDepthStencilView);
	void CreateInputLayout(const INPUT_ELEMENT_DESC *pInputElementDescs, unsigned int NumElements, InputLayout **ppInputLayout);
	bool CreateTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, Texture2D **pTexture);
	bool CreateSamplerState(SAMPLER_DESC *desc, SamplerState **pSamplerState);
};