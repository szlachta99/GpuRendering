#include "App.hpp"

#include "Debug.hpp"
#include "Shader.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/quaternion.hpp"

struct UBO {
    glm::mat4 mvp;
};


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

    auto vert_shader = VertexShader("shaders/bin/shader.spv.vert", 0, 1, 0, 0);
    auto frag_shader = FragmentShader("shaders/bin/shader.spv.frag", 0, 0, 0, 0);

    GraphicsPipelineBuilder builder(m_gpu_device);
    builder.setVertexShader(vert_shader)
    .setFragmentShader(frag_shader)
    .setPrimitiveType(SDL_GPU_PRIMITIVETYPE_TRIANGLELIST)
    .addColorTargetDescription({
        .format = SDL_GetGPUSwapchainTextureFormat(m_gpu_device, m_window),
    });

    m_graphics_pipeline = builder.build();

    int window_width, window_height;
    SDL_GetWindowSize(m_window, &window_width, &window_height);

    m_camera = Camera(45.0f, (float)window_width / (float)window_height, 0.1f, 10.0f);

    m_camera.set_position(glm::vec3(0.0f, 0.0f, -2.0f));
    m_camera.set_rotation(glm::quatLookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    m_state = State::RUNNING;
}

void App::iter()
{
    auto cmd_buffer = SDL_AcquireGPUCommandBuffer(m_gpu_device);

    SDL_GPUTexture *swap_tex;
    SDL_AcquireGPUSwapchainTexture(cmd_buffer, m_window, &swap_tex, NULL, NULL);
    if(swap_tex != nullptr){
        SDL_GPUColorTargetInfo color_target = {
            .texture = swap_tex,
            .clear_color = {0.0f, 0.2f, 0.4f, 1.0f},
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto render_pass = SDL_BeginGPURenderPass(cmd_buffer, &color_target, 1, nullptr);
        
        glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), model_pos) * glm::rotate(glm::mat4(1.0f), (float)SDL_GetTicks() / 500.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        UBO uniform = {
            .mvp = m_camera.get_view_mat() * model
        };
        
        m_graphics_pipeline.bind(render_pass);
        SDL_PushGPUVertexUniformData(cmd_buffer, 0, &uniform, sizeof(UBO));

        SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
    
        SDL_EndGPURenderPass(render_pass);

    }

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
