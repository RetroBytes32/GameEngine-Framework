#include <GameEngineFramework/Engine/Engine.h>

ENGINE_API extern EngineComponents  Components;
ENGINE_API extern ColorPreset       Colors;
ENGINE_API extern NumberGeneration  Random;
ENGINE_API extern Logger            Log;
ENGINE_API extern Timer             PhysicsTime;
ENGINE_API extern Timer             Time;

ENGINE_API extern Serialization     Serializer;
ENGINE_API extern ResourceManager   Resources;
ENGINE_API extern ScriptSystem      Scripting;
ENGINE_API extern RenderSystem      Renderer;
ENGINE_API extern PhysicsSystem     Physics;
ENGINE_API extern AudioSystem       Audio;
ENGINE_API extern InputSystem       Input;
ENGINE_API extern MathCore          Math;
ENGINE_API extern ActorSystem       AI;

ENGINE_API extern ApplicationLayer      Application;
ENGINE_API extern EngineSystemManager   Engine;



Component* EngineSystemManager::CreateComponent(ComponentType type) {
    void* component_object = nullptr;
    
    switch (type) {
        
        case COMPONENT_TYPE_TRANSFORM: {
            component_object = (void*)mTransforms.Create();
            break;
        }
        case COMPONENT_TYPE_MESH_RENDERER: {
            component_object = (void*)Renderer.CreateMeshRenderer();
            break;
        }
        case COMPONENT_TYPE_CAMERA: {
            component_object = (void*)Renderer.CreateCamera();
            break;
        }
        case COMPONENT_TYPE_LIGHT: {
            component_object = (void*)Renderer.CreateLight();
            break;
        }
        
        case COMPONENT_TYPE_SCRIPT: {
            component_object = (void*)Scripting.CreateScript();
            break;
        }
        case COMPONENT_TYPE_RIGID_BODY: {
            component_object = (void*)Physics.CreateRigidBody();
            break;
        }
        case COMPONENT_TYPE_ACTOR: {
            component_object = (void*)AI.CreateActor();
            break;
        }
        case COMPONENT_TYPE_TEXT: {
            component_object = (void*)mTextObjects.Create();
            break;
        }
        case COMPONENT_TYPE_PANEL: {
            component_object = (void*)mPanelObjects.Create();
            break;
        }
        
        default:
            return nullptr;
    }
    
    Component* newComponent = mComponents.Create();
    newComponent->SetComponent(type, component_object);
    
    mDoUpdateDataStream = true;
    return newComponent;
}

bool EngineSystemManager::DestroyComponent(Component* componentPtr) {
    assert(componentPtr != nullptr);
    
    ComponentType componentType = componentPtr->GetType();
    
    switch (componentType) {
        
        case COMPONENT_TYPE_TRANSFORM: {
            mTransforms.Destroy( (Transform*)componentPtr->GetComponent() );
            break;
        }
        
        case COMPONENT_TYPE_MESH_RENDERER: {
            Renderer.DestroyMeshRenderer( (MeshRenderer*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_CAMERA: {
            Renderer.DestroyCamera( (Camera*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_LIGHT: {
            Renderer.DestroyLight( (Light*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_SCRIPT: {
            Scripting.DestroyScript( (Script*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_RIGID_BODY: {
            Physics.DestroyRigidBody( (RigidBody*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_ACTOR: {
            AI.DestroyActor( (Actor*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_TEXT: {
            mTextObjects.Destroy( (Text*)componentPtr->GetComponent() );
            break;
        }
        case COMPONENT_TYPE_PANEL: {
            mPanelObjects.Destroy( (Panel*)componentPtr->GetComponent() );
            break;
        }
        
        default:
            return false;
    }
    mComponents.Destroy(componentPtr);
    
    mDoUpdateDataStream = true;
    return true;
}

Component* EngineSystemManager::CreateComponentMeshRenderer(Mesh* meshPtr, Material* materialPtr) {
    Component* rendererComponent = CreateComponent(Components.MeshRenderer);
    MeshRenderer* meshRenderer = (MeshRenderer*)rendererComponent->mObject;
    
    meshRenderer->mesh = meshPtr;
    meshRenderer->material = materialPtr;
    
    sceneMain->AddMeshRendererToSceneRoot( meshRenderer );
    
    mDoUpdateDataStream = true;
    return rendererComponent;
}

Component* EngineSystemManager::CreateComponentLight(glm::vec3 position) {
    Component* lightComponent = CreateComponent(Components.Light);
    Light* lightPoint = (Light*)lightComponent->mObject;
    
    lightPoint->position = position;
    
    sceneMain->AddLightToSceneRoot( lightPoint );
    
    mDoUpdateDataStream = true;
    return lightComponent;
}
