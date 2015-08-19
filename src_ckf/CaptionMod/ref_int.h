#pragma once

#include <gl/gl.h>
#include <gl/glext.h>

#ifndef SYNCTYPE_T
#define SYNCTYPE_T
typedef enum
{
	ST_SYNC = 0,
	ST_RAND
}
synctype_t;
#endif

typedef struct
{
	int ident;
	int version;
	int type;
	int texFormat;
	float boundingradius;
	int width;
	int height;
	int numframes;
	float beamlength;
	synctype_t synctype;
}
dsprite_t;

#define SPR_VP_PARALLEL_UPRIGHT 0
#define SPR_FACING_UPRIGHT 1
#define SPR_VP_PARALLEL 2
#define SPR_ORIENTED 3
#define SPR_VP_PARALLEL_ORIENTED 4

#define SPR_NORMAL 0
#define SPR_ADDITIVE 1
#define SPR_INDEXALPHA 2
#define SPR_ALPHTEST 3

#define TEX_TYPE_NONE 0
#define TEX_TYPE_ALPHA 1
#define TEX_TYPE_LUM 2
#define TEX_TYPE_ALPHA_GRADIENT 3
#define TEX_TYPE_RGBA 4

#pragma pack(1)
typedef struct gltexture_s
{
	int texnum;
	short servercount;
	short paletteIndex;
	int width;
	int height;
	qboolean mipmap;
	char identifier[64];
}gltexture_t;
#pragma pack()

typedef enum
{
	GLT_SYSTEM,
	GLT_DECAL,
	GLT_HUDSPRITE,
	GLT_STUDIO,
	GLT_WORLD,
	GLT_SPRITE,
}
GL_TEXTURETYPE;

typedef struct
{
	int origin[2];
	int width;
	int height;
}
dspriteframe_t;

typedef struct
{
	int numframes;
}
dspritegroup_t;

typedef struct
{
	float interval;
}
dspriteinterval_t;

typedef enum
{
	SPR_SINGLE = 0,
	SPR_GROUP
}
spriteframetype_t;

typedef struct
{
	spriteframetype_t type;
}
dspriteframetype_t;

#define IDSPRITEHEADER (('P' << 24) + ('S' << 16) + ('D' << 8) + 'I')

typedef struct mspriteframe_s
{
	int width;
	int height;
	float up, down, left, right;
	int gl_texturenum;
}
mspriteframe_t;

typedef struct
{
	int numframes;
	float *intervals;
	mspriteframe_t *frames[1];
}
mspritegroup_t;

typedef struct
{
	spriteframetype_t type;
	mspriteframe_t *frameptr;
}
mspriteframedesc_t;

typedef struct msprite_s
{
	short type;
	short texFormat;
	int maxwidth;
	int maxheight;
	int numframes;
	int paloffset;
	float beamlength;
	void *cachespot;
	mspriteframedesc_t frames[1];
}msprite_t;

#define NL_PRESENT 0
#define NL_NEEDS_LOADED 1
#define NL_UNREFERENCED 2
#define NL_CLIENT 3

typedef struct
{
	qboolean fog;
	vec4_t color;
	float start;
	float end;
	float density;
	float fresnel;
}water_parm_t;

typedef struct
{
  int keynum;
  int topcolor;
  int bottomcolor;
  model_t *model;
  char name[260];
  int index;
  int source;
  int width;
  int height;
  int gl_index;
}skin_t;

typedef struct
{
	int r, g, b;
}mcolor24_t;

typedef struct sfx_s
{
	char name[MAX_QPATH];
	cache_user_t cache;
}sfx_t;

typedef struct refdef_s
{
	vrect_t vrect;
	vrect_t aliasvrect;
	int vrectright;
	int vrectbottom;
	int aliasvrectright;
	int aliasvrectbottom;
	float vrectrightedge;
	float fvrectx;
	float fvrecty;
	float fvrectx_adj;
	float fvrecty_adj;
	int vrect_x_adj_shift20;
	int vrectright_adj_shift20;
	float fvrectright_adj;
	float fvrectbottom_adj;
	float fvrectright;
	float fvrectbottom;
	float horizontalFieldOfView;
	float xOrigin;
	float yOrigin;
	vec3_t vieworg;
	vec3_t viewangles;
	color24 ambientlight;
	qboolean onlyClientDraws;
}refdef_t;

typedef struct msurface_s msurface_t;

typedef vec_t matrix3x4[3][4];

typedef DWORD cachewad_t;

