// Rosu Mihai Cosmin GMRV1A
#include "lab/tema/tema.h"

#include <vector>
#include <string>
#include <iostream>
#include <stb/stb_image.h>

using namespace std;
using namespace lab;


Tema::Tema()
{
}


Tema::~Tema()
{
}


void Tema::Init()
{
    camera = GetSceneCamera();
    GetCameraInput()->SetActive(false);

    framebuffer_object = 0;
    color_texture = 0;
    information_texture = 0;
    depth_texture = 0;

    terrainSize = 100;
    treeHeight = 1.2f;
    treeBranchScaleDown = 0.7f;
    treeMaxLevel = 6;
    treeMaxCount = 10;
    treeSpawnRadius = 30;
    treeId = 0;
    helicopterPosition = glm::vec3(0.0f);
    helicopterDirection = glm::vec3(0.0f);
    helicopterDestination = glm::vec3(0.0f);
    helicopterMovementThreshold = 0.5f;
    helicopterMoveSpeed = 4;
    helicopterAnimation = 0;
    helicopterAnimationSpeed = 5;
    helicopterBodyColor = glm::vec3(0.1, 0.3, 0);
    helicopterPropellerColor = glm::vec3(0.3, 0.24, 0.09);
    circleVertexCount = 30;
    markerAnimation = 0;
    markerAnimationSpeed = 1;
    circleColor = glm::vec3(1, 0.55f, 0.95f);
    cameraOffset = glm::vec3(0, 10, 10);

    // Load textures
    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\tree.png");
        mapTextures["tree"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\leaves.jpg");
        mapTextures["leaves"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\noise.png");
        mapTextures["noise"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\ground.jpg");
        mapTextures["ground"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\snow.jpg");
        mapTextures["snow"] = texture;
    }


    // Paralelipiped
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0.2f, -0.2f, 0.2f), glm::vec3(1, 0, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(0.2f, -0.2f, -0.2f), glm::vec3(1, 0, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.2f, -0.2f, -0.2f), glm::vec3(0, 0, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.2f, -0.2f, 0.2f), glm::vec3(0, 0, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(0.2f, treeHeight, 0.2f), glm::vec3(1, 0, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(0.2f, treeHeight, -0.2f), glm::vec3(1, 0, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.2f, treeHeight, -0.2f), glm::vec3(0, 0, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.2f, treeHeight, 0.2f), glm::vec3(0, 0, 1), glm::vec3(0))
        };

        vector<unsigned int> indices =
        {
            0, 3, 1,
            3, 2, 1,
            0, 7, 3,
            7, 0, 4,
            4, 0, 1,
            5, 4, 1,
            5, 1, 2,
            5, 2, 6,
            7, 6, 2,
            7, 2, 3,
            4, 6, 7,
            5, 6, 4
        };

        Mesh* mesh = new Mesh("paralelipiped");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Box
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 1, 0), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 1), glm::vec3(0)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 1), glm::vec3(0))
        };

        vector<unsigned int> indices =
        {
            0, 3, 1,
            3, 2, 1,
            0, 7, 3,
            7, 0, 4,
            4, 0, 1,
            5, 4, 1,
            5, 1, 2,
            5, 2, 6,
            7, 6, 2,
            7, 2, 3,
            4, 6, 7,
            5, 6, 4
        };

        Mesh* mesh = new Mesh("box");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Terrain
    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        for (int i = 0; i < terrainSize - 1; i++) {
            for (int j = 0; j < terrainSize - 1; j++) {
                vertices.push_back(VertexFormat(glm::vec3(j - terrainSize / 2, 0.0f, i - terrainSize / 2),
                                                glm::vec3(0.0f),
                                                glm::vec3(0.0f),
                                                glm::vec2(float(j) / terrainSize, float(i) / terrainSize)));

                indices.push_back(i * terrainSize + j);
                indices.push_back((i + 1) * terrainSize + j + 1);
                indices.push_back(i * terrainSize + j + 1);

                indices.push_back(i * terrainSize + j);
                indices.push_back((i + 1) * terrainSize + j);
                indices.push_back((i + 1) * terrainSize + j + 1);
            }

            vertices.push_back(VertexFormat(glm::vec3(terrainSize - 1 - terrainSize / 2, 0.0f, i - terrainSize / 2),
                                            glm::vec3(0.0f),
                                            glm::vec3(0.0f),
                                            glm::vec2(float(terrainSize - 1) / terrainSize, float(i) / terrainSize)));
        }

        for (int j = 0; j < terrainSize - 1; j++) {
            vertices.push_back(VertexFormat(glm::vec3(j - terrainSize / 2, 0.0f, terrainSize - 1 - terrainSize / 2),
                                            glm::vec3(0.0f),
                                            glm::vec3(0.0f),
                                            glm::vec2(float(j) / terrainSize, float(terrainSize - 1) / terrainSize)));
        }

        vertices.push_back(VertexFormat(glm::vec3(terrainSize - 1 - terrainSize / 2, 0.0f, terrainSize - 1 - terrainSize / 2),
                                        glm::vec3(0.0f),
                                        glm::vec3(0.0f),
                                        glm::vec2(float(terrainSize - 1) / terrainSize, float(terrainSize - 1) / terrainSize)));

        Mesh* mesh = new Mesh("terrain");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Circle
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), circleColor, glm::vec3(0))
        };

        vector<unsigned int> indices;

        float circleStep = 2 * RADIANS(180) / circleVertexCount;
        for (int i = 0; i < circleVertexCount - 1; i++) {
            vertices.push_back(VertexFormat(glm::vec3(cos(i * circleStep), 0.0f, sin(i * circleStep)), circleColor));

            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }

        vertices.push_back(VertexFormat(glm::vec3(cos((circleVertexCount - 1) * circleStep), 0.0f, sin((circleVertexCount - 1) * circleStep)), circleColor));

        indices.push_back(0);
        indices.push_back(circleVertexCount - 1);
        indices.push_back(circleVertexCount);

        indices.push_back(0);
        indices.push_back(circleVertexCount);
        indices.push_back(1);

        Mesh* mesh = new Mesh("circle");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load shaders
    LoadShader("Helicopter");
    LoadShader("Tree");
    LoadShader("Terrain");
    LoadShader("Marker");

    // Create trees
    for (int i = 0; i < treeMaxCount; i++) {
        trees.push_back(glm::vec3(rand() % int(treeSpawnRadius) - treeSpawnRadius / 2, 0, rand() % int(treeSpawnRadius) - treeSpawnRadius / 2));
    }

    // Create frame buffer
    CreateFramebuffer(window->GetResolution().x, window->GetResolution().y);
}

