#include "main.h"

#ifndef C_VMT_H
#define C_VMT_H
// ----------------------------------------------------------------
// Class: VMTManager
// ----------------------------------------------------------------
// Hooks virtual functions by replacing the vtable pointer from an instance.
class C_VMT 
{
	// Forbid copy constructing and assignment.
	C_VMT(const C_VMT&);
	C_VMT& operator= (const C_VMT&);

public:

	C_VMT(void* inst, size_t offset = 0, uint vfuncs = 0);
	~C_VMT();

	// Hooks a function by index.
	FORCEINLINE void HookMethod(void* newfunc, size_t index) 
	{
		assert(index < vtable_function_count_);
		_array[index+3] = newfunc;
	}

	// Unhooks a function by index.
	FORCEINLINE void UnhookMethod(size_t index) 
	{
		assert(index < vtable_function_count_);
		_array[index+3] = old_vtable_[index];
	}

	// Manage the hooks.
	FORCEINLINE void Unhook() { *vtable_ = old_vtable_; }
	FORCEINLINE void Rehook() { *vtable_ = _array + 3; }
	FORCEINLINE bool Hooked() const { return *vtable_ != old_vtable_; }
	FORCEINLINE void EraseHooks() 
	{
		for (uint i = 0; i < vtable_function_count_; ++i) 
		{
			_array[i+3] = vtable_[i];
		}
	}

	FORCEINLINE uint NumFuncs() const
	{
		return vtable_function_count_;
	}

	// If the instance is somehow destroyed
	// before you get a chance to unhook it
	// or destruct this hook object, call this.
	// It'll prevent the destructor from crashing.
	FORCEINLINE void Poof() { vtable_ = 0; }

	// Get the original function.
	// Use a function prototype
	// for the template argument to make it very easy to call this function.
	// Example syntax:
	// hook.GetMethod<bool (__thiscall*)( void*, int )>( 12 )( inst, arg );
	template< typename Fn >
	FORCEINLINE Fn GetMethod(size_t index) const
	{
		assert(index < vtable_function_count_);
		return (Fn) old_vtable_[ index ];
	}

	FORCEINLINE void _set_guard(size_t S) 
	{
		_array[1] = reinterpret_cast<void*>(S);
	}

	FORCEINLINE size_t _get_guard() const { return (size_t) _array[1]; }
	FORCEINLINE void _set_backptr(void* ptr ) { _array[0] = ptr; }
	FORCEINLINE void* _get_backptr() const { return _array[0]; }


	void*** vtable_;
	void**  old_vtable_;
	void**  _array;
	uint  vtable_function_count_;
};

extern C_VMT *Client;
extern C_VMT *EntityList;
extern C_VMT *Prediction;
extern C_VMT *GameMovement;
extern C_VMT *Engine;
extern C_VMT *Input;
extern C_VMT *Cvar;
extern C_VMT *ModelInfo;
extern C_VMT *EngineTrace;
extern C_VMT *PhysicsSurfaceProps;
extern C_VMT *Panel;
extern C_VMT *Surface;
extern C_VMT *RenderView;
extern C_VMT *MaterialSystem;
extern C_VMT *ModelRender;
extern C_VMT *GameEventManager;
extern C_VMT *Effects;
extern C_VMT *ClientMode;

#endif