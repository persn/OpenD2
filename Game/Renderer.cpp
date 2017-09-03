#include "Renderer.hpp"

/*
 *	The renderer in OpenD2 is significantly different from the one in retail Diablo 2.
 *
 *	For starters, the window management and rendering are totally separated.
 *	There are three main render targets that we want to work with:
 *	- Software Renderer (SDL)
 *	- DirectX (d3d)
 *	- OpenGL (opengl)
 *
 *	The render target can be changed in D2.ini, registry, or commandline.
 *	Later on we will want to create a VideoTest.exe that can be used to set optimal video settings.
 *	(by default we will use software since it works on literally everything)
 */

D2Renderer* RenderTarget = nullptr;

static D2Renderer RenderTargets[OD2RT_MAX] = {
	{	// SDL Hardware-Accelerated Renderer
		Renderer_SDL_Init,
		Renderer_SDL_Shutdown,
		Renderer_SDL_Present,
		Renderer_SDL_RegisterTexture,
		Renderer_SDL_TextureFromDC6,
		Renderer_SDL_TextureFromAnimatedDC6,
		Renderer_SDL_DrawTexture,
		Renderer_SDL_DrawTextureFrames,
		Renderer_SDL_DrawTextureFrame,
		Renderer_SDL_DeregisterTexture,
		Renderer_SDL_SetTextureBlendMode,
		Renderer_SDL_RegisterAnimation,
		Renderer_SDL_DeregisterAnimation,
		Renderer_SDL_Animate,
		Renderer_SDL_SetAnimFrame,
	},

	{	// SDL Software Renderer
		nullptr,
	},

	{	// DirectX Renderer (Windows-only)
		nullptr,
	},

	{	// OpenGL Renderer
		nullptr,
	},
};

/*
 *	Initializes the renderer.
 *	Call after the window has been created.
 */
void Render_Init(D2GameConfigStrc* pConfig, OpenD2ConfigStrc* pOpenConfig, SDL_Window* pWindow)
{
	OpenD2RenderTargets DesiredRenderTarget = OD2RT_SDL;

	// Determine which render target to go with
	if (pConfig->bOpenGL)
	{
		DesiredRenderTarget = OD2RT_OPENGL;
	}
	else if (pConfig->bD3D)
	{
		DesiredRenderTarget = OD2RT_DIRECTX;
	}
	else if (pOpenConfig->bNoSDLAccel)
	{
		DesiredRenderTarget = OD2RT_SDL_SOFTWARE;
	}
	else
	{
		DesiredRenderTarget = OD2RT_SDL;
	}

	// Load palettes
	Pal_Init();

	RenderTarget = &RenderTargets[DesiredRenderTarget];
	RenderTarget->RF_Init(pConfig, pOpenConfig, pWindow);
}

/*
 *	Map rendertarget exports to game module exports
 *	@author	eezstreet
 */
void Render_MapRenderTargetExports(D2ModuleImportStrc* pExport)
{
	pExport->R_RegisterTexture = RenderTarget->RF_RegisterTexture;
	pExport->R_RegisterDC6Texture = RenderTarget->RF_TextureFromStitchedDC6;
	pExport->R_RegisterAnimatedDC6 = RenderTarget->RF_TextureFromAnimatedDC6;
	pExport->R_DrawTexture = RenderTarget->RF_DrawTexture;
	pExport->R_DrawTextureFrame = RenderTarget->RF_DrawTextureFrame;
	pExport->R_DrawTextureFrames = RenderTarget->RF_DrawTextureFrames;
	pExport->R_SetTextureBlendMode = RenderTarget->RF_SetTextureBlendMode;
	pExport->R_Present = RenderTarget->RF_Present;
	pExport->R_DeregisterTexture = RenderTarget->RF_DeregisterTexture;
	pExport->R_RegisterAnimation = RenderTarget->RF_RegisterAnimation;
	pExport->R_DeregisterAnimation = RenderTarget->RF_DeregisterAnimation;
	pExport->R_Animate = RenderTarget->RF_Animate;
	pExport->R_SetAnimFrame = RenderTarget->RF_SetAnimFrame;
}