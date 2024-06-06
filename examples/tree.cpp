#include <iostream>
#include <vector>
#include <fstream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Line.hpp"
#include "Transform.hpp"
#include "PlaneMesh.hpp"
#include "Quad.hpp"
#include "Texture.hpp"
#include "Sphere.hpp"
#include "Point.hpp"
#include "Model.hpp"

#include "utils.hpp"
#include "CubeMap.hpp"

class TModel;

class TreeRenderingWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader shader = spry::ShaderManager::mvp_shader();
    spry::Camera m_camera;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Cuboid cube;
    spry::Sphere sphere;
    spry::PlaneMesh plane;

    spry::Shader model_shader;
    // spry::Model tree;
    // TModel tmodel;

    bool update_camera = true;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("model", model);
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        // shader.set_uniform_vec("color", glm::vec4(0.5, 0.3, 0.3, 1.0));
        // sphere.draw();

        // model = spry::Transform().translate(glm::vec3(-50.0f, 0.0f, 50.0f)).rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)).get_model();
        // shader.set_uniform_matrix("model", model);
        // shader.set_uniform_vec("color", glm::vec4(0.34, 0.5, 0.3, 1.0));
        // plane.draw();

        model_shader.use();
        model_shader.set_uniform_matrix("model", model);
        model_shader.set_uniform_matrix("projection", projection);
        model_shader.set_uniform_matrix("view", view);
        // tree.draw(model_shader);

        check_for_opengl_error();
        // close_window();
        draw_ui(delta_time);
    }

    void process_input(float delta_time)
    {
        if (is_key_pressed(GLFW_KEY_ESCAPE)) {
            close_window();
        }
        if (is_key_pressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_D)) {
            m_camera.process_movement(spry::Camera::movement::RIGHT, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_SPACE)) {
            m_camera.process_movement(spry::Camera::movement::UP, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
            m_camera.process_movement(spry::Camera::movement::DOWN, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_U)) {
            set_mouse_capture(false);
            update_camera = false;
        }
        if (is_key_pressed(GLFW_KEY_L)) {
            set_mouse_capture(true);
            update_camera = true;
            m_camera.mouse_data.first_mouse = true;
        }
    }

    void on_mouse_move(double x_pos_in, double y_pos_in) override
    {
        if (!update_camera)
            return;
        float x_pos = static_cast<float>(x_pos_in);
        float y_pos = static_cast<float>(y_pos_in);

        if (m_camera.mouse_data.first_mouse) {
            m_camera.mouse_data.last_x = x_pos;
            m_camera.mouse_data.last_y = y_pos;
            m_camera.mouse_data.first_mouse = false;
        }

        float x_offset = x_pos - m_camera.mouse_data.last_x;
        float y_offset = m_camera.mouse_data.last_y - y_pos;

        m_camera.mouse_data.last_x = x_pos;
        m_camera.mouse_data.last_y = y_pos;

        m_camera.process_mouse_movement(x_offset, y_offset, true);
    }

    void on_mouse_scroll(double x_offset, double y_offset) override
    {
        m_camera.process_mouse_scroll(static_cast<float>(y_offset));
    }

    void on_screen_size_change(int width, int height) override
    {
        m_camera.set_screen_size(width, height);
    }

    void draw_axes()
    {
        auto line_model = glm::mat4(1.0f);
        shader.set_uniform_matrix("model", line_model);
        shader.set_uniform_vec("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        x_axis.draw();
        shader.set_uniform_vec("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        y_axis.draw();
        shader.set_uniform_vec("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        z_axis.draw();
    }

    void draw_ui(float delta_time)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Options");
        ImGui::Text("FPS: %f", 1.0f / delta_time);
        ImGui::Text("Pos: %f, %f %f", m_camera.m_position.x, m_camera.m_position.y, m_camera.m_position.z);
        ImGui::SliderFloat("speed", &m_camera.m_speed, 0.0f, 1000.0f);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

public:
    TreeRenderingWindow(int width, int height)
        : Window(width, height, "Trees")
        , m_width(width)
        , m_height(height)
        // , tree("../examples/models/backpack/backpack.obj")
        // , tmodel("../examples/models/backpack/backpack.obj")
        , model_shader("../examples/model.vert", "../examples/model.frag")
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.3, 0.3, 0.3, 1.0f);

        set_mouse_capture(true);
        glPointSize(1.0f);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_far_point = 1000.0f;

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);
        plane.load(100.0f, 100.0f, 10, 10);
        model_shader.compile();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(get_window(), true);
        ImGui_ImplOpenGL3_Init();

        check_for_opengl_error();
    }
};

