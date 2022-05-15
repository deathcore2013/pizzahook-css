#include "vmt.h"

C_VMT *Client;
C_VMT *EntityList;
C_VMT *Prediction;
C_VMT *GameMovement;
C_VMT *Engine;
C_VMT *Input;
C_VMT *Cvar;
C_VMT *ModelInfo;
C_VMT *EngineTrace;
C_VMT *PhysicsSurfaceProps;
C_VMT *Panel;
C_VMT *Surface;
C_VMT *RenderView;
C_VMT *MaterialSystem;
C_VMT *ModelRender;
C_VMT *GameEventManager;
C_VMT *Effects;
C_VMT *ClientMode;

C_VMT::C_VMT(void* inst, size_t offset, uint vfuncs) 
{
	vtable_ = make_ptr<void***>(inst, offset);
	old_vtable_ = *vtable_;
	// Count vfuncs ourself if needed
	if ( !vfuncs ) vfuncs = CountFuncs( old_vtable_ );
	vtable_function_count_ = vfuncs;

	_array = reinterpret_cast<void**>(malloc((vfuncs+3)*sizeof(void*) ));
	// Initialize RTTI pointer (should there be one)
	_array[2] = old_vtable_[-1];
	// Copy over the other vfuncs
	for (uint i = 0; i < vfuncs; ++i ) _array[i+3] = old_vtable_[i];
	// Hook it
	*vtable_ = _array+3;
}

C_VMT::~C_VMT() 
{
	if (vtable_) Unhook();
	free(_array);
}