void Tema::CreateFramebuffer(int width, int height)
{
    // Create buffer
    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

    // Create first color texture
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

    // Create second color texture
    glGenTextures(1, &information_texture);
    glBindTexture(GL_TEXTURE_2D, information_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, information_texture, 0);

    // Create depth texture
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    // Set textures as draw textures
    std::vector<GLenum> draw_textures;

    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    draw_textures.push_back(GL_COLOR_ATTACHMENT1);

    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete!: %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D *Tema::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    GLint internalFormat = GL_RGBA;
    switch (channels) {
        case 1:
            internalFormat = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema::DrawTree(glm::vec3 position, int level, int branchCode)
{
    if (level == treeMaxLevel)
        return;

    // If the branch is on level 0, it means it is the trunk.
    if (level == 0) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position);
        RenderSimpleMesh(meshes["paralelipiped"], shaders["Tree"], modelMatrix, mapTextures["tree"], mapTextures["leaves"], level, mapTextures["noise"]);

        for (int i = 0; i < 3; i++)
            DrawTree(position, level + 1, i);
    }
    // Otherwise, the branch has to use the transformations specified by its branch code.
    else {
        int levelCopy = level;
        int branchCodeCopy = branchCode;
        float currentHeight = treeHeight;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position);

        while (levelCopy > 0) {
            int currentCode = branchCodeCopy % 10;

            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, currentHeight, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(120 * currentCode), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45), glm::vec3(0, 0, 1));

            branchCodeCopy /= 10;
            currentHeight = currentHeight * treeBranchScaleDown;
            levelCopy--;
        }

        modelMatrix = glm::scale(modelMatrix, glm::vec3(pow(treeBranchScaleDown, level)));
        RenderSimpleMesh(meshes["paralelipiped"], shaders["Tree"], modelMatrix, mapTextures["tree"], mapTextures["leaves"], level, mapTextures["noise"]);

        for (int i = 0; i < 3; i++)
            DrawTree(position, level + 1, i * pow(10, level) + branchCode);
    }
}

void Tema::DrawHelicopter()
{
    float helicopterAngle = atan2(helicopterDirection.x, helicopterDirection.z);
    helicopterDrawBody = true;

    // Main Body
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, 0.5f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    // Tail
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.3f, 0.1f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    helicopterDrawBody = false;

    // Main propellers
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3f, 0));
    modelMatrix = glm::rotate(modelMatrix, helicopterAnimation, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.05f, 0.1f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3f, 0));
    modelMatrix = glm::rotate(modelMatrix, helicopterAnimation + RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.05f, 0.1f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    // Tail propellers
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, 0.15f, 0));
    modelMatrix = glm::rotate(modelMatrix, helicopterAnimation, glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.1f, 0.05f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::rotate(modelMatrix, helicopterAngle + RADIANS(-90), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, 0.15f, 0));
    modelMatrix = glm::rotate(modelMatrix, helicopterAnimation + RADIANS(90), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.1f, 0.05f));
    RenderSimpleMesh(meshes["box"], shaders["Helicopter"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);
}

void Tema::DrawMarker(glm::vec3 position)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    RenderSimpleMesh(meshes["circle"], shaders["Marker"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4f + sin(markerAnimation) * 0.1f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.25f, 0.1f));
    RenderSimpleMesh(meshes["box"], shaders["Marker"], modelMatrix, NULL, NULL, 0, mapTextures["noise"]);
}

