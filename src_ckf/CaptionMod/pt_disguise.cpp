#include <metahook.h>
#include "exportfuncs.h"
#include "engfuncs.h"
#include "client.h"
#include "cvar.h"
#include "util.h"
#include "studio_util.h"
#include "particle.h"
#include "StudioModelRenderer.h"
#include "GameStudioModelRenderer.h"

extern cl_entity_t ent;

qboolean EV_IsLocal( int idx );

class CDisguiseSmoke : public CPartSystemEntity
{
public:
	CDisguiseSmoke(){}
	void Init(int parts, int childs, cl_entity_t *entity, int team)
	{
		CPartSystemEntity::Init(PS_DisguiseSmoke, parts, childs, entity);
		Reset(team);

		m_mod = entity->model;
	}
	void Reset(int team)
	{
		m_team = team;
		SetDie(2.0f);
	}
	virtual void Movement(part_t *p, float *org)
	{
		CALC_FRACTION(p);

		g_StudioRenderer.m_pCurrentEntity = m_entity;
		entity_bones_t *bones = g_StudioRenderer.GetEntityBones();
		if(bones)
		{
			VectorTransform(p->org, bones->m_bonetransform[p->bone], org);
			VectorMA(org, frac2, p->vel, org);
		}
	}
	virtual void Render(part_t *p, float *org)
	{
		if(m_entity->player && EV_IsLocal(m_entity->index) && !CL_IsThirdPerson())
			return;

		CALC_FRACTION(p);

		ent.curstate.rendermode = kRenderTransAlpha;
		ent.curstate.renderfx = kRenderFxNone;
		if(m_team == 0)
		{
			ent.curstate.rendercolor.r = p->col[0] * frac + 130 * frac2;
			ent.curstate.rendercolor.g = p->col[1] * frac + 108 * frac2;
			ent.curstate.rendercolor.b = p->col[2] * frac + 108 * frac2;
		}
		else
		{
			ent.curstate.rendercolor.r = p->col[0] * frac + 185 * frac2;
			ent.curstate.rendercolor.g = p->col[1] * frac + 200 * frac2;
			ent.curstate.rendercolor.b = p->col[2] * frac + 218 * frac2;
		}
		if(frac > 0.66)
			ent.curstate.renderamt = p->col[3] * frac2 / 0.33;
		else
			ent.curstate.renderamt = p->col[3] * frac / 0.66;

		ent.curstate.scale = .06f * frac2 + .06f;
		ent.curstate.fuser1 = 1;
		ent.curstate.frame = 0;

		VectorCopy(org, ent.origin);
		ent.angles[2] = frac2*p->rot;

		R_DrawTGASprite(&ent, &g_texRocketSmoke[p->modn]);
	}
	virtual void Update(void)
	{
		if(m_entity->model != m_mod)
		{
			RemoveInvalid();
			m_mod = m_entity->model;
		}
		if(!m_dead && m_entity->model)
		{
			AddParticle();
		}
	}
	void RemoveInvalid(void)
	{
		if(!m_entity->model)
			return;

		studiohdr_t *pstudiohdr = (studiohdr_t *)IEngineStudio.Mod_Extradata(m_entity->model);

		for(int i = 0;i < m_part.size(); ++i)
		{
			if(!m_part[i].free)
			{
				if(m_part[i].bone > pstudiohdr->numbones)
					m_part[i].die = 0;
			}
		}
	}
	void AddParticle(void)
	{
		if(m_entity->player && EV_IsLocal(m_entity->index) && !CL_IsThirdPerson())
			return;

		part_t *p = AllocParticle();

		p->modn = RANDOM_LONG(0,4);
		if(m_team == 0)
		{
			p->col[0] = RANDOM_LONG(180, 232);
			p->col[1] = RANDOM_LONG(180, 205);
			p->col[2] = RANDOM_LONG(180, 188);
		}
		else
		{
			p->col[0] = RANDOM_LONG(180, 188);
			p->col[1] = RANDOM_LONG(180, 206);
			p->col[2] = RANDOM_LONG(180, 232);
		}
		p->col[3] = RANDOM_LONG(128, 200);

		//generate position for this model
		{
			studiohdr_t *pstudiohdr = (studiohdr_t *)IEngineStudio.Mod_Extradata(m_entity->model);
			if(pstudiohdr && pstudiohdr->numhitboxes > 0)
			{
				float rfrac;
				mstudiobbox_t *pbboxes = (mstudiobbox_t *)((byte *)pstudiohdr + pstudiohdr->hitboxindex);
				int i = RANDOM_LONG(0, pstudiohdr->numhitboxes-1);
				p->bone = pbboxes[i].bone;
				rfrac = RANDOM_FLOAT(0, 1);
				p->org[0] = pbboxes[i].bbmax[0] * rfrac + pbboxes[i].bbmin[0] * (1-rfrac);
				rfrac = RANDOM_FLOAT(0, 1);
				p->org[1] = pbboxes[i].bbmax[1] * rfrac + pbboxes[i].bbmin[1] * (1-rfrac);
				rfrac = RANDOM_FLOAT(0, 1);
				p->org[2] = pbboxes[i].bbmax[2] * rfrac + pbboxes[i].bbmin[2] * (1-rfrac);
			}
		}

		p->vel[0] = RANDOM_FLOAT(-12, 12);
		p->vel[1] = RANDOM_FLOAT(-12, 12);
		p->vel[2] = RANDOM_FLOAT(-8, 8);
		p->life = 1.4;
		p->die = g_flClientTime + p->life;
	}
public:
	int m_team;
	model_t *m_mod;
};

