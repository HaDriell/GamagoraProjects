#include "Scene.h"

////////////////////////////////////////////////////////////////////////////////
// Node Functions //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Node::setTransform(const Transform& transform)
{
    this->transform = transform;
}

const Transform& Node::getTransform() const
{
    return transform;
}

void Node::setMesh(Ref<Mesh> mesh)
{
    this->mesh = mesh;
}

Ref<Mesh> Node::getMesh() const
{
    return mesh;
}

void Node::setMaterial(Ref<Material> material)
{
    this->material = material;
}

Ref<Material> Node::getMaterial() const
{
    return material;
}


////////////////////////////////////////////////////////////////////////////////
// Scene Functions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


Scene::Scene(Ref<Camera> camera) : Layer("Scene"), camera(camera)
{
}

void Scene::setLights(const LightSetup& setup)
{
    this->lights = setup;
}

void Scene::addNode(Ref<Node> node)
{
    nodes.push_back(node);
}

void Scene::removeNode(Ref<Node> node)
{
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it != nodes.end())
    {
        nodes.erase(it);
    }
}

void Scene::onRender()
{
    //Rendering
    for (Ref<Node>& node : nodes)
    {

        //Require Mesh & Material refs to render a Node
        Ref<Mesh> mesh = node->getMesh();
        if (!mesh) continue;
        Ref<Material> material = node->getMaterial();
        if (!material) continue;

        //Setup the Material
        material->bind();
        material->setupCamera(*camera);
        material->setupLights(lights);
        material->setupModelMatrix(node->getTransform().getMatrix());
        material->setupProperties();

        //Pipeline Setup & Draw Call
        Render::DrawIndexed(*mesh->getVertexArray(), *mesh->getIndexBuffer());
    }
}