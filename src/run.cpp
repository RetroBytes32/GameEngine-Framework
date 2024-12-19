#include <GameEngineFramework/Engine/EngineSystems.h>
#include <GameEngineFramework/functions.h>

#include <GameEngineFramework/plugins.h>


glm::vec3 force(0);
float forceDblTime=0;

bool isDebugReportActive = false;
bool isFullScreen = false;

std::string targetGene = "";


void Run() {
    
    chunkManager.Update();
    
    //
    // Raycast from player
    //
    
    // Move the player out of the way as we cant cast a ray from inside the collider...
    rp3d::RigidBody* rigidBody = Engine.cameraController->GetComponent<RigidBody>();
    rp3d::Transform bodyTransform = rigidBody->getTransform();
    rp3d::Vector3 bodyPosition = bodyTransform.getPosition();
    bodyPosition.y += 1000;
    
    // Cast forward a ray
    Hit hit;
    float distance = 15;
    
    glm::vec3 from      = Engine.cameraController->GetComponent<Transform>()->position;
    glm::vec3 direction = Engine.cameraController->GetComponent<Camera>()->forward;
    
    // Center the from angle
    from.y += 0.24f;
    
    
    // Pick an actors genome
    
    if (Input.CheckMouseMiddlePressed()) {
        
        if (Physics.Raycast(from, direction, distance, hit, LayerMask::Actor)) {
            
            GameObject* hitObject = (GameObject*)hit.gameObject;
            Actor* hitActor = hitObject->GetComponent<Actor>();
            
            targetGene = AI.genomes.ExtractGenome(hitActor);
            
            std::string destGene = "gene<" + targetGene + ">";
            
            Platform.SetClipboardText( destGene );
            
            unsigned int numberOfGenes = hitActor->GetNumberOfGenes();
            
            for (unsigned int i=0 ; i < numberOfGenes; i++) {
                
                MeshRenderer* geneRenderer = hitActor->GetMeshRendererAtIndex(i);
                
                std::string geneDataString = "";
                
                float xPos = geneRenderer->transform.position.x;
                float yPos = geneRenderer->transform.position.y;
                float zPos = geneRenderer->transform.position.z;
                
                float xScale = geneRenderer->transform.scale.x;
                float yScale = geneRenderer->transform.scale.y;
                float zScale = geneRenderer->transform.scale.z;
                
                geneDataString += Float.ToString(xPos) + ", " + Float.ToString(yPos) + ", " + Float.ToString(zPos) + "    ";
                geneDataString += Float.ToString(xScale) + ", " + Float.ToString(yScale) + ", " + Float.ToString(zScale);
                
                Engine.Print( geneDataString );
                
            }
            
        }
        
    }
    
    
    // Spawn an actor with the picked genome
    
    if (Input.CheckMouseLeftPressed()) {
        
        if (Physics.Raycast(from, direction, distance, hit, LayerMask::Ground)) {
            
            std::string sourceGene = Platform.GetClipboardText();
            
            if (sourceGene.size() == 0) 
                return;
            
            // Remove end character
            size_t pos = sourceGene.find(">");
            
            if (pos != std::string::npos) 
                sourceGene.erase(pos, 1);
            
            std::vector<std::string> sourceArray = String.Explode(sourceGene, '<');
            
            if ((sourceArray[0][0] == 'g') & 
                (sourceArray[0][1] == 'e') & 
                (sourceArray[0][2] == 'n') & 
                (sourceArray[0][3] == 'e')) {
                
                GameObject* newActorObject = chunkManager.SpawnActor( hit.point.x, hit.point.y, hit.point.z );
                
                GameObject* hitObject = (GameObject*)hit.gameObject;
                Actor* newActor = newActorObject->GetComponent<Actor>();
                
                AI.genomes.PreyBase( newActor );
                
                AI.genomes.InjectGenome(newActor, sourceArray[1]);
                newActor->SetGeneticUpdateFlag();
                
                newActor->SetAge( 900 + Random.Range(100, 800) );
                
                newActor->SetGeneticUpdateFlag();
                
            }
            
        }
        
    }
    
    
    // Destroy an actor
    
    if (Input.CheckMouseRightPressed()) {
        
        if (Physics.Raycast(from, direction, distance, hit, LayerMask::Actor)) {
            
            GameObject* hitObject = (GameObject*)hit.gameObject;
            
            chunkManager.KillActor( hitObject );
            
        }
        
    }
    
    
    // Move the actor back into position as we are now finished casting rays...
    bodyPosition.y -= 1000;
    bodyTransform.setPosition(bodyPosition);
    
    
    
    
    //
    // Profiling
    //
    
    if (Input.CheckKeyPressed(VK_F4)) {
        
        if (Engine.CheckIsProfilerActive()) {
            Engine.DisableProfiler();
            
            for (unsigned int i=0; i < PROFILER_NUMBER_OF_ELEMENTS; i++) 
                Engine.WriteDialog(i, "");
            
        } else {
            
            Engine.EnableProfiler();
        }
        
    }
    
    if (Input.CheckKeyPressed(VK_F3)) {
        
        isDebugReportActive = !isDebugReportActive;
        
        if (!isDebugReportActive) {
            for (unsigned int i=0; i < PROFILER_NUMBER_OF_ELEMENTS; i++) 
                Engine.WriteDialog(i, "");
        }
        
    }
    
    if (Input.CheckKeyPressed(VK_F11)) {
        
        isFullScreen = !isFullScreen;
        
        if (isFullScreen) {
            
            Platform.WindowEnableFullscreen();
            
        } else {
            
            Platform.WindowDisableFullscreen();
            
        }
        
    }
    
    // Debug report
    
    /*
    
    if (isDebugReportActive) {
        
        // Print current chunk details
        
        if (Physics.Raycast(from, glm::vec3(0, -1, 0), 1000, hit, LayerMask::Ground)) {
            
            GameObject* hitObject = (GameObject*)hit.gameObject;
            MeshRenderer* chunkRenderer = hitObject->GetComponent<MeshRenderer>();
            
            Chunk* hitChunk = (Chunk*)hitObject->GetUserData();
            
            if (hitObject != nullptr) {
                
                std::string chunkPosition = "Chunk ";
                
                chunkPosition += Float.ToString( hitObject->GetPosition().x );
                chunkPosition += ", ";
                chunkPosition += Float.ToString( hitObject->GetPosition().z );
                
                Engine.WriteDialog(0, chunkPosition);
                
                if (hitChunk->actorList.size() > 0) 
                    Engine.WriteDialog(1, "Actors " + Int.ToString(hitChunk->actorList.size()));
                
            }
            
        }
        
        // Print player position
        glm::vec3 cameraPosition = Engine.cameraController->GetPosition();
        
        std::string playerPosition = "x: ";
        playerPosition += Int.ToString( cameraPosition.x );
        playerPosition += " y: ";
        playerPosition += Int.ToString( cameraPosition.y );
        playerPosition += " z: ";
        playerPosition += Int.ToString( cameraPosition.z );
        
        Engine.WriteDialog(3, playerPosition);
        
        // Check object in front of camera
        
        if (Physics.Raycast(from, direction, distance, hit, LayerMask::Actor)) {
            
            GameObject* hitObject = (GameObject*)hit.gameObject;
            Actor* hitActor = hitObject->GetComponent<Actor>();
            
            Engine.WriteDialog(4, hitActor->GetName());
            Engine.WriteDialog(5, "Age: " + Int.ToString(hitActor->GetAge()));
            
            float actorChunkX = Math.Round( hitObject->GetPosition().x / chunkManager.chunkSize ) * chunkManager.chunkSize;
            float actorChunkZ = Math.Round( hitObject->GetPosition().z / chunkManager.chunkSize ) * chunkManager.chunkSize;
            
            // Set actor chunk to the current chunk
            Chunk* chunkPtr = chunkManager.CheckChunk( glm::vec2(actorChunkX, actorChunkZ) );
            if (chunkPtr != nullptr) {
                
                Engine.WriteDialog(6, Float.ToString(chunkPtr->position.x / chunkManager.chunkSize) + 
                                      ", " + 
                                      Float.ToString(chunkPtr->position.y / chunkManager.chunkSize));
                
            }
            
            Engine.WriteDialog(7, Float.ToString(actorChunkX) + ", " + Float.ToString(actorChunkZ));
            
            Engine.WriteDialog(8, "Genes         " + Int.ToString( hitActor->GetNumberOfGenes() ));
            
        } else {
            
            for (unsigned int i=0; i < 10-4; i++) 
                Engine.WriteDialog(4 + i, "");
            
        }
        
    }
    
    
    
    
    
    //
    // Lighting day night cycle experimentation 
    //
    
    float skyLightingMax    = 0.87f;
    float skyLightingMin    = 0.0087f;
    
    float lightingMax       = 10.0f;
    float lightingMin       = 0.0f;
    
    
    
    // Light direction
    if (Weather.sunLight != nullptr) {
        Transform* lightTransform = Weather.sunObject->GetComponent<Transform>();
        
        if (!Platform.isPaused) {
            
            if (Input.CheckKeyCurrent(VK_I)) {ambientLight += 0.01f;}
            if (Input.CheckKeyCurrent(VK_K)) {ambientLight -= 0.01f;}
            
            if (Input.CheckKeyCurrent(VK_T)) {lightTransform->RotateEuler(0, 0,  0.1);}
            if (Input.CheckKeyCurrent(VK_G)) {lightTransform->RotateEuler(0, 0, -0.1);}
            
        }
        
    }
    
    
    // Ambient limits
    if (ambientLight > 0.87f) ambientLight = 0.87f;
    if (ambientLight < 0.0f)  ambientLight = 0.0f;
    
    // World brightness
    chunkManager.world.ambientLight = ambientLight;
    
    // Sky brightness
    if (Weather.skyMaterial != nullptr) 
        Weather.skyMaterial->ambient = Math.Lerp(skyLightingMin, skyLightingMax, ambientLight);
    
    // Light brightness
    if (Weather.sunLight != nullptr) 
        Weather.sunLight->intensity = Math.Lerp(lightingMin, lightingMax, ambientLight);
    
    
    
    //
    // Update weather system
    //
    
    Weather.Update();
    
    
    //
    // Map generation
    //
    
    chunkManager.Update();
    
    */
    
    
    
    
    //
    // Camera controller movement
    //
    
    if (Engine.cameraController == nullptr) 
        return;
    
    float forceAccelerate = 0.012f;
    float forceDecelerate = 0.015f;
    
    Camera* mainCamera = Engine.sceneMain->camera;
    
    if (mainCamera != nullptr) {
        
        // No movement when paused
        if (!Platform.isPaused) {
            
            // WASD Directional
            if (Input.CheckKeyCurrent(VK_W)) {force += mainCamera->forward;}
            if (Input.CheckKeyCurrent(VK_S)) {force -= mainCamera->forward;}
            if (Input.CheckKeyCurrent(VK_A)) {force += mainCamera->right;}
            if (Input.CheckKeyCurrent(VK_D)) {force -= mainCamera->right;}
            
            // Space/Shift Elevation
            if (Input.CheckKeyCurrent(VK_SPACE)) {force += mainCamera->up;}
            if (Input.CheckKeyCurrent(VK_SHIFT)) {force -= mainCamera->up;}
        }
        
        // Double speed
        if (Input.CheckKeyCurrent(VK_CONTROL)) forceDblTime += 0.24f;
        
        if (forceDblTime > 1.0f) {forceDblTime -= (forceDecelerate * 8.0f);} else {forceDblTime = 1.0f;}
        
        if (forceDblTime > 3.5f) 
            forceDblTime = 3.5f;
        
        // Accelerate
        glm::vec3 forceTotal = force * forceAccelerate * forceDblTime;
        
        // Decelerate
        if ( glm::length(force) >  0.0001f) force -= (force * forceDecelerate);
        if (-glm::length(force) < -0.0001f) force -= (force * forceDecelerate);
        
        Engine.cameraController->AddForce(forceTotal.x, forceTotal.y, forceTotal.z);
        
        
        // Field of view effect
        float fovPullback = glm::length(forceTotal) * 80.0f;
        
        if (fovPullback > 10.0f) 
            fovPullback = 10.0f;
        
        Engine.sceneMain->camera->fov = 60 + fovPullback;
        
    }
    
    
    //
    // Pausing
    if (Input.CheckKeyPressed(VK_ESCAPE)) {
        
        Platform.Pause();
        
        if (Platform.isPaused) {
            
            Engine.EnableConsole();
            
            mainCamera->DisableMouseLook();
            
            Input.ClearKeys();
            
            Platform.ShowMouseCursor();
            
        } else {
            
            Engine.DisableConsole();
            Engine.ConsoleClearInputString();
            
            mainCamera->EnableMouseLook();
            
            // Reset mouse position
            Input.SetMousePosition(Renderer.displayCenter.x, Renderer.displayCenter.y);
            
            Platform.HideMouseCursor();
            
            // Reset timers
            Time.Update();
            PhysicsTime.Update();
        }
        
    }
    
    return;
}


void TickUpdate(void) {
    
    return;
}
