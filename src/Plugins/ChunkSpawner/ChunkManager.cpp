#include <GameEngineFramework/Plugins/ChunkSpawner/ChunkManager.h>


ChunkManager::ChunkManager() : 
    
    renderDistance(8),
    
    chunkSize(32),
    
    worldSeed(100),
    
    ChunkCounterX(0),
    ChunkCounterZ(0)
{
    
    return;
}

void ChunkManager::Initiate(void) {
    
    // Source meshes for world construction
    
    glm::vec3 normalUp(0, 1, 0);
    
    Engine.meshes.wallHorizontal->SetNormals(normalUp);
    Engine.meshes.wallVertical->SetNormals(normalUp);
    
    Engine.meshes.grassHorz->SetNormals(normalUp);
    Engine.meshes.grassVert->SetNormals(normalUp);
    
    Engine.meshes.stemHorz->SetNormals(normalUp);
    Engine.meshes.stemVert->SetNormals(normalUp);
    
    Engine.meshes.log->GetSubMesh(2, subMeshTree);
    
    Engine.meshes.wallHorizontal->GetSubMesh(0, subMeshWallHorz);
    Engine.meshes.wallVertical  ->GetSubMesh(0, subMeshWallVert);
    
    Engine.meshes.grassHorz->GetSubMesh(0, subMeshGrassHorz);
    Engine.meshes.grassVert->GetSubMesh(0, subMeshGrassVert);
    
    Engine.meshes.stemHorz->GetSubMesh(0, subMeshStemHorz);
    Engine.meshes.stemVert->GetSubMesh(0, subMeshStemVert);
    
    watermesh     = Engine.Create<Mesh>();
    watermaterial = Engine.Create<Material>();
    
    watermesh->isShared     = true;
    watermaterial->isShared = true;
    
    return;
}