typedef struct
{
	int (*LoadTGA)(const char *szFilename, byte *buffer, int bufferSize, int *width, int *height);
	void (*R_Clear)(void);
	void (*R_ForceCVars)(qboolean mp);
	void (*R_RenderView)(void);
	void (*R_RenderScene)(void);
	void (*R_NewMap)(void);
	void (*R_DrawEntitiesOnList)(void);
	void (*R_DrawTEntitiesOnList)(int onlyClientDraw);
	void (*R_DrawWorld)(void);
	void (*R_SetupFrame)(void);
	void (*R_SetFrustum)(void);
	void (*R_SetupGL)(void);
	void (*R_DrawSkyChain)(msurface_t *s);
	void (*R_DrawSkyBox)(void);
	qboolean (*R_CullBox)(vec3_t mins, vec3_t maxs);
	void (*GL_BuildLightmaps)(void);
	void (*GL_Bind)(int texnum);
	void (*GL_SelectTexture)(GLenum target);
	void (*GL_DisableMultitexture)(void);
	void (*GL_EnableMultitexture)(void);
	void (*GL_BeginRendering)(int *x, int *y, int *width, int *height);
	void (*GL_EndRendering)(void);
	int (*GL_LoadTexture)(char *identifier, int textureType, int width, int height, byte *data, qboolean mipmap, int iType, byte *pPal);
	int (*GL_LoadTexture2)(char *identifier, int textureType, int width, int height, byte *data, qboolean mipmap, int iType, byte *pPal, int filter);
	void (*EmitWaterPolys)(msurface_t *fa, int direction);
	void (*BuildSurfaceDisplayList)(msurface_t *fa);
	void (*R_DrawSequentialPoly)(msurface_t *s, int face);
	void (*R_RecursiveWorldNode)(mnode_t *node);
	texture_t *(*R_TextureAnimation)(msurface_t *fa);
	void (*R_RenderDynamicLightmaps)(msurface_t *fa);
	void (*R_DrawDecals)(qboolean bMultitexture);
	texture_t *(*Draw_DecalTexture)(int index);
	void (*R_AllocObjects)(int nMax);
	void (*Draw_MiptexTexture)(cachewad_t *wad, byte *data);
	void (*R_BuildLightMap)(msurface_t *psurf, byte *dest, int stride);
	void *(*Mem_Malloc)(size_t);
	void *(*Hunk_Alloc)(size_t);
	void (*R_DecalMPoly)(float *v, texture_t *ptexture, msurface_t *psurf, int vertCount);
	void (*FreeFBObjects)(void);//only above 5953
	void (*R_MarkLeaves)(void);
	void (*R_DrawBrushModel)(cl_entity_t *e);
	float (*GlowBlend)(cl_entity_t *e);
	void (*R_DrawSpriteModel)(cl_entity_t *ent);
	int (*CL_FxBlend)(cl_entity_t *ent);
	mspriteframe_t *(*R_GetSpriteFrame)(msprite_t *spr, int frame);
	void (*R_GetSpriteAxes)(cl_entity_t *entity, int type, float *vforwrad, float *vright, float *vup);
	void (*R_SpriteColor)(mcolor24_t *col, cl_entity_t *entity, int renderamt);
}ref_funcs_t;

typedef struct
{
	//Engine Studio
	void (*R_GLStudioDrawPoints)(void);
	studiohdr_t *(*R_LoadTextures)(struct model_s *psubmodel);
	void (*R_LightStrength)(int bone, float *vert, float (*light)[4]);
	void (*R_StudioLighting)(float *lv, int bone, int flags, vec3_t normal);
	void (*R_StudioSetupSkin)(studiohdr_t *ptexturehdr, int index);
	void (*R_LightLambert)(float (*light)[4], float *normal, float *src, float *lambert);
	void (*BuildNormalIndexTable)(void);
	void (*BuildGlowShellVerts)(vec3_t *pstudioverts, auxvert_t *pauxverts);
	void (*R_StudioChrome)(int *pchrome, int bone, vec3_t normal);
	void (*R_StudioRenderFinal)(void);

	//Studio API
	void (*studioapi_StudioDrawPoints)(void);
	void (*studioapi_StudioSetupLighting)(alight_t *plighting);
	void (*studioapi_SetupRenderer)(int rendermode);
	void (*studioapi_RestoreRenderer)(void);
}studio_funcs_t;

