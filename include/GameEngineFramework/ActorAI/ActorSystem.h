#ifndef _ACTOR_AI_SYSTEM__
#define _ACTOR_AI_SYSTEM__

#include <GameEngineFramework/ActorAI/ActorStates.h>
#include <GameEngineFramework/ActorAI/GeneticPresets.h>

#include <GameEngineFramework/MemoryAllocation/PoolAllocator.h>

#include <GameEngineFramework/ActorAI/neurons/NeuralLayer.h>
#include <GameEngineFramework/ActorAI/neurons/WeightedLayer.h>
#include <GameEngineFramework/ActorAI/Genetics/Gene.h>

#include <GameEngineFramework/ActorAI/components/actor.h>
#include <GameEngineFramework/ActorAI/components/plant.h>

#include <thread>
#include <mutex>
#include <chrono>


class ENGINE_API ActorSystem {
    
public:
    
    ActorSystem();
    
    /// Create an actor and return its pointer.
    Actor* CreateActor(void);
    
    /// Destroy an actor.
    bool DestroyActor(Actor* actorPtr);
    
    /// Create a static actor plant and return its pointer.
    Plant* CreatePlant(void);
    
    /// Destroy a static actor plant.
    bool DestroyPlant(Plant* plantPtr);
    
    /// Set the player position in the simulation.
    void SetPlayerWorldPosition(glm::vec3 position);
    
    /// Get the player position in the simulation.
    glm::vec3 GetPlayerWorldPosition(void);
    
    /// Get the number of actors in the AI simulation.
    unsigned int GetNumberOfActors(void);
    
    /// Get an actor from the simulation by its index.
    Actor* GetActor(unsigned int index);
    
    /// Signal to the AI thread to update the simulation.
    void UpdateSendSignal(void);
    
    
    /// Set the update distance from the camera position.
    void SetActorUpdateDistance(float distance);
    
    
    /// Initiate the actor AI system. (called internally)
    void Initiate(void);
    
    /// Shutdown the actor AI system. (called internally)
    void Shutdown(void);
    
    /// Update the actors in the simulation. (called internally from another thread)
    void Update();
    
    /// Genetic entity definitions.
    GeneticPresets genomes;
    
private:
    
    // Current position of the player in the world
    glm::vec3 mPlayerPosition;
    
    // Distance beyond which the actors will no longer update
    float mActorUpdateDistance;
    
    // Threading
    std::thread* mActorSystemThread;
    std::mutex   mux;
    
    // Object pools
    PoolAllocator<Actor> mActors;
    PoolAllocator<Plant> mPlants;
    
};


#endif
