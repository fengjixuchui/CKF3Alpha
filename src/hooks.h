#pragma once

typedef struct
{
	struct model_s *(*CL_GetModelByIndex)(int index);
	void (*CL_AddToResourceList)(struct resource_s *pResource, struct resource_s *pList);
	struct event_hook_s *(*CL_FindEventHook)(char *name);
	void (*Info_SetValueForKey)(char *s, char *key, char *value, int maxsize);
	HRESULT (CALLBACK *VID_EnumDisplayModesProc)(void *lpDDSurfaceDesc, DWORD *pBPP);
	class CVideoMode_Common *(*VideoMode_Create)(void);
	void (__fastcall *LoadStartupGraphic)(void *, int);
	void (__fastcall *DrawStartupGraphic)(void *, int, HWND);
	void (*SDL_GetWindowSize)(HWND, int *, int *);
	void (*SDL_SwapBuffer)(HWND);
	//https://github.com/ValveSoftware/halflife/issues/1654
	void (*CL_ReadClientDLLData)(void);
	void (*CL_DemoParseSound)(void);
	void (*CL_DemoPlaySound)( int channel, char *sample, float attenuation, float volume, int flags, int pitch );
	FileHandle_t *cls_demofile;	
}HookFuncs_t;

extern HookFuncs_t gHookFuncs;

//extern void (*g_pfnMod_LoadStudioModel)(struct model_s *mod, byte *buffer);
//extern struct model_s *(*g_pfnCL_GetModelByIndex)(int index);
//extern void (*g_pfnCL_AddToResourceList)(struct resource_s *pResource, struct resource_s *pList);
//extern void (*g_pfnCL_FireEvents)(void);
//extern struct event_hook_s *(*g_pfnCL_FindEventHook)(char *name);
//extern void (*g_pfnInfo_SetValueForKey)(char *s, char *key, char *value, int maxsize);
//extern int (*g_pfnLoadTGA)(const char *szFilename, byte *buffer, int bufferSize, int *width, int *height);
//extern int (*g_pfnGL_LoadTexture)(char *identifier, int textureType, int width, int height, byte *data, qboolean mipmap, int iType, byte *pPa);
//extern int (*g_pfnGL_LoadTexture2)(char *identifier, int textureType, int width, int height, byte *data, qboolean mipmap, int iType, byte *pPal, int filter);
//extern void (*g_pfnGL_Bind)(int texnum);
//extern HRESULT (CALLBACK *g_pVID_EnumDisplayModesProc)(void *lpDDSurfaceDesc, DWORD *pBPP);
//extern class CVideoMode_Common *(*g_pfnVideoMode_Create)(void);

void InstallHook(void);