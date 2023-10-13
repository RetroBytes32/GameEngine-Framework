#ifndef OPENGL_RENDERER_SYSTEM
#define OPENGL_RENDERER_SYSTEM

#include "../../vendor/CodeBaseLibrary/poolallocator.h"
#include "../../vendor/CodeBaseLibrary/types.h"
#include "../../vendor/CodeBaseLibrary/logging.h"
extern Logger Log;

#include "enumerators.h"

#include "../engine/types/viewport.h"

#include "components/shader.h"
#include "components/material.h"
#include "components/mesh.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/meshrenderer.h"
#include "components/scene.h"


class RenderSystem {
    
public:
    
    /// Default material object.
    Material* defaultMaterial;
    /// Default GLSL shader object.
    Shader*   defaultShader;
    
    /// Current rendering view port
    Viewport  viewport;
    /// Current size of the display.
    glm::vec2 displaySize;
    /// Current center of the display.
    glm::vec2 displayCenter;
    
    
    RenderSystem();
    
    Scene* operator[] (unsigned int const i) {return mRenderQueue[i];}
    
    /// Create a mesh renderer object and return its pointer.
    MeshRenderer* CreateMeshRenderer(void);
    /// Destroy a mesh renderer object and return true on success.
    bool DestroyMeshRenderer(MeshRenderer* meshRendererPtr);
    
    /// Create a mesh object and return its pointer.
    Mesh* CreateMesh(void);
    /// Destroy a mesh object and return true on success.
    bool DestroyMesh(Mesh* meshPtr);
    
    /// Create a shader object and return its pointer.
    Shader* CreateShader(void);
    /// Destroy a shader object and return true on success.
    bool DestroyShader(Shader* shaderPtr);
    
    /// Create a camera object and return its pointer.
    Camera* CreateCamera(void);
    /// Destroy a camera object and return true on success.
    bool DestroyCamera(Camera* cameraPtr);
    
    /// Create a material object and return its pointer.
    Material* CreateMaterial(void);
    /// Destroy a material object and return true on success.
    bool DestroyMaterial(Material* materialPtr);
    
    /// Create a light object and return its pointer.
    Light* CreateLight(void);
    /// Destroy a light object and return true on success.
    bool DestroyLight(Light* lightPtr);
    
    /// Create a scene object and return its pointer.
    Scene* CreateScene(void);
    /// Destroy a scene object and return true on success.
    bool DestroyScene(Scene* scenePtr);
    
    
    /// Prepare the render system.
    void Initiate(void);
    
    /// Set the target render context.
    GLenum  SetRenderTarget(HWND wHndl);
    /// Free the target render context.
    void    ReleaseRenderTarget(void);
    
    /// Set the render area within the display.
    void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    /// Draw the current frame as it stands.
    void RenderFrame(float deltaTime);
    
    /// Add a scene to the render queue for drawing.
    void AddSceneToRenderQueue(Scene* scene);
    /// Remove a scene from the render queue.
    bool RemoveSceneFromRenderQueue(Scene* scene);
    /// Get the number of scenes in the render queue.
    unsigned int GetRenderQueueSize(void);
    
    /// Set the main camera.
    void SetCamera(Camera* newCamera);
    /// Get the main camera.
    Camera* GetCamera(void);
    
    /// Return the number of entities that have been created.
    unsigned int GetMeshRendererCount(void);
    
    /// Calculate the model matrix from a given transform.
    glm::mat4 CalculateModelMatrix(Transform& modelTransform);
    
    /// Return a list of any and all openGL error codes.
    std::vector<std::string> GetGLErrorCodes(std::string errorLocationString);
    
    
private:
    
    // Device render context
    HWND  mWindowHandle;
    HDC   mDeviceContext;
    HGLRC mRenderContext;
    
    // Render queue group
    std::vector<Scene*>  mRenderQueue;
    
    // Current asset bindings
    Mesh*      mCurrentMesh;
    Material*  mCurrentMaterial;
    
    // Main camera
    Camera*   mCameraMain;
    
    // Light list
    unsigned int mNumberOfLights=0;
    glm::vec3    mLightPosition    [RENDER_NUMBER_OF_LIGHTS];
    glm::vec3    mLightAttenuation [RENDER_NUMBER_OF_LIGHTS];
    glm::vec3    mLightColor       [RENDER_NUMBER_OF_LIGHTS];
    
    
    // Render component allocators
    PoolAllocator<MeshRenderer>    mEntity;
    PoolAllocator<Mesh>            mMesh;
    PoolAllocator<Material>        mMaterial;
    PoolAllocator<Shader>          mShader;
    PoolAllocator<Camera>          mCamera;
    PoolAllocator<Light>           mLight;
    PoolAllocator<Scene>           mScene;
    
};


#endif
