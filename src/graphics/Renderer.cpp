#include "graphics/Renderer.hpp"
#include "graphics/ScreenTransformer.hpp"

graphics::Renderer::Renderer(unsigned int width, unsigned int height)
    : frameBuffer_(width, height), rasterizer_(frameBuffer_)
{
}

void graphics::Renderer::beginFrame(const Pixel &clearColor)
{
    frameBuffer_.clear(clearColor);
}

void graphics::Renderer::drawWireframe(const geometry::Mesh &mesh, const Pixel &color)
{
    // Transform a copy so the original mesh can be reused next frame.
    auto vertices = mesh.vertices;
    for (auto &vertex : vertices)
    {
        ScreenTransformer::transformVector(vertex, frameBuffer_.getWidth(), frameBuffer_.getHeight());
    }

    for (std::size_t i = 0; i + 1 < mesh.indices.size(); i += 2)
    {
        const math::Vec3f &firstVertex = vertices[mesh.indices[i]];
        const math::Vec3f &secondVertex = vertices[mesh.indices[i + 1]];

        rasterizer_.drawLine(static_cast<unsigned>(firstVertex.elements[0]),
                             static_cast<unsigned>(firstVertex.elements[1]),
                             static_cast<unsigned>(secondVertex.elements[0]),
                             static_cast<unsigned>(secondVertex.elements[1]), color);
    }
}
