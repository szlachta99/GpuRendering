#pragma once
#include <SDL3/SDL.h>
#include "GraphicsPipeline.hpp"
#include "Camera.hpp"

class App
{
public:
    enum State
    {
        RUNNING,
        EXIT,
        ERROR
    };

    App();
    void iter();
    void process_event(SDL_Event *event);
    void run();
    ~App();
    
    inline App::State getState() { return m_state; }
private:

    App::State m_state;
    SDL_Window *m_window;
    SDL_GPUDevice *m_gpu_device;
    GraphicsPipeline m_graphics_pipeline;
    Camera m_camera;
};