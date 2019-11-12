#pragma once

#include "../../Common.h"
#include "../../System.h"

#include "../Material/Light.h"
#include "../Material/Material.h"

#include "Mesh.h"

class Node
{
private:
    Transform       transform;
    Ref<Mesh>       mesh;
    Ref<Material>   material;

public:
    void setTransform(const Transform& transform);
    const Transform& getTransform() const;

    void setMesh(Ref<Mesh> mesh);
    Ref<Mesh> getMesh() const;

    void setMaterial(Ref<Material> material);
    Ref<Material> getMaterial() const;
};

struct Scene : public Layer
{
private:
    LightSetup      lights;
    Ref<Camera>     camera;
    
    std::vector<Ref<Node>> nodes;

public:
    Scene(Ref<Camera> camera);

    void setLights(const LightSetup& setup);

    void addNode(Ref<Node> node);
    void removeNode(Ref<Node> node);

    void onRender() override;
};