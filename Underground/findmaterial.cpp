#include "main.h"


typedef void *(__thiscall *FindMaterialFn)(void*, const char *pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);

void *__fastcall new_FindMaterial(void*thisptr, int edx, const char *pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix)
{
	IMaterial* pTemp = (IMaterial*)MaterialSystem->GetMethod<FindMaterialFn>(71)(thisptr, pMaterialName, pTextureGroupName, complain, pComplainPrefix);

	if ( pTemp )
	{
		if ( strstr( pTemp->GetName(), "skybox" ) )
		{
				pTemp->ColorModulate( 0.0f, 0.0f, 0.0f );
		}
	}

	for (MaterialHandle_t i = MaterialSystemSDK->FirstMaterial(); i != MaterialSystemSDK->InvalidMaterial(); i = MaterialSystemSDK->NextMaterial(i))
	{
		IMaterial *pMaterial = MaterialSystemSDK->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), TEXTURE_GROUP_WORLD))
		{
			pMaterial->AlphaModulate(0.5f);
			pMaterial->ColorModulate(0.5f, 0.5f, 0.5f);
			pMaterial->SetMaterialVarFlag(MaterialVarFlags_t::MATERIAL_VAR_VERTEXALPHA, true);
		}
	}

	return pTemp;
}