typedef struct
{
	GLuint (*R_CompileShader)(const char *vscode, const char *fscode, const char *vsfile, const char *fsfile);
	void (*GL_UseProgram)(GLuint program);
	void (*GL_EndProgram)(void);
	GLuint (*GL_GetUniformLoc)(GLuint program, const char *name);
	GLuint (*GL_GetAttribLoc)(GLuint program, const char *name);
	void (*GL_Uniform1i)(GLuint loc, int v0);
	void (*GL_Uniform2i)(GLuint loc, int v0, int v1);
	void (*GL_Uniform3i)(GLuint loc, int v0, int v1, int v2);
	void (*GL_Uniform4i)(GLuint loc, int v0, int v1, int v2, int v3);
	void (*GL_Uniform1f)(GLuint loc, float v0);
	void (*GL_Uniform2f)(GLuint loc, float v0, float v1);
	void (*GL_Uniform3f)(GLuint loc, float v0, float v1, float v2);
	void (*GL_Uniform4f)(GLuint loc, float v0, int v1, int v2, int v3);
	void (*GL_VertexAttrib3f)(GLuint index, float x, float y, float z);
	void (*GL_VertexAttrib3fv)(GLuint index, float *v);
	void (*GL_MultiTexCoord2f)(GLenum target, float s, float t);
	void (*GL_MultiTexCoord3f)(GLenum target, float s, float t, float r);
}shaderapi_t;

typedef struct
{
	//common
	int (*R_GetDrawPass)(void);
	int (*R_GetSupportExtension)(void);
	//water
	void (*R_SetWaterParm)(water_parm_t *parm);
	//studio
	void (*R_GLStudioDrawPointsEx)(void);
	entity_state_t *(*R_GetPlayerState)(int index);
	//refdef
	void (*R_PushRefDef)(void);
	void (*R_UpdateRefDef)(void);
	void (*R_PopRefDef)(void);
	float *(*R_GetSavedViewOrg)(void);
	refdef_t *(*R_GetRefDef)(void);
	//shadow
	void (*R_CreateShadowLight)(cl_entity_t *entity, vec3_t angles, float radius, float fard, float scale, int texscale);
	//texture
	GLuint (*R_GLGenTexture)(int w, int h);
	byte *(*R_GetTexLoaderBuffer)(int *bufsize);
	int (*R_LoadTextureEx)(const char *path, const char *name, int *width, int *height, GL_TEXTURETYPE type, qboolean mipmap, qboolean ansio);
	int (*GL_LoadTextureEx)(const char *identifier, GL_TEXTURETYPE textureType, int width, int height, byte *data, qboolean mipmap, qboolean ansio);
	gltexture_t *(*R_GetCurrentGLTexture)(void);
	//capture screen
	byte *(*R_GetSCRCaptureBuffer)(int *bufsize);
	//3dsky
	void (*R_Add3DSkyEntity)(cl_entity_t *ent);
	void (*R_Setup3DSkyModel)(void);
	void (*R_Finish3DSkyModel)(void);
	//2d postprocess
	void (*R_BeginFXAA)(int w, int h);
	void (*R_BeginDrawRoundRect)(int centerX, int centerY, float radius, float blurdist);
	void (*R_BeginDrawHudMask)(int r, int g, int b);
	//cloak
	void (*R_RenderCloakTexture)(void);
	//3dhud
	int (*R_Get3DHUDTexture)(void);
	void (*R_Draw3DHUDQuad)(int x, int y, int left, int top);
	void (*R_BeginDrawTrianglesInHUD_Direct)(int x, int y);
	void (*R_BeginDrawTrianglesInHUD_FBO)(int x, int y, int left, int top);
	void (*R_FinishDrawTrianglesInHUD)(void);
	void (*R_BeginDrawHUDInWorld)(int texid, int w, int h);
	void (*R_FinishDrawHUDInWorld)(void);
	//shader
	shaderapi_t ShaderAPI;
}ref_export_t;

extern ref_export_t *gpRefExports;
extern ref_funcs_t *gpRefFuncs;
extern studio_funcs_t *gpStudioFuncs;

#define r_draw_normal 0
#define r_draw_reflect 1
#define r_draw_refract 2
#define r_draw_shadow 3
#define r_draw_3dhud 4
#define r_draw_shadowscene 5
#define r_draw_3dsky 6
#define r_draw_hudinworld 7

#define r_ext_fbo (1<<0)
#define r_ext_msaa (1<<1)
#define r_ext_water (1<<2)
#define r_ext_shader (1<<3)
#define r_ext_shadow (1<<4)

enum
{
	kRenderFxCloak = 22,
	kRenderFxShadow,
	kRenderFxFireLayer
};

typedef void (*xcommand_t)(void);

typedef struct
{
	int (*GetHUDFov)(void);
	float *(*GetPunchAngles)(void);
	pfnUserMsgHook (*HookUserMsg)(char *pMsgName, pfnUserMsgHook pfnHook);
	xcommand_t (*HookCmd)(char *cmd_name, xcommand_t newfuncs);
}bte_funcs_t;