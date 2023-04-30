#ifndef SCRIPTING_SYSTEM
#define SCRIPTING_SYSTEM

#include "../std/poolallocator.h"

#include "components/script.h"


class ScriptSystem {
    
public:
    
    
    ScriptSystem();
    ~ScriptSystem();
    
    Script* CreateScript(void);
    bool DestroyScript(Script* scriptPtr);
    
    
    void Update(void);
    
private:
    
    PoolAllocator<Script> script;
    
};

#endif
