#include "Material.h"



Material::Material(Ref<Shader> shader, const RenderPipeline& pipeline) : shader(shader), pipeline(pipeline)
{
}


void Material::bind()
{
    shader->bind();
    onBind(shader);
    Render::ConfigurePipeline(pipeline);
}

void Material::unbind()
{
    shader->unbind();
}