Chunk ChunkManager::CreateChunk(float x, float y) {
    Chunk chunk;
    
    chunk.x = x;
    chunk.y = y;
    
    chunk.gameObject = Engine.Create<GameObject>();
    chunk.staticObject = Engine.Create<GameObject>();
    
    // Add renderers
    chunk.gameObject->AddComponent( Engine.CreateComponent<MeshRenderer>() );
    chunk.staticObject->AddComponent( Engine.CreateComponent<MeshRenderer>() );
    
    MeshRenderer* chunkRenderer = chunk.gameObject->GetComponent<MeshRenderer>();
    MeshRenderer* staticRenderer = chunk.staticObject->GetComponent<MeshRenderer>();
    
    Engine.sceneMain->AddMeshRendererToSceneRoot( chunkRenderer, RENDER_QUEUE_GEOMETRY );
    Engine.sceneMain->AddMeshRendererToSceneRoot( staticRenderer, RENDER_QUEUE_GEOMETRY );
    
    // Chunk renderer
    
    Transform* chunkTransform = chunk.gameObject->GetComponent<Transform>();
    
    chunkTransform->position = glm::vec3( x, 0, y);
    chunkTransform->scale = glm::vec3( 1, 1, 1 );
    
    chunkRenderer->mesh = Engine.Create<Mesh>();
    chunkRenderer->mesh->isShared = false;
    chunkRenderer->EnableFrustumCulling();
    
    chunkRenderer->material = Engine.Create<Material>();
    chunkRenderer->material->isShared = false;
    
    chunkRenderer->material->diffuse = Colors.gray;
    chunkRenderer->material->ambient = Colors.MakeGrayScale(0.2f);
    
    chunkRenderer->material->shader = Engine.shaders.color;
    
    // Static renderer
    
    Transform* staticTransform = chunk.staticObject->GetComponent<Transform>();
    
    staticTransform->position = glm::vec3( x, 0, y);
    staticTransform->scale = glm::vec3( 1, 1, 1 );
    
    staticRenderer->mesh = Engine.Create<Mesh>();
    staticRenderer->mesh->isShared = false;
    staticRenderer->EnableFrustumCulling();
    
    staticRenderer->material = Engine.Create<Material>();
    staticRenderer->material->isShared = false;
    staticRenderer->material->DisableCulling();
    
    staticRenderer->material->diffuse = Colors.gray;
    staticRenderer->material->ambient = Colors.MakeGrayScale(0.2f);
    
    staticRenderer->material->shader = Engine.shaders.color;
    
    
    // Generate perlin
    
    float heightField [ (chunkSize+1) * (chunkSize+1) ];
    glm::vec3 colorField  [ (chunkSize+1) * (chunkSize+1) ];
    
    Engine.SetHeightFieldValues(heightField, chunkSize+1, chunkSize+1, 0);
    Engine.SetColorFieldValues(colorField, chunkSize+1, chunkSize+1, Colors.white);
    
    unsigned int numberOfLayers = perlin.size();
    
    float min=0.0;
    
    for (unsigned int l=0; l < numberOfLayers; l++) {
        
        Perlin* perlinLayer = &perlin[l];
        
        min = 
        Engine.AddHeightFieldFromPerlinNoise(heightField, chunkSize+1, chunkSize+1, 
                                            perlinLayer->noiseWidth, 
                                            perlinLayer->noiseHeight, 
                                            perlinLayer->heightMultuplier, 
                                            x, y, worldSeed);
        
        continue;
    }
    
    // Generate water below water level
    if (min < (world.waterLevel - 32)) {
        
        chunk.waterObject = Engine.Create<GameObject>();
        
        chunk.waterObject->AddComponent( Engine.CreateComponent<MeshRenderer>() );
        MeshRenderer* waterRenderer = chunk.waterObject->GetComponent<MeshRenderer>();
        
        Engine.sceneMain->AddMeshRendererToSceneRoot( waterRenderer, RENDER_QUEUE_POSTGEOMETRY );
        
        // Water renderer
        Transform* waterTransform = chunk.waterObject->GetComponent<Transform>();
        
        waterTransform->position = glm::vec3( x, world.waterLevel, y);
        waterTransform->scale = glm::vec3( 32, 1, 32 );
        
        waterRenderer->mesh = Engine.meshes.plain;
        waterRenderer->EnableFrustumCulling();
        
        waterRenderer->material = Engine.Create<Material>();
        waterRenderer->material->isShared = false;
        waterRenderer->material->DisableCulling();
        waterRenderer->material->EnableBlending();
        
        waterRenderer->material->diffuse = Colors.blue * Colors.MakeGrayScale(0.4f);
        
        waterRenderer->material->shader = Engine.shaders.water;
        
    }
    
    // Generate terrain color
    
    Color colorLow;
    Color colorHigh;
    
    colorLow  = Colors.brown * Colors.green * Colors.MakeGrayScale(0.4f);
    colorHigh = Colors.brown * Colors.MakeGrayScale(0.2f);
    
    Engine.GenerateColorFieldFromHeightField(colorField, heightField, chunkSize+1, chunkSize+1, colorLow, colorHigh, 0.024f);
    
    Engine.GenerateWaterTableFromHeightField(heightField, chunkSize+1, chunkSize+1, 0);
    
    // Finalize chunk
    
    Engine.AddHeightFieldToMesh(chunkRenderer->mesh, heightField, colorField, chunkSize+1, chunkSize+1, 0, 0, 1, 1);
    
    chunkRenderer->mesh->Load();
    
    // Physics
    
    chunk.rigidBody = Physics.world->createRigidBody( rp3d::Transform::identity() );
    
    chunk.rigidBody->setAngularLockAxisFactor( rp3d::Vector3(0, 0, 0) );
    chunk.rigidBody->setLinearLockAxisFactor( rp3d::Vector3(0, 0, 0) );
    chunk.rigidBody->setType(rp3d::BodyType::STATIC);
    
    rp3d::Transform bodyTransform = rp3d::Transform::identity();
    bodyTransform.setPosition( rp3d::Vector3(x, 0, y) );
    chunk.rigidBody->setTransform(bodyTransform);
    
    // Generate a height field collider
    
    MeshCollider*  meshCollider = Physics.CreateHeightFieldMap(heightField, chunkSize+1, chunkSize+1);
    
    rp3d::Collider* bodyCollider = chunk.rigidBody->addCollider( meshCollider->heightFieldShape, rp3d::Transform::identity() );
    bodyCollider->setUserData( (void*)chunk.gameObject );
    bodyCollider->setCollisionCategoryBits((unsigned short)LayerMask::Ground);
    bodyCollider->setCollideWithMaskBits((unsigned short)CollisionMask::Entity);
    
    chunk.bodyCollider = bodyCollider;
    chunk.meshCollider = meshCollider;
    
    // Decorate chunk
    
    Decorate(chunk, x, y, staticRenderer->mesh);
    
    staticRenderer->mesh->Load();
    
    return chunk;
}

