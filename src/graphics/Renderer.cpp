#include "graphics/Renderer.hpp"
#include "graphics/ScreenTransformer.hpp"
#include "math/Geometry.hpp"

graphics::Renderer::Renderer(unsigned int width, unsigned int height)
    : frameBuffer_(width, height), rasterizer_(frameBuffer_, width, height)
{
}

void graphics::Renderer::beginFrame(const Pixel &clearColor)
{
    frameBuffer_.clear(clearColor);
}

void graphics::Renderer::drawWireframe(const geometry::Mesh &mesh,
                                       const Color &color)
{
    // Transform a copy so the original mesh can be reused next frame.
    auto vertices = mesh.vertices;
    for (auto &vertex : vertices)
    {
        ScreenTransformer::transformVector(vertex.pos, frameBuffer_.getWidth(),
                                           frameBuffer_.getHeight());
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
    if (!mesh.isFormedOfTriangles)
        return;

    // Transform a copy so the original mesh can be reused next frame.
    auto vertices = mesh.vertices;
    for (auto &vertex : vertices)
    {
        ScreenTransformer::transformVector(vertex.pos, frameBuffer_.getWidth(),
                                           frameBuffer_.getHeight());
    }

    for (std::size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        geometry::Vertex firstVertex = vertices[mesh.indices[i]];
        geometry::Vertex secondVertex = vertices[mesh.indices[i + 1]];
        geometry::Vertex thirdVertex = vertices[mesh.indices[i + 2]];

        const float area = math::edgeFunction(math::Vec2f{firstVertex.pos},
                                              math::Vec2f{secondVertex.pos},
                                              math::Vec2f{thirdVertex.pos});

        // 1. Reject degenerate triangles
        if (area == 0.0f)
            continue;

        // 2. Backface culling
        if (usingBackfaceCulling_ && area < 0.0f)
            continue;

        // 3. Normalize winding for the rasterizer
        if (area < 0.0f)
            std::swap(secondVertex, thirdVertex);

        rasterizer_.drawTriangle(
            firstVertex, secondVertex, thirdVertex,
            [&](const graphics::Fragment &fragment)
            { frameBuffer_.putPixel(fragment.x, fragment.y, fragment.color); });
    }
}