// int main(int argc, char** argv)
// {
//     stbi_set_flip_vertically_on_load(true);

//     TreeRenderingWindow w(1200, 1000);
//     w.start();

//     return 0;
// }
// 'C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64\cl.exe'

class TModel {
private:
    Assimp::Importer importer; // https://assimp-docs.readthedocs.io/en/v5.1.0/ ... (An older Assimp website: http://assimp.sourceforge.net/lib_html/index.html)
    const aiScene* scene = nullptr;
    aiNode* root_node = nullptr; // Only being used in the: load_model_cout_console() function.

    struct Mesh {
        unsigned int VAO, VBO1, VBO2, VBO3, EBO; // Buffer handles (Typically type: GLuint is used)

        std::vector<glm::vec3> vert_positions;
        std::vector<glm::vec3> vert_normals;
        std::vector<glm::vec2> tex_coords;
        std::vector<unsigned int> vert_indices;
        unsigned int tex_handle;
    };

    struct Texture {
        unsigned int textureID;
        std::string image_name;
    };

public:
    unsigned int num_meshes;
    std::vector<Mesh> mesh_list;
    std::vector<Texture> texture_list;

    TModel(const char* model_path) // Constructor
    {
        // http://assimp.sourceforge.net/lib_html/postprocess_8h.html (See: aiPostProcessSteps) (Flag options)

        // scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
        // scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
        // scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        // scene = importer.ReadFile(model_path, NULL);

        load_model(); // Uncomment only one of these two load model functions.
                      // load_model_cout_console();
    }

private:
    void load_model()
    {
        if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
            std::cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
        else {
            num_meshes = scene->mNumMeshes;
            mesh_list.resize(num_meshes);

            aiMesh* mesh {};
            int indices_offset = 0; // Not being used yet... i.e. indices_offset += mesh->mNumVertices; is commented further down.

            // (1) Loop through all the model's meshes
            // -----------------------------------------------------
            for (unsigned int i = 0; i < num_meshes; ++i) {
                mesh = scene->mMeshes[i]; // http://assimp.sourceforge.net/lib_html/structai_mesh.html

                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // http://assimp.sourceforge.net/lib_html/structai_material.html

                // This loop will only run once (i.e. there's only 1 texture per mesh)
                for (unsigned int tex_count = 0; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count) // Also, only using: aiTextureType_DIFFUSE.
                {
                    aiString string;
                    material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string); // Acquire the name of the image file to be loaded.

                    // (2) Load mesh [i]'s texture if not already loaded
                    // ---------------------------------------------------------------
                    int already_loaded = is_image_loaded(string.C_Str()); // Returns -1 if texture Not already loaded, otherwise returns Existing texture handle.

                    if (already_loaded == -1) // Image not yet loaded so now attempt to load it.
                    {
                        bool load_success = false;
                        unsigned int texture_handle = load_texture_image(string.C_Str(), load_success);

                        if (load_success) // Although do nothing if the image fails to load.
                        {
                            Texture texture;
                            texture.image_name = string.C_Str();
                            texture.textureID = texture_handle;

                            texture_list.push_back(texture);
                            mesh_list[i].tex_handle = texture_handle;
                        }
                    } else
                        mesh_list[i].tex_handle = already_loaded; // Assign existing texture handle.
                }
                // (3) Loop through all mesh [i]'s vertices
                // ---------------------------------------------------
                for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2) {
                    glm::vec3 position {};
                    position.x = mesh->mVertices[i2].x;
                    position.y = mesh->mVertices[i2].y;
                    position.z = mesh->mVertices[i2].z;
                    mesh_list[i].vert_positions.push_back(position);

                    if (mesh->HasNormals()) {
                        glm::vec3 normal {};
                        normal.x = mesh->mNormals[i2].x;
                        normal.y = mesh->mNormals[i2].y;
                        normal.z = mesh->mNormals[i2].z;
                        mesh_list[i].vert_normals.push_back(normal);
                    } else
                        mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

                    if (mesh->HasTextureCoords(0)) // Only slot [0] is in question.
                    {
                        glm::vec2 tex_coords {};
                        tex_coords.x = mesh->mTextureCoords[0][i2].x;
                        tex_coords.y = mesh->mTextureCoords[0][i2].y;
                        mesh_list[i].tex_coords.push_back(tex_coords);
                    } else
                        mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
                }
                // (4) Loop through all mesh [i]'s Indices
                // --------------------------------------------------
                for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
                    for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
                        mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);

