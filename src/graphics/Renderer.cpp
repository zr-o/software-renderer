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
        ScreenTransformer::transformVector(vertex.pos, frameBuffer_.getWidth(), frameBuffer_.getHeight());
    }

    const std::size_t stride = mesh.isFormedOfTriangles ? 3 : 2;
    for (std::size_t i = 0; i + stride - 1 < mesh.indices.size(); i += stride)
    {
        const geometry::Vertex &firstVertex = vertices[mesh.indices[i]];
        const geometry::Vertex &secondVertex = vertices[mesh.indices[i + 1]];

        rasterizer_.drawLine(firstVertex, secondVertex, color);
        if (mesh.isFormedOfTriangles)
        {
            const geometry::Vertex &thirdVertex = vertices[mesh.indices[i + 2]];
            rasterizer_.drawLine(secondVertex, thirdVertex, color);
            rasterizer_.drawLine(thirdVertex, firstVertex, color);
        }
    }
}

void graphics::Renderer::drawTriangles(const geometry::Mesh &mesh)
{
    if (!mesh.isFormedOfTriangles) return;

    // Transform a copy so the original mesh can be reused next frame.
    auto vertices = mesh.vertices;
    for (auto &vertex : vertices)
    {
        ScreenTransformer::transformVector(vertex.pos, frameBuffer_.getWidth(), frameBuffer_.getHeight());
    }

    for (std::size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        const geometry::Vertex &firstVertex = vertices[mesh.indices[i]];
        const geometry::Vertex &secondVertex = vertices[mesh.indices[i + 1]];
        const geometry::Vertex &thirdVertex = vertices[mesh.indices[i + 2]];

       rasterizer_.drawTriangle(firstVertex, secondVertex, thirdVertex);
    }
}