class CDisguiseFlash : public CPartSystemEntity
{
public:
	CDisguiseFlash(){}
	void Init(int parts, int childs, cl_entity_t *entity)
	{
		CPartSystemEntity::Init(PS_DisguiseFlash, parts, childs, entity);
	}
	virtual void Movement(part_t *p, float *org)
	{
		R_ParticleMovementEntity(p, org, m_entity);
	}
	virtual void Render(part_t *p, float *org)
	{
		if(m_entity->player && EV_IsLocal(m_entity->index) && !CL_IsThirdPerson())
			return;

		CALC_FRACTION(p);

		ent.curstate.rendermode = kRenderTransAdd;
		ent.curstate.rendercolor.r = p->col[0];
		ent.curstate.rendercolor.g = p->col[1];
		ent.curstate.rendercolor.b = p->col[2];
		ent.curstate.renderamt = p->col[3] * frac;

		ent.curstate.scale = 0.1*frac;
		ent.curstate.fuser1 = 0.1;
		ent.curstate.frame = 0;

		VectorCopy(org, ent.origin);
		ent.angles[2] = 0;

		R_DrawTGASprite(&ent, &g_texBrightGlowY);
	}
	virtual void Update(void)
	{
		if(!m_dead && m_parent->m_die - g_flClientTime < 1.0 && GetActivePartCount() < 35)
		{
			AddParticle();
		}
	}
	void AddParticle(void)
	{
		if(m_entity->player && EV_IsLocal(m_entity->index) && !CL_IsThirdPerson())
			return;

		part_t *p = AllocParticle();

		CDisguiseSmoke *parent = (CDisguiseSmoke *)m_parent;
		if(parent->m_team == 0)
		{
			p->col[0] = RANDOM_LONG(236, 255);
			p->col[1] = RANDOM_LONG(33, 96);
			p->col[2] = 33;
		}
		else
		{
			p->col[0] = 33;
			p->col[1] = RANDOM_LONG(11, 215);
			p->col[2] = RANDOM_LONG(236, 255);
		}
		p->col[3] = RANDOM_LONG(64, 233);

		p->org[0] = RANDOM_FLOAT(-12, 12);
		p->org[1] = RANDOM_FLOAT(-12, 12);
		p->org[2] = RANDOM_FLOAT(-20, 24);
		p->life = 0.4;
		p->die = g_flClientTime + p->life;
	}
private:
};

void R_DisguiseSmoke(cl_entity_t *pEntity, int iTeam)
{
	CDisguiseSmoke *pOldSmoke = (CDisguiseSmoke *)R_FindPartSystem(PS_DisguiseSmoke, pEntity);;

	if(pOldSmoke)
	{
		pOldSmoke->SetDead(0);
		pOldSmoke->Reset(iTeam-1);
		return;
	}

	CDisguiseSmoke *pSmoke = new CDisguiseSmoke;
	pSmoke->Init(150, 1, pEntity, iTeam-1);

	CDisguiseFlash *pFlash = new CDisguiseFlash;
	pFlash->Init(35, 0, pEntity);

	pSmoke->AddChild(pFlash);
	R_AddPartSystem(pSmoke);
}