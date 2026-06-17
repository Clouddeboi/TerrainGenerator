#pragma once

/*
    Framebuffer class

    Responsibilities:
    - Create and manage pixel-scaled framebuffer objects
    - Handle bind/unbind operations
    - Blit rendered content to screen with GL_NEAREST filtering
    - Handle window resize and FBO recreation

    Design:
    - Default pixel resolution: 480x270 (4x scale at 1920x1080)
    - PIXEL_SCALE constant controls density: 2 for sharp, 4 for retro, 8 for chunky
    - Colour attachment uses GL_NEAREST for crisp pixels
    - Depth+stencil as RBO for efficiency
*/

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    //Prevent copying — owns GPU resources
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    //Create pixel FBO with given window dimensions
    //Pixel resolution = windowW / PIXEL_SCALE x windowH / PIXEL_SCALE
    void Create(int windowWidth, int windowHeight);

    //Bind/unbind the FBO for rendering
    void Bind() const;
    void Unbind() const;

    //Blit FBO to screen (default framebuffer) with GL_NEAREST filtering
    void BlitToScreen(int windowWidth, int windowHeight) const;

    //Check if FBO needs recreation (e.g., window resized)
    bool NeedsRecreate(int windowWidth, int windowHeight) const;

    //Get current pixel resolution
    int GetPixelWidth() const { return m_PixelWidth; }
    int GetPixelHeight() const { return m_PixelHeight; }

    //Public access to colour texture for composite pass
    unsigned int GetColourTexture() const { return m_ColourTexture; }

private:
    //Clean up GPU resources
    void Destroy();

private:
    unsigned int m_FBO = 0;              //Framebuffer object
    unsigned int m_ColourTexture = 0;    //Colour attachment (GL_NEAREST)
    unsigned int m_DepthStencilRBO = 0;  //Depth+stencil renderbuffer

    int m_PixelWidth = 0;   //Current pixel resolution width
    int m_PixelHeight = 0;  //Current pixel resolution height
    int m_LastWindowWidth = 0;   //Track window size for resize detection
    int m_LastWindowHeight = 0;
};