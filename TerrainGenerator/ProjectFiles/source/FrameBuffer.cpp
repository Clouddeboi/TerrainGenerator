#include <glad/glad.h>

#include "../headers/Framebuffer.hpp"
#include <iostream>

//Pixel scale: 4 = retro feel, 2 = sharper, 8 = chunkier
constexpr int PIXEL_SCALE = 2;

Framebuffer::Framebuffer()
{
}

Framebuffer::~Framebuffer()
{
    Destroy();
}

void Framebuffer::Create(int windowWidth, int windowHeight)
{
    //Clean up any existing FBO FIRST before recalculating dimensions
    //Destroy() resets m_PixelWidth/Height to 0, so it must run before we set them
    Destroy();

    //Store for resize detection
    m_LastWindowWidth = windowWidth;
    m_LastWindowHeight = windowHeight;

    //Calculate pixel resolution
    m_PixelWidth = windowWidth / PIXEL_SCALE;
    m_PixelHeight = windowHeight / PIXEL_SCALE;

    //Clamp to at least 1x1
    if (m_PixelWidth < 1) m_PixelWidth = 1;
    if (m_PixelHeight < 1) m_PixelHeight = 1;

    //Create framebuffer object
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    //Create colour texture with GL_NEAREST filtering (pixel-perfect)
    glGenTextures(1, &m_ColourTexture);
    glBindTexture(GL_TEXTURE_2D, m_ColourTexture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB8,
        m_PixelWidth,
        m_PixelHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr);

    //GL_NEAREST = pixel-perfect, no interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Attach colour texture to FBO
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_ColourTexture,
        0);

    //Create depth+stencil renderbuffer (combined for efficiency)
    glGenRenderbuffers(1, &m_DepthStencilRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRBO);

    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        m_PixelWidth,
        m_PixelHeight);

    //Attach depth+stencil RBO to FBO
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_DepthStencilRBO);

    //Verify FBO is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    std::cout << "FBO created: " << m_FBO
        << ", Texture: " << m_ColourTexture
        << ", RBO: " << m_DepthStencilRBO
        << ", Dims: " << m_PixelWidth << "x" << m_PixelHeight
        << std::endl;

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FRAMEBUFFER ERROR: Status code " << status << std::endl;
    }

    //Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glViewport(0, 0, m_PixelWidth, m_PixelHeight);
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::BlitToScreen(int windowWidth, int windowHeight) const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);

    glViewport(0, 0, windowWidth, windowHeight);

    glBlitFramebuffer(
        0, 0, m_PixelWidth, m_PixelHeight,
        0, 0, windowWidth, windowHeight,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

bool Framebuffer::NeedsRecreate(int windowWidth, int windowHeight) const
{
    return (windowWidth != m_LastWindowWidth) ||
        (windowHeight != m_LastWindowHeight);
}

void Framebuffer::Destroy()
{
    if (m_FBO != 0)
    {
        glDeleteFramebuffers(1, &m_FBO);
        m_FBO = 0;
    }

    if (m_ColourTexture != 0)
    {
        glDeleteTextures(1, &m_ColourTexture);
        m_ColourTexture = 0;
    }

    if (m_DepthStencilRBO != 0)
    {
        glDeleteRenderbuffers(1, &m_DepthStencilRBO);
        m_DepthStencilRBO = 0;
    }

    m_PixelWidth = 0;
    m_PixelHeight = 0;
}