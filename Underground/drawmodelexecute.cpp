#include "definitions.h"

IMaterial* ICreateMaterial(bool shouldIgnoreZ, bool isWireframe = false)
{
	static int created = 0;


	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"VGUI/white\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"1\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"1\"\
		\n\t\"$wireframe\" \"%i\"\
		\n}\n"
	};

	static const char baseType[] = "VertexLitGeneric";


	// this holds the material
	char material[512];

	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);


	// make a unique name for our material
	char name[512];

	sprintf_s(name, sizeof(name), "custom_material_%i.vmt", created);


	++created;


	// construct keyvalues based on the basetype
	KeyValues *keyValues = new KeyValues(baseType);


	// load material into our instance of keyvalues
	keyValues->LoadFromBuffer(name, material, 0, 0);


	// create the actual material
	IMaterial *createdMaterial = MaterialSystemSDK->CreateMaterial(name, keyValues);


	createdMaterial->IncrementReferenceCount();


	return createdMaterial;
}

void ForceMaterial(Color Color,float Alpha, IMaterial *Material, bool bColorC = true, bool bForceMat = true)
{
	if (bColorC)
	{
		float Temp[3] = { Color.r(), Color.g(), Color.b() };
		RenderViewSDK->SetBlend(Alpha);
		RenderViewSDK->SetColorModulation(Temp);
	}

	if (bForceMat)
		ModelRenderSDK->ForcedMaterialOverride(Material);

}

void PutLight(float r, float g, float b, const ModelRenderInfo_t &pInfo, float radius,bool OnlyOn = false)
{
	dlight_t *dl2 = EffectsSDK->CL_AllocElight(pInfo.entity_index);
	if (dl2)
	{
		dl2->color.r = (byte)r;
		dl2->color.g = (byte)g;
		dl2->color.b = (byte)b;
		dl2->color.exponent = 10;

		Vector lightlocation = pInfo.origin;
		lightlocation.z += 50;

		dl2->color.exponent = 10.0f;
		dl2->m_InnerAngle = 180 - (rand() % 360);
		dl2->m_OuterAngle = 0;
		dl2->minlight = 1024.0f;
		dl2->radius = radius;
		dl2->decay = dl2->radius * 1.5f;
		dl2->key = pInfo.entity_index;
		dl2->m_Direction = lightlocation;
		dl2->origin = pInfo.origin;
		dl2->die = Globals->curtime + 0.05f;
	}


	if (OnlyOn) return;

	dlight_t *dl = EffectsSDK->CL_AllocDlight(pInfo.entity_index);
	if (dl)
	{
		dl->color.r = (byte)r;
		dl->color.g = (byte)g;
		dl->color.b = (byte)b;
		dl->color.exponent = 10;

		Vector lightlocation = pInfo.origin;
		lightlocation.z += 50;

		dl->color.exponent = 10.0f;
		dl->m_InnerAngle = 180 - (rand() % 360);
		dl->m_OuterAngle = 0;
		dl->minlight = 1024.0f;
		dl->radius = radius;
		dl->decay = dl->radius * 1.5f;
		dl->key = pInfo.entity_index;
		dl->m_Direction = lightlocation;
		dl->origin = pInfo.origin;
		dl->die = Globals->curtime + 0.05f;
	}
}

typedef void(__thiscall*DrawModelDrawExecuteFn)(void*, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
void __fastcall new_DrawModelExecute(void*thisptr, int edx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static IMaterial *In = ICreateMaterial(true), *Out = ICreateMaterial(false), *Wire = ICreateMaterial(true, true);

	std::string ModelName = ModelInfoSDK->GetModelName(pInfo.pModel);

	bool Active = false;
	C_BasePlayer *OthersPlayers = (C_BasePlayer*)(EntityListSDK->GetClientEntity(pInfo.entity_index));
	if (!OthersPlayers) return;

	Vector mins, maxs;
	OthersPlayers->GetRenderBounds(mins, maxs);

	if (ModelName.find("models/player") != std::string::npos)
	{
		if (OthersPlayers->GetHealth() > 0)
		{
			if ((OthersPlayers->GetHealth() < 500) && (OthersPlayers->GetRenderColor().a >= 255.f))
			{
				if (OthersPlayers->GetTeamNumber() == 2)
				{
					//PutLight(255.0f, 0.0f, 0, pInfo, maxs.Length());
					ForceMaterial(Color(225, 0, 0, 0), 1.0f, In);
				}
				else if (OthersPlayers->GetTeamNumber() == 3)
				{
					//PutLight(0.0f, 0.0f, 255.0f, pInfo, maxs.Length());
					ForceMaterial(Color(0, 0, 225, 0), 1.0f, In);
				}
				else
				{
					//PutLight(255.0f, 255.0f, 255.0f, pInfo, maxs.Length());
					ForceMaterial(Color(0.5f, 0.5f, 0.5f) , 1.0f, In);
				}
			}
			else
			{
				//PutLight(255.0f, 255.0f, 255.0f, pInfo, maxs.Length());
				ForceMaterial(Color(0.5f, 0.5f, 0.5f) , 1.0f, In);
			}

			ModelRender->GetMethod<DrawModelDrawExecuteFn>(19)(thisptr, state, pInfo, pCustomBoneToWorld);

			if ((OthersPlayers->GetHealth() < 500) && (OthersPlayers->GetRenderColor().a >= 255.f))
			{
				if (OthersPlayers->GetTeamNumber() == 2)
				{
					ForceMaterial(Color(225, 0, 0, 225), 1.0f, Out);
				}
				else if (OthersPlayers->GetTeamNumber() == 3)
				{
					ForceMaterial(Color(0, 0, 225, 225), 1.0f, Out);
				}
				else
				{
					ForceMaterial(Color(1.0f, 1.0f, 1.0f) , 1.0f, Out);
				}
			}
			else
			{
				ForceMaterial(Color(1.0f, 1.0f, 1.0f) , 1.0f, Out);
			}
		}
		else
		{
			ForceMaterial(Color(0.5f, 0.5f, 0.5f) , 1.0f, In);

			ModelRender->GetMethod<DrawModelDrawExecuteFn>(19)(thisptr, state, pInfo, pCustomBoneToWorld);

			ForceMaterial(Color(1.0f, 1.0f, 1.0f) , 1.0f, Out);
		}

		Active = true;
	}
	else if (ModelName.find("models/weapons/w_") != std::string::npos)
	{
		PutLight(255.0f, 255.0f, 255.0f, pInfo, maxs.Length());
		ForceMaterial(Color(0.5f, 0.5f, 0.5f) , 1.0f, In);

		ModelRender->GetMethod<DrawModelDrawExecuteFn>(19)(thisptr, state, pInfo, pCustomBoneToWorld);

		ForceMaterial(Color(1.0f, 1.0f, 1.0f) , 1.0f, Out);

		Active = true;
	}

	ModelRender->GetMethod<DrawModelDrawExecuteFn>(19)(thisptr, state, pInfo, pCustomBoneToWorld);

	if (Active)
	{
		ForceMaterial(Color(0, 0, 0, 0), 0, 0);
	}


}