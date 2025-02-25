#pragma once
#include <SDL3/SDL.h>
#include <iostream>

template<SDL_GPUShaderStage stage>
class Shader 
{
public:
    Shader(
        const char* path,
        Uint32 samplerCount,
        Uint32 uniformBufferCount,
        Uint32 storageBufferCount,
        Uint32 storageTextureCount
    ) :
        m_path(path),
        m_samplerCount(samplerCount),
        m_uniformBufferCount(uniformBufferCount),
        m_storageBufferCount(storageBufferCount),
        m_storageTextureCount(storageTextureCount),
        m_shader(nullptr)
    {};
    
    ~Shader();
    template<SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_SPIRV>
    SDL_GPUShader* load(SDL_GPUDevice *device);
private:
    const char* m_path;
    Uint32 m_samplerCount;
    Uint32 m_uniformBufferCount;
    Uint32 m_storageBufferCount;
    Uint32 m_storageTextureCount;
    SDL_GPUDevice* m_device;
    SDL_GPUShader* m_shader;
};

template <SDL_GPUShaderStage stage>
inline Shader<stage>::~Shader()
{
    if(m_shader != NULL)
    {
        SDL_ReleaseGPUShader(m_device, m_shader);
    }
}

template <SDL_GPUShaderStage stage>
template <SDL_GPUShaderFormat format>
inline SDL_GPUShader *Shader<stage>::load(SDL_GPUDevice *device)
{
    if(m_shader != NULL)
    {
        SDL_ReleaseGPUShader(m_device, m_shader);
    }
    m_device = device;
    char* entrypoint;
    switch(format)
    {
        case SDL_GPU_SHADERFORMAT_SPIRV:
            entrypoint = "main";
            break;
        case SDL_GPU_SHADERFORMAT_MSL:
            entrypoint = "main0";
            break;
        case SDL_GPU_SHADERFORMAT_DXIL:
            entrypoint = "main";
            break;
        default:
            SDL_Log("Unknown shader format: %d", format);
            return NULL;
    }

    size_t code_length;
    Uint8* code = (Uint8*)SDL_LoadFile(m_path, &code_length);
    if(code == nullptr)
    {
        SDL_Log("Failed to load shader file: %s", m_path);
        return nullptr;
    }

    auto shaderInfo = (SDL_GPUShaderCreateInfo){
        .code_size = code_length,
		.code = code,
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = m_samplerCount,
		.num_storage_textures = m_storageTextureCount,
		.num_storage_buffers = m_storageBufferCount,
		.num_uniform_buffers = m_uniformBufferCount
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == nullptr)
	{
		SDL_Log("Failed to create shader!");
		SDL_free(code);
		return nullptr;
	}

    SDL_free(code);
    
    m_shader = shader;
    
    return shader;
}


typedef Shader<SDL_GPU_SHADERSTAGE_VERTEX> VertexShader;
typedef Shader<SDL_GPU_SHADERSTAGE_FRAGMENT> FragmentShader;