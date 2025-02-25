#pragma once

#include <SDL3/SDL.h>
#include "Shader.hpp"
#include <vector>

class GraphicsPipeline;

class GraphicsPipelineBuilder
{
public:
    GraphicsPipelineBuilder(SDL_GPUDevice *device) : m_device(device) {SDL_zero(m_info);};
    GraphicsPipelineBuilder& setVertexShader(VertexShader&shader);
    GraphicsPipelineBuilder& setFragmentShader(FragmentShader &shader);
    GraphicsPipelineBuilder& setPrimitiveType(SDL_GPUPrimitiveType type);
    GraphicsPipelineBuilder& addColorTargetDescription(SDL_GPUColorTargetDescription &&desc);
    GraphicsPipelineBuilder& clearColorTargetDescriptions();
    GraphicsPipeline build();
private:
    SDL_GPUDevice *m_device;
    SDL_GPUGraphicsPipelineCreateInfo m_info;
    std::vector<SDL_GPUColorTargetDescription> m_color_target_descs;
};

class GraphicsPipeline
{
public:
    GraphicsPipeline() : m_device(nullptr), m_pipeline(nullptr) {};
    void bind(SDL_GPURenderPass *render_pass);
    void release();
    ~GraphicsPipeline();
private:
    SDL_GPUDevice *m_device;
    SDL_GPUGraphicsPipeline *m_pipeline;
    friend class GraphicsPipelineBuilder;
};