void Tema::FrameStart()
{
    glm::ivec2 resolution = window->GetResolution();

    // Both color attachements are cleared to the color black.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // The second color attachment must be cleared to white.
    GLenum informationBuffer = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(1, &informationBuffer);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffers);

    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema::Update(float deltaTimeSeconds)
{
    camera->SetPositionAndRotation(helicopterPosition + cameraOffset, glm::quatLookAt(-glm::normalize(cameraOffset - glm::vec3(0, 5, 0)), glm::vec3(0, 1, 0)));

    // Terrain
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["terrain"], shaders["Terrain"], modelMatrix, mapTextures["ground"], mapTextures["snow"], 0, mapTextures["noise"]);
    }

    // Trees
    for (int i = 0; i < trees.size(); i++) {
        treeId = i;
        DrawTree(trees[i], 0, 0);
    }

    // Marker
    markerAnimation += deltaTimeSeconds * markerAnimationSpeed;
    if (markerAnimation > RADIANS(360) * 1000)
        markerAnimation = 0;
    if (glm::distance(helicopterPosition, helicopterDestination) > helicopterMovementThreshold)
        DrawMarker(helicopterDestination);

    // Helicopter
    helicopterAnimation += deltaTimeSeconds * helicopterAnimationSpeed;
    if (helicopterAnimation > RADIANS(360) * 1000)
        helicopterAnimation = 0;

    if (glm::distance(helicopterPosition, helicopterDestination) > helicopterMovementThreshold)
        helicopterPosition += helicopterDirection * deltaTimeSeconds * helicopterMoveSpeed;
    DrawHelicopter();
}


void Tema::FrameEnd()
{
    glm::ivec2 resolution = window->GetResolution();
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_object);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


void Tema::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1, Texture2D *texture2, int level, Texture2D *noise)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int loc_level = glGetUniformLocation(shader->program, "level");
    glUniform1i(loc_level, level);

    int loc_tree_height = glGetUniformLocation(shader->program, "treeHeight");
    glUniform1f(loc_tree_height, treeHeight);

    int loc_marker_color = glGetUniformLocation(shader->program, "marker_color");
    glUniform3f(loc_marker_color, circleColor.r, circleColor.g, circleColor.b);

    int loc_terrain_size = glGetUniformLocation(shader->program, "terrainSize");
    glUniform1i(loc_terrain_size, terrainSize);

    int loc_helicopter_pos = glGetUniformLocation(shader->program, "helicopterPosition");
    glUniform3f(loc_helicopter_pos, helicopterPosition.x, helicopterPosition.y, helicopterPosition.z);

    int loc_heli_draw_body = glGetUniformLocation(shader->program, "helicopterDrawBody");
    glUniform1i(loc_heli_draw_body, helicopterDrawBody);

    int loc_heli_body_color = glGetUniformLocation(shader->program, "helicopterBodyColor");
    glUniform3f(loc_heli_body_color, helicopterBodyColor.r, helicopterBodyColor.g, helicopterBodyColor.b);

    int loc_heli_prop_color = glGetUniformLocation(shader->program, "helicopterPropellerColor");
    glUniform3f(loc_heli_prop_color, helicopterPropellerColor.r, helicopterPropellerColor.g, helicopterPropellerColor.b);

    int loc_tree_id = glGetUniformLocation(shader->program, "treeID");
    glUniform1i(loc_tree_id, treeId);

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);

        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());

        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    if (noise) {
        glActiveTexture(GL_TEXTURE2);

        glBindTexture(GL_TEXTURE_2D, noise->GetTextureID());

        glUniform1i(glGetUniformLocation(shader->program, "noise"), 2);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Tema::LoadShader(const std::string& name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders");

    // Create a shader program
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 2) {
        float data[4];

        mouseY = window->props.resolution.y - mouseY;

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
        glReadBuffer(GL_COLOR_ATTACHMENT1);

        glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_FLOAT, data);

        // If the g value of the color is 0 (less than 0.1), it means the click was on the terrain.
        if (data[1] < 0.1) {
            helicopterDestination = glm::vec3(data[0] * terrainSize - terrainSize / 2, 0, data[2] * terrainSize - terrainSize / 2);
            helicopterDirection = glm::normalize(helicopterDestination - helicopterPosition);
        }
        // Else if was on a tree (excluding g being 1 (more than 0.9)).
        else if (data[1] < 0.9) {
            float roundedValue = round(data[1] * 100);
            printf("%f\n", roundedValue);
            int clickedTreeId = int(roundedValue) - 20;
            printf("Clicked on tree %d\n", clickedTreeId);
            trees.erase(trees.begin() + clickedTreeId);

            trees.push_back(glm::vec3(rand() % int(treeSpawnRadius) - treeSpawnRadius / 2, 0, rand() % int(treeSpawnRadius) - treeSpawnRadius / 2));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}
