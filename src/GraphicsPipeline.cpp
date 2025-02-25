#include "GraphicsPipeline.hpp"

GraphicsPipelineBuilder& GraphicsPipelineBuilder::setVertexShader(VertexShader &shader)
{
    m_info.vertex_shader = shader.load(m_device);
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::setFragmentShader(FragmentShader &shader)
{
    m_info.fragment_shader = shader.load(m_device);
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::setPrimitiveType(SDL_GPUPrimitiveType type)
{
    m_info.primitive_type = type;
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::addColorTargetDescription(SDL_GPUColorTargetDescription &&desc)
{
    m_color_target_descs.push_back(desc);
    return *this;
}

GraphicsPipelineBuilder &GraphicsPipelineBuilder::clearColorTargetDescriptions()
{
    m_color_target_descs.clear();
    return *this;
}

GraphicsPipeline GraphicsPipelineBuilder::build()
{
    SDL_assert(m_info.vertex_shader != nullptr);
    SDL_assert(m_info.fragment_shader != nullptr);
    m_info.target_info.color_target_descriptions = m_color_target_descs.data();
    m_info.target_info.num_color_targets = m_color_target_descs.size();
    GraphicsPipeline pipeline;
    pipeline.m_device = m_device;
    pipeline.m_pipeline =  SDL_CreateGPUGraphicsPipeline(m_device, &m_info);
    if(pipeline.m_pipeline == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create graphics pipeline: %s", SDL_GetError());
    }
    return pipeline;
}

void GraphicsPipeline::bind(SDL_GPURenderPass *render_pass)
{
    SDL_BindGPUGraphicsPipeline(render_pass, m_pipeline);
}
