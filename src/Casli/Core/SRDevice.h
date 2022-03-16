#include "SRIBuffer.h"
#include "SRCore.h"
#include "SRTexture2D.h"
#include "SRSamplerState.h"
#include "SRRenderTargetView.h"
#include "SRInputLayout.h"
#include "SRDepthStencilView.h"
#include "SRBlendState.h"

class CORE_API SRDevice
{
public:
	SRDevice();
	~SRDevice();

	bool CreateBuffer(BUFFER_DESC *bd, SUBRESOURCE_DATA *sd, SRIBuffer **buffer);
	bool CreateRenderTargetView(void *gFbo, int width, int height, SRRenderTargetView **pRTView);
	bool CreateDepthStencilView(SRTexture2D *pDepthBuffer, DEPTH_STENCIL_VIEW_DESC *desc, SRDepthStencilView **ppDepthStencilView);
	bool CreateInputLayout(const INPUT_ELEMENT_DESC *pInputElementDescs, unsigned int NumElements, SRInputLayout **ppInputLayout);
	bool CreateTexture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd, SRTexture2D **pTexture);
	bool CreateSamplerState(SAMPLER_DESC *desc, SRSamplerState **pSamplerState);
	bool CreateBlendState(BLEND_DESC *desc, SRBlendState **pBlendState);
};