bool ChunkManager::DestroyChunk(Chunk& chunk) {
    
    Engine.Destroy<GameObject>( chunk.gameObject );
    Engine.Destroy<GameObject>( chunk.staticObject );
    
    MeshRenderer* chunkRenderer = chunk.gameObject->GetComponent<MeshRenderer>();
    MeshRenderer* staticRenderer = chunk.staticObject->GetComponent<MeshRenderer>();
    
    Engine.sceneMain->RemoveMeshRendererFromSceneRoot( chunkRenderer, RENDER_QUEUE_GEOMETRY );
    Engine.sceneMain->RemoveMeshRendererFromSceneRoot( staticRenderer, RENDER_QUEUE_GEOMETRY );
    
    // Destroy water if it was generated
    if (chunk.waterObject != nullptr) {
        
        Engine.Destroy<GameObject>( chunk.waterObject );
        
        MeshRenderer* waterRenderer = chunk.staticObject->GetComponent<MeshRenderer>();
        
        Engine.sceneMain->RemoveMeshRendererFromSceneRoot( waterRenderer, RENDER_QUEUE_POSTGEOMETRY );
    }
    
    Physics.DestroyRigidBody( chunk.rigidBody );
    Physics.DestroyHeightFieldMap(chunk.meshCollider);
    
    // Wrangle any associated actors for saving
    for (unsigned int i=0; i < actors.size(); i++) {
        
        GameObject* actorObject = actors[i];
        glm::vec3 actorPos = actorObject->GetPosition();
        glm::vec3 chunkPos = glm::vec3(chunk.x, 0, chunk.y);
        
        if (glm::distance(actorPos, chunkPos) < chunkSize ) 
            KillActor(actorObject);
        
        continue;
    }
    
    
    // TODO: Send the save data into a threaded saving/loading system
    
    
    return true;
}


bool ChunkManager::SaveWorld(std::string worldname) {
    
    
    
    
    
    return 0;
}

bool ChunkManager::LoadWorld(std::string worldname) {
    
    
    
    
    
    return 0;
}

void ChunkManager::ClearWorld(void) {
    
    for (unsigned int i=0; i < chunks.size(); i++) 
        DestroyChunk( chunks[i] );
    
    for (unsigned int a=0; a < actors.size(); a++) 
        actors[a]->Deactivate();
    
    chunks.clear();
    
    ChunkCounterX = 0;
    ChunkCounterZ = 0;
    
    return;
}


GameObject* ChunkManager::SpawnActor(float x, float y, float z) {
    
    GameObject* actorObject = nullptr;
    
    for (unsigned int a=0; a < actors.size(); a++) {
        
        if (actors[a]->isActive) 
            continue;
        
        actorObject = actors[a];
        actorObject->SetPosition(x, y, z);
        actorObject->Activate();
        
        break;
    }
    
    if (actorObject == nullptr) {
        
        actorObject = Engine.CreateAIActor( glm::vec3(x, y, z) );
        
        actors.push_back( actorObject );
        
    }
    
    return actorObject;
}

bool ChunkManager::KillActor(GameObject* actorObject) {
    
    Actor* ActorPtr = actorObject->GetComponent<Actor>();
    
    ActorPtr->SetName("");
    ActorPtr->SetChanceToChangeDirection(0);
    ActorPtr->SetChanceToFocusOnActor(0);
    ActorPtr->SetChanceToStopWalking(0);
    ActorPtr->SetChanceToWalk(0);
    ActorPtr->SetTargetPoint(glm::vec3(0, 0, 0));
    
    ActorPtr->SetSpeed(0);
    ActorPtr->SetSpeedMultiplier(0);
    
    ActorPtr->SetYouthScale(0);
    ActorPtr->SetAdultScale(0);
    
    actorObject->Deactivate();
    
    return true;
    
    /*
    
    for (unsigned int i=0; i < actors.size(); i++) {
        
        GameObject* worldActorObject = actors[i];
        
        if (worldActorObject != actorObject) 
            continue;
        
        mFreeActors.push_back( actorObject );
        
        actorObject->Deactivate();
        
        actors.erase( actors.begin() + i );
        
        return true;
    }
    
    */
    
    return false;
}
    
