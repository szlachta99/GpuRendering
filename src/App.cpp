#include "App.hpp"

#include "Debug.hpp"
#include "Shader.hpp"

App::App()
{
    setup_debug();

    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow("GPU Renderer", 1280, 780, 0);

    m_gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);

    if(m_gpu_device == NULL)
    {
        SDL_Log("Failed to create GPU device: %s", SDL_GetError());
        m_state = State::ERROR;
        return;
    }

    SDL_ClaimWindowForGPUDevice(m_gpu_device, m_window);

    auto vert_shader = Shader<SDL_GPU_SHADERSTAGE_VERTEX>("shaders/bin/shader.spv.vert", 0, 0, 0, 0);
    auto frag_shader = Shader<SDL_GPU_SHADERSTAGE_FRAGMENT>("shaders/bin/shader.spv.frag", 0, 0, 0, 0);

    GraphicsPipelineBuilder builder(m_gpu_device);
    builder.setVertexShader(vert_shader)
    .setFragmentShader(frag_shader)
    .setPrimitiveType(SDL_GPU_PRIMITIVETYPE_TRIANGLELIST)
    .addColorTargetDescription({
        .format = SDL_GetGPUSwapchainTextureFormat(m_gpu_device, m_window),
    });

    m_graphics_pipeline = builder.build();

    m_state = State::RUNNING;
}

void App::iter()
{
    auto cmd_buffer = SDL_AcquireGPUCommandBuffer(m_gpu_device);

    SDL_GPUTexture *swap_tex;
    SDL_AcquireGPUSwapchainTexture(cmd_buffer, m_window, &swap_tex, NULL, NULL);
    
    SDL_GPUColorTargetInfo color_target = {
        .texture = swap_tex,
        .clear_color = {0.0f, 0.2f, 0.4f, 1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    auto render_pass = SDL_BeginGPURenderPass(cmd_buffer, &color_target, 1, nullptr);
    
    m_graphics_pipeline.bind(render_pass);
    SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);

    SDL_EndGPURenderPass(render_pass);

    SDL_SubmitGPUCommandBuffer(cmd_buffer);
}

void App::process_event(SDL_Event *event)
{
    if(event->type == SDL_EVENT_QUIT)
    {
        m_state = State::EXIT;
    }
}

void App::run()
{
    SDL_Event event;
    while(m_state == App::State::RUNNING)
    {
        iter();
        while(SDL_PollEvent(&event))
        {
            process_event(&event);
        }
    }
}

App::~App()
{
    SDL_DestroyWindow(m_window);
}
