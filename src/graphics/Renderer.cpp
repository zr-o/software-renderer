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

    const std::size_t stride = mesh.isFormedOfTriangles ? 3 : 2;
    for (std::size_t i = 0; i + stride - 1 < mesh.indices.size(); i += stride)
    {
        const math::Vec3f &firstVertex = vertices[mesh.indices[i]];
        const math::Vec3f &secondVertex = vertices[mesh.indices[i + 1]];

        rasterizer_.drawLine(math::Vec2f(firstVertex), math::Vec2f(secondVertex), color);
        if (mesh.isFormedOfTriangles)
        {
            const math::Vec3f &thirdVertex = vertices[mesh.indices[i + 2]];
            rasterizer_.drawLine(math::Vec2f(secondVertex), math::Vec2f(thirdVertex), color);
            rasterizer_.drawLine(math::Vec2f(thirdVertex), math::Vec2f(firstVertex), color);
        }
    }
}

void graphics::Renderer::drawTriangles(const geometry::Mesh &mesh, const Pixel &color)
{
    if (!mesh.isFormedOfTriangles) return;

    // Transform a copy so the original mesh can be reused next frame.
    auto vertices = mesh.vertices;
    for (auto &vertex : vertices)
    {
        ScreenTransformer::transformVector(vertex, frameBuffer_.getWidth(), frameBuffer_.getHeight());
    }

    for (std::size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        const math::Vec3f &firstVertex = vertices[mesh.indices[i]];
        const math::Vec3f &secondVertex = vertices[mesh.indices[i + 1]];
        const math::Vec3f &thirdVertex = vertices[mesh.indices[i + 2]];

       rasterizer_.drawTriangle(math::Vec2f{firstVertex}, math::Vec2f{secondVertex}, math::Vec2f{thirdVertex}, color);
    }
}
