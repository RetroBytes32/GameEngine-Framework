#include <GameEngineFramework/Engine/EngineSystems.h>
#include <GameEngineFramework/functions.h>

#include <GameEngineFramework/plugins.h>


void Shutdown(void) {
    
    if (chunkManager.world.doGenerateChunks) 
        chunkManager.SaveWorld();
    
    return;
}