                // indices_offset += mesh->mNumVertices; // Disabled for tutorial: Model Loading (Part 1 of 3)

                set_buffer_data(i); // Set up: VAO, VBO and EBO.
            }
        }
    }

    void load_model_cout_console()
    {
        // Briefly looking at the node structure
        // ------------------------------------------------
        if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
            std::cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
        else {
            num_meshes = scene->mNumMeshes;
            mesh_list.resize(num_meshes);

            std::cout << "\n\n   Start of Assimp Loading Meshes & Analysis";
            std::cout << "\n   -----------------------------------------";

            root_node = scene->mRootNode;

            std::cout << "\n   node->mNumMeshes: " << root_node->mNumMeshes;
            std::cout << "\n   node->mName.C_Str(): " << root_node->mName.C_Str();
            std::cout << "\n\n   node->mNumChildren: " << root_node->mNumChildren;
            // ------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < root_node->mNumChildren; ++i) {
                std::cout << "\n   node->mChildren[i]->mName.C_Str(): " << root_node->mChildren[i]->mName.C_Str();
                std::cout << "\n   node->mChildren[i]->mNumMeshes: " << root_node->mChildren[i]->mNumMeshes;
            }
            std::cout << "\n\n   scene->HasMaterials(): " << scene->HasMaterials();
            // ------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
                std::cout << "\n   scene->mMaterials[i]->GetName(): " << scene->mMaterials[i]->GetName().C_Str();

            std::cout << "\n\n   scene->HasTextures(): " << scene->HasTextures();

            aiMesh* mesh {};

            int total_num_indices = 0;
            int indices_offset = 0; // Not being used yet... i.e. indices_offset += mesh->mNumVertices; is commented further down.

            // (1) Loop through all the model's meshes
            // -----------------------------------------------------
            std::cout << "\n   scene->mNumMeshes: " << num_meshes;
            std::cout << "\n   ********************\n";
            // ---------------------------------------------------------
            for (unsigned int i = 0; i < num_meshes; ++i) // In this case... scene->mNumMeshes = node->mChildren[i]->mNumMeshes
            {
                mesh = scene->mMeshes[i]; // http://assimp.sourceforge.net/lib_html/structai_mesh.html

                std::cout << "\n\n   mesh->mMaterialIndex: " << mesh->mMaterialIndex;
                std::cout << "\n   ----------------------- ";
                std::cout << "\n   mesh->mName.C_Str(): " << mesh->mName.C_Str();

                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // http://assimp.sourceforge.net/lib_html/structai_material.html

                std::cout << "\n\n   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string): " << material->GetTextureCount(aiTextureType_DIFFUSE);
                std::cout << "\n   material->GetTexture(aiTextureType_SPECULAR, tex_count, &string): " << material->GetTextureCount(aiTextureType_SPECULAR);
                std::cout << "\n   material->GetTexture(aiTextureType_AMBIENT, tex_count, &string): " << material->GetTextureCount(aiTextureType_AMBIENT) << "\n\n";

                unsigned int tex_count = 0;
                for (; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count) // The above std::cout reveals that only using: aiTextureType_DIFFUSE
                {
                    aiString string;
                    material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string); // Acquire the name of the image file to be loaded.
                    std::cout << "   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string): " << string.C_Str() << "\n\n";

                    // (2) Load mesh [i]'s texture if not already loaded
                    // ---------------------------------------------------------------
                    int already_loaded = is_image_loaded(string.C_Str()); // Returns -1 if texture Not already loaded, otherwise returns Existing texture handle.
                    std::cout << "   Loading Image\n";

                    if (already_loaded == -1) // Image not yet loaded.
                    {
                        bool load_complete = false;
                        unsigned int texture_handle = load_texture_image(string.C_Str(), load_complete);

                        if (load_complete) // Although do nothing if the image fails to load.
                        {
                            Texture texture;
                            texture.image_name = string.C_Str();
                            texture.textureID = texture_handle;

                            texture_list.push_back(texture);
                            mesh_list[i].tex_handle = texture_handle;
                        }
                    } else // Assign existing texture handle.
                    {
                        std::string edited = string.C_Str();
                        std::size_t position = edited.find_last_of("\\");

                        std::cout << "   Image file: " << edited.substr(position + 1) << " (is already loaded)";
                        mesh_list[i].tex_handle = already_loaded;
                    }
                }
                if (tex_count == 0)
                    std::cout << "   material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string): No image has been applied to this mesh\n\n";
                else
                    std::cout << "\n";

                for (unsigned int slot = 0; slot < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++slot)
                    std::cout << "   mesh->HasTextureCoords(" << slot << "): " << mesh->HasTextureCoords(slot) << "\n";

                std::cout << "\n   Mesh index: " << i << " (mesh->mNumVertices: " << mesh->mNumVertices << ")";
                std::cout << "\n   ------------------------------------- ";

                // (3) Loop through all mesh [i]'s vertices
                // ---------------------------------------------------
                for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2) {
                    glm::vec3 position {};
                    position.x = mesh->mVertices[i2].x;
                    position.y = mesh->mVertices[i2].y;
                    position.z = mesh->mVertices[i2].z;
                    mesh_list[i].vert_positions.push_back(position);

                    std::cout << "\n   Count: " << i2;
                    std::cout << "\n   mesh->mVertices[" << i2 << "].x: " << position.x;
                    std::cout << "\n   mesh->mVertices[" << i2 << "].y: " << position.y;
                    std::cout << "\n   mesh->mVertices[" << i2 << "].z: " << position.z;

                    if (mesh->HasNormals()) {
                        glm::vec3 normal {};
                        normal.x = mesh->mNormals[i2].x;
                        normal.y = mesh->mNormals[i2].y;
                        normal.z = mesh->mNormals[i2].z;
                        mesh_list[i].vert_normals.push_back(normal);
                        std::cout << "\n   mesh->mNormals[" << i2 << "] X: " << normal.x << " Y: " << normal.y << " Z: " << normal.z;
                    } else
                        mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

                    if (mesh->HasTextureCoords(0)) // Above for loop: AI_MAX_NUMBER_OF_TEXTURECOORDS reveals that only slot [0] is in question.
                    {
                        glm::vec2 tex_coords {};
                        tex_coords.x = mesh->mTextureCoords[0][i2].x;
                        tex_coords.y = mesh->mTextureCoords[0][i2].y;
                        mesh_list[i].tex_coords.push_back(tex_coords);
                        std::cout << "\n   mesh->mTextureCoords[0][" << i2 << "] X: " << tex_coords.x << " Y: " << tex_coords.y;
                    } else
                        mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
                }
                std::cout << "\n\n   mesh->mNumFaces: " << mesh->mNumFaces << "\n";
                std::cout << "   ------------------ ";

                // (4) Loop through all mesh [i]'s Indices
                // --------------------------------------------------
                for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3) {
                    std::cout << "\n";
                    for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4) {
                        std::cout << "   mesh->mFaces[" << i3 << "].mIndices[" << i4 << "]: " << mesh->mFaces[i3].mIndices[i4] << "\n";
                        mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);
                        ++total_num_indices;
                    }
                }
                std::cout << "\n   Total number of indices: " << total_num_indices;
                std::cout << "\n   **************************";
                total_num_indices = 0;

                // indices_offset += mesh->mNumVertices; // Disabled for tutorial: Model Loading (Part 1 of 3)

                std::cout << "\n   Indices offset (Total 'mesh->mNumVertices' so far): " << indices_offset;
                std::cout << "\n   *****************************************************\n\n";

                set_buffer_data(i); // Set up: VAO, VBO and EBO.
            }
            // Look to see if each mesh's texture handle corresponds correctly to the loaded image
            // ----------------------------------------------------------------------------------------------------------------
            if (texture_list.size() > 0)
                for (unsigned int i = 0; i < texture_list.size(); ++i) {
                    std::cout << "   image_list[" << i << "].imageID: " << texture_list[i].textureID << "... image_list[" << i << "].image_name: " << texture_list[i].image_name << "\n";

                    for (unsigned int i2 = 0; i2 < num_meshes; ++i2)
                        if (texture_list[i].textureID == mesh_list[i2].tex_handle)
                            std::cout << "   mesh_list[" << i2 << "].tex_handle: " << mesh_list[i2].tex_handle << "\n";
                    std::cout << "\n";
                }
            else
                std::cout << "   ***** No images have been loaded\n";
        }
    }

    void set_buffer_data(unsigned int index)
    {
        glGenVertexArrays(1, &mesh_list[index].VAO);
        glGenBuffers(1, &mesh_list[index].VBO1); // Alternative to using 3 separate VBOs, instead use only 1 VBO and set glVertexAttribPointer's offset...
        glGenBuffers(1, &mesh_list[index].VBO2); // like was done in tutorial 3... Orbiting spinning cubes.
        glGenBuffers(1, &mesh_list[index].VBO3);
        glGenBuffers(1, &mesh_list[index].EBO);

        glBindVertexArray(mesh_list[index].VAO);

        // Vertex Positions
        // ---------------------
        glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_positions.size(), &mesh_list[index].vert_positions[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // Void pointer below is for legacy reasons. Two possible meanings: "offset for buffer objects" & "address for client state arrays"
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Vertex Normals
        // --------------------
        glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_normals.size(), &mesh_list[index].vert_normals[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Texture Coordinates
        // ---------------------------
        glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO3);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh_list[index].tex_coords.size(), &mesh_list[index].tex_coords[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        // Indices for: glDrawElements()
        // ---------------------------------------
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0); // Unbind VAO
    }

    int is_image_loaded(std::string file_name)
    {
        for (unsigned int i = 0; i < texture_list.size(); ++i)
            if (file_name.compare(texture_list[i].image_name) == 0)
                return texture_list[i].textureID;
        return -1;
    }

    unsigned int load_texture_image(std::string file_name, bool& load_complete)
    {
        // stbi_set_flip_vertically_on_load(1); // Call this function if the image is upside-down.

        std::size_t position = file_name.find_last_of("\\");
        file_name = "Images\\" + file_name.substr(position + 1);

        int width, height, num_components;
        unsigned char* image_data = stbi_load(file_name.c_str(), &width, &height, &num_components, 0);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        if (image_data) {
            GLenum format {};

            if (num_components == 1)
                format = GL_RED;
            else if (num_components == 3)
                format = GL_RGB;
            else if (num_components == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Recommended by NVIDIA Rep: https://devtalk.nvidia.com/default/topic/875205/opengl/how-does-gl_unpack_alignment-work-/

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
            // ----------------------------------------------------------------------------------------------------------------
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // GL_REPEAT... GL_MIRRORED_REPEAT... GL_CLAMP_TO_EDGE... GL_CLAMP_TO_BORDER.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

            // float border_colour[] = {0.45, 0.55, 0.95};
            // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour); // For above when using: GL_CLAMP_TO_BORDER

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST... GL_LINEAR... GL_NEAREST_MIPMAP_NEAREST (See above link for full list)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST or GL_LINEAR.

            load_complete = true;
            stbi_image_free(image_data);
            std::cout << "   Image loaded OK: " << file_name << "\n";
        } else {
            load_complete = false;
            stbi_image_free(image_data);
            std::cout << "   Image failed to load: " << file_name << "\n";
        }
        return textureID;
    }
};