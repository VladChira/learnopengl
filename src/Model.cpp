#include "Model.hpp"

#include "SceneManager.hpp"

void Model::Init(std::string const &path)
{
    loadModel(path);
    resetTransform();
}

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw(shader, this);
}

void Model::loadModel(std::string const &path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_GenUVCoords);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    std::string name(scene->mRootNode->mName.C_Str());
    if (name == "")
        name = "No name";
    this->setName(name);

    // process all the materials
    std::vector<std::shared_ptr<PhongMaterial>> materials;
    for (int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial *_material = scene->mMaterials[i];
        auto material = std::make_shared<PhongMaterial>();
        std::string matName = std::string(_material->GetName().C_Str());
        if (matName == "")
            matName = "Material";
        material->setName(matName);
        SceneManager::GetInstance()->addMaterial(material);

        materials.push_back(material);
    }

    // process ASSIMP's root node recursively
    processNode(this, scene->mRootNode, glm::mat4(1), scene, materials);
}

void Model::processNode(Entity *entityNode, aiNode *node, glm::mat4 mat, const aiScene *scene, std::vector<std::shared_ptr<PhongMaterial>> materials)
{
    auto m = node->mTransformation;
    mat = glm::mat4(
              m.a1, m.a2, m.a3, m.a4,
              m.b1, m.b2, m.b3, m.b4,
              m.c1, m.c2, m.c3, m.c4,
              m.d1, m.d2, m.d3, m.d4) *
          mat;

    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        std::string name = std::string(mesh->mName.C_Str());

        std::shared_ptr<Mesh> meshChild = processMesh(mesh, mat, scene, materials);
        meshChild->setName(name);
        meshes.push_back(meshChild);

        entityNode->getChildren().push_back(meshChild);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        std::shared_ptr<Entity> nullChild = std::make_shared<Entity>(EntityType::NullObject);
        std::string name = std::string(node->mChildren[i]->mName.C_Str());
        nullChild->setName(name);
        entityNode->getChildren().push_back(nullChild);
        processNode(nullChild.get(), node->mChildren[i], mat, scene, materials);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *mesh, glm::mat4 mat, const aiScene *scene, std::vector<std::shared_ptr<PhongMaterial>> materials)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = glm::vec4(vector, 1) * mat;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = glm::vec4(vector, 1) * glm::transpose(glm::inverse(mat));
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
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
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial *_material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(_material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // // 2. specular maps
    // std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // // 3. normal maps
    // std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NORMAL);
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // // 4. height maps
    // std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::HEIGHT);
    // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    if (_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        materials[mesh->mMaterialIndex]->useDiffuseMap = true;
        materials[mesh->mMaterialIndex]->diffuseMap = diffuseMaps[0];
    }

    // return a mesh object created from the extracted mesh data
    auto newMesh = std::make_shared<Mesh>(vertices, indices, textures, materials[mesh->mMaterialIndex]);

    return newMesh;
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType ttype)
{
    std::vector<std::shared_ptr<Texture>> typeTextures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string s(str.C_Str());
        std::string filename = this->directory + "/" + s;

        typeTextures.push_back(TextureManager::GetInstance()->loadTexture(filename, ttype));
    }
    return typeTextures;
}