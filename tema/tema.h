#pragma once
// Rosu Mihai Cosmin GMRV1A

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
     public:
        Tema();
        ~Tema();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        Texture2D *CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char *data);
        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1 = NULL, Texture2D *texture2 = NULL, int level = 0, Texture2D *noise = NULL);
        
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateFramebuffer(int width, int height);
        Texture2D *LoadTexture(const char *imagePath);
        void LoadShader(const std::string& name);
        // Function used to draw a tree at a given position. Function is used recursively to draw the branches.
        void DrawTree(glm::vec3 position, int level, int branchCode);
        // Function used to draw the helicopter.
        void DrawHelicopter();
        // Function used to draw the marker for the helicopter's destination.
        void DrawMarker(glm::vec3 position);

        std::unordered_map<std::string, Texture2D *> mapTextures;

        // Ids used for frame buffer
        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int information_texture;
        unsigned int depth_texture;

        // Terrain data
        int terrainSize;

        // Tree data
        float treeHeight;
        float treeBranchScaleDown;
        float treeMaxLevel;
        int treeMaxCount;
        float treeSpawnRadius;
        std::vector<glm::vec3> trees;
        int treeId;

        // Helicopter data
        glm::vec3 helicopterPosition;
        glm::vec3 helicopterDirection;
        glm::vec3 helicopterDestination;
        float helicopterMovementThreshold;
        float helicopterMoveSpeed;
        float helicopterAnimation;
        float helicopterAnimationSpeed;
        bool helicopterDrawBody;
        glm::vec3 helicopterBodyColor;
        glm::vec3 helicopterPropellerColor;

        // Marker data
        int circleVertexCount;
        float markerAnimation;
        float markerAnimationSpeed;
        glm::vec3 circleColor;

        // Camera data
        gfxc::Camera *camera;
        glm::vec3 cameraOffset;
    };
}   // namespace lab
