#include <iostream>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Tetrahedron.hpp"
#include "Camera.hpp"
#include "PlaneMesh.hpp"
#include "Sphere.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Model.hpp"

#include "utils.hpp"

#include <stb_image.h>


namespace temp {
    using namespace std;

    #define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(spry::Shader &shader) 
    {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if(name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
             else if(name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            // glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
            shader.set_uniform_int((name + number).c_str(), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		// ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};

    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(spry::Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }
};


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

}

class MyWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader m_vetex_and_color_shader;
    spry::Shader m_vetex_shader;
    spry::Shader m_light_shader;
    spry::Shader m_model_shader;

    spry::Camera m_camera;
    spry::Tetrahedron tetra;
    spry::PlaneMesh plane;
    spry::Sphere sphere;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Point point;

    spry::Cuboid cube;
    spry::Model backpack_model;

    // temp::Model tmodel;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //float x = glm::abs(glm::sin(get_global_time())) * 30.0f;
        //float z = glm::abs(glm::cos(get_global_time())) * 30.0f;
        glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
        glm::vec3 light_color = glm::vec3(0.6f, 0.3f, 0.4f);
        glm::vec3 diffuse_color = light_color * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuse_color * glm::vec3(0.2f);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        m_vetex_shader.use();
        m_vetex_shader.set_uniform_matrix("projection", projection);
        m_vetex_shader.set_uniform_matrix("view", view);
        draw_axes();

        m_light_shader.use();
        m_light_shader.set_uniform_matrix("model", model);
        m_light_shader.set_uniform_matrix("view", view);
        m_light_shader.set_uniform_matrix("projection", projection);

        m_light_shader.set_uniform_vec("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        m_light_shader.set_uniform_vec("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        m_light_shader.set_uniform_vec("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_light_shader.set_uniform_float("material.shininess", 32.0f);

        m_light_shader.set_uniform_vec("light.position", glm::vec3(0.0f, 10.f, 20.0f));
        m_light_shader.set_uniform_vec("light.ambient", ambientColor);
        m_light_shader.set_uniform_vec("light.diffuse", diffuse_color);
        m_light_shader.set_uniform_vec("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        m_light_shader.set_uniform_vec("object_color", glm::vec4(0.5, 0.7, 0.6, 1.0));
        m_light_shader.set_uniform_vec("view_pos", m_camera.m_position);

        // cube.draw();
        // sphere.draw();

        //spry::Transform plane_transform;
        //plane_transform.set_translation(glm::vec3(-5.0f, 0.0f, 5.0f));
        //plane_transform.set_rotation(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //m_light_shader.set_uniform_matrix("model", plane_transform.get_model());
        //m_light_shader.set_uniform_vec("object_color", glm::vec4(0.8, 0.8, 0.6, 1.0));
        //plane.draw();

        m_model_shader.use();
        check_for_opengl_error();

        m_model_shader.set_uniform_matrix("model", model);
        m_model_shader.set_uniform_matrix("view", view);
        check_for_opengl_error();

        m_model_shader.set_uniform_matrix("projection", projection);
        check_for_opengl_error();

        backpack_model.draw(m_model_shader);
        // tmodel.Draw(m_model_shader);

        check_for_opengl_error();
        //  close_window();
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
    }

    void on_mouse_move(double x_pos_in, double y_pos_in) override
    {
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

    void draw_basic_scene()
    {
        float angle = get_global_time();

        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        auto cube_model = glm::mat4(1.0f);
        cube_model = glm::rotate(cube_model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        cube_model = glm::translate(cube_model, glm::vec3(0.0f, sin(angle * 0.5f), 0.0f));
        cube_model = glm::scale(cube_model, glm::vec3(1.5f, 1.5f, 1.5f));

        m_vetex_and_color_shader.use();
        m_vetex_and_color_shader.set_uniform_matrix("model", cube_model);
        m_vetex_and_color_shader.set_uniform_matrix("view", view);
        m_vetex_and_color_shader.set_uniform_matrix("projection", projection);
        tetra.draw();

        auto plane_model = glm::mat4(1.0f);
        plane_model = glm::translate(plane_model, glm::vec3(-5.0f, -3.0f, -5.0f));
        plane_model = glm::rotate(plane_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_vetex_shader.use();
        m_vetex_shader.set_uniform_matrix("model", plane_model);
        m_vetex_shader.set_uniform_matrix("view", view);
        m_vetex_shader.set_uniform_matrix("projection", projection);
        m_vetex_shader.set_uniform_vec("color", glm::vec4(1.0f, 0.9f, 0.8f, 1.0f));
        plane.draw();

        auto sphere_model = glm::mat4(1.0f);
        sphere_model = glm::translate(sphere_model, glm::vec3(3.0f, 1.0f, -5.0f));
        m_vetex_shader.set_uniform_matrix("model", sphere_model);
        m_vetex_shader.set_uniform_vec("color", glm::vec4(0.8f, 1.0f, 0.9f, 1.0f));
        sphere.draw();

        auto line_model = glm::mat4(1.0f);
        m_vetex_shader.set_uniform_matrix("model", line_model);
        m_vetex_shader.set_uniform_vec("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        x_axis.draw();
        m_vetex_shader.set_uniform_vec("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        y_axis.draw();
        m_vetex_shader.set_uniform_vec("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        z_axis.draw();

        m_vetex_shader.set_uniform_vec("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        point.draw();
    }

    void draw_axes()
    {
        auto line_model = glm::mat4(1.0f);
        m_vetex_shader.set_uniform_matrix("model", line_model);
        m_vetex_shader.set_uniform_vec("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        x_axis.draw();
        m_vetex_shader.set_uniform_vec("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        y_axis.draw();
        m_vetex_shader.set_uniform_vec("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        z_axis.draw();
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Test")
        , m_width(width)
        , m_height(height)
        , m_vetex_and_color_shader("./test/basic.vert", "./test/basic.frag")
        , m_vetex_shader("./test/planeMesh.vert", "./test/planeMesh.frag")
        , m_light_shader("./test/lighting.vert", "./test/lighting.frag")
        , m_model_shader("./test/model.vert", "./test/model.frag")
        , backpack_model("./test/models/backpack/backpack.obj")
        // , tmodel("../test/models/backpack/backpack.obj")
    {
        set_mouse_capture(true);
        // set_wireframe_mode(true);

        //plane.load(10.0f, 10.0f, 5, 5);
        // sphere.load(2.0f, 20, 20);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);

        m_vetex_and_color_shader.compile();
        m_vetex_shader.compile();
        m_light_shader.compile();
        m_model_shader.compile();

        //std::vector<glm::vec3> point_vec = {
        //    glm::vec3(0.0f, 0.0f, 0.0f),
        //    glm::vec3(1.0f, 1.0f, 1.0f),
        //    glm::vec3(-1.0f, -1.0f, -1.0f),
        //    glm::vec3(1.0f, 3.0f, -4.0f),
        //};
        //point_vec.shrink_to_fit();
        //point.load(point_vec);
        //point.set_point_size(1.0f);

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));
        check_for_opengl_error();
    }
};

int main(int argc, char** argv)
{
stbi_set_flip_vertically_on_load(true);

    MyWindow w(800, 600);
    w.start();
    return 0;
}

/* ray tracer
 * fogs
 * sky box
 * text
 */