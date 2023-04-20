/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <bit>
#include <array>
#include <iostream>
#include <algorithm>
#include <student/gpu.hpp>

typedef struct triangle {
    OutVertex point[3];
} triangle_t;


void perspectiveDivision(triangle_t* Triangle)
{

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Triangle->point[i].gl_Position[j] /= Triangle->point[i].gl_Position[3];
        }
    }
}

void viewportTransformation(Frame frame, triangle_t* Triangle)
{
    for (int p = 0; p < 3; p++)
    {
        float* x = &(Triangle->point[p].gl_Position[0]);
        *x = (*x * 0.5 + 0.5) * frame.width;

        float* y = &(Triangle->point[p].gl_Position[1]);
        *y = (*y * 0.5 + 0.5) * frame.height;
    }
}

long double sideLength(double x1, double y1, double x2, double y2)
{
    long double dx = x1 - x2;
    long double dy = y1 - y2;
    long double length = sqrt(pow(dx, 2) + pow(dy, 2));
    return length;
}

double triangleArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
    double circ = 0;

    long double a = sideLength(x1, y1, x2, y2);
    long double b = sideLength(x2, y2, x3, y3);
    long double c = sideLength(x3, y3, x1, y1);

    circ = (a + b + c) / 2.0;


    double area = sqrt(circ * (circ - a) * (circ - b) * (circ - c));

    return area;
}

void getBarycentrics(triangle_t Triangle, std::vector<double>& bar, double area, int x, int y)
{
    double a = (double)x + 0.5;
    double b = (double)y + 0.5;

    double a0 = triangleArea(Triangle.point[1].gl_Position[0], Triangle.point[1].gl_Position[1], \
        Triangle.point[2].gl_Position[0], Triangle.point[2].gl_Position[1], a, b);
    double a1 = triangleArea(Triangle.point[2].gl_Position[0], Triangle.point[2].gl_Position[1], \
        Triangle.point[0].gl_Position[0], Triangle.point[0].gl_Position[1], a, b);
    double a2 = triangleArea(Triangle.point[0].gl_Position[0], Triangle.point[0].gl_Position[1], \
        Triangle.point[1].gl_Position[0], Triangle.point[1].gl_Position[1], a, b);

    bar[0] = a0 / area;
    bar[1] = a1 / area;
    bar[2] = a2 / area;

}

void createFragment(InFragment* inFragment, triangle_t Triangle, Program prg, \
    std::vector<double> bar, double area, int x, int y)
{


    inFragment->gl_FragCoord.x = x + 0.5;
    inFragment->gl_FragCoord.y = y + 0.5;


    inFragment->gl_FragCoord.z = Triangle.point[0].gl_Position[2] * bar[0] + Triangle.point[1].gl_Position[2] * bar[1] + Triangle.point[2].gl_Position[2] * bar[2];

    double s = (bar[0] / Triangle.point[0].gl_Position[3]) + (bar[1] / Triangle.point[1].gl_Position[3]) + (bar[2] / Triangle.point[2].gl_Position[3]);

    bar[0] = (bar[0]) / (Triangle.point[0].gl_Position[3] * s);
    bar[1] = (bar[1]) / (Triangle.point[1].gl_Position[3] * s);
    bar[2] = (bar[2]) / (Triangle.point[2].gl_Position[3] * s);

    double res;

    for (int i = 0; i < maxAttributes; i++) {
        if (prg.vs2fs[i] == AttributeType::VEC3)
        {
            for (int i = 0; i < 3; i++)
            {
                res = 0;
                for (int j = 0; j < 3; j++)
                {
                    res += bar[j] * Triangle.point[j].attributes[0].v3[i];
                }

                inFragment->attributes[0].v3[i] = res;
            }
        }
        else if (prg.vs2fs[i] == AttributeType::VEC4)
        {
            for (int i = 0; i < 4; i++)
            {
                res = 0;
                for (int j = 0; j < 3; j++)
                {
                    res += bar[j] * Triangle.point[j].attributes[0].v4[i];
                }

                inFragment->attributes[0].v4[i] = res;
            }
        }
    }
}

float clampColor(float c)
{
    if (c > 1)
    {
        c = 1.0;
    }

    return c;
}

void perFragmentOperations(Frame* frame, OutFragment outFragment, int pix, float z)
{
    float r = clampColor(outFragment.gl_FragColor.r);
    float g = clampColor(outFragment.gl_FragColor.g);
    float b = clampColor(outFragment.gl_FragColor.b);
    float a = outFragment.gl_FragColor.w;

    if (a < 1)
    {
        r = clampColor((frame->color[pix * 4] / 255.f) * (1 - a) + (r) * (a));
        g = clampColor((frame->color[(pix * 4) + 1] / 255.f) * (1 - a) + (g) * (a));
        b = clampColor((frame->color[(pix * 4) + 2] / 255.f) * (1 - a) + (b) * (a));
    }

    if (z < frame->depth[pix])
    {
        if (a > 0.5)
        {
            frame->depth[pix] = z;
        }

        frame->color[pix * 4] = r * 255.f;
        frame->color[(pix * 4) + 1] = g * 255.f;
        frame->color[(pix * 4) + 2] = b * 255.f;
        return;
    }
}

void rasterize(Frame& frame, triangle_t const& Triangle, Program const& prg)
{
    int minX = std::min(std::min(Triangle.point[0].gl_Position[0], Triangle.point[1].gl_Position[0]), Triangle.point[2].gl_Position[0]);
    int minY = std::min(std::min(Triangle.point[0].gl_Position[1], Triangle.point[1].gl_Position[1]), Triangle.point[2].gl_Position[1]);

    int maxX = std::max(std::max(Triangle.point[0].gl_Position[0], Triangle.point[1].gl_Position[0]), Triangle.point[2].gl_Position[0]);
    int maxY = std::max(std::max(Triangle.point[0].gl_Position[1], Triangle.point[1].gl_Position[1]), Triangle.point[2].gl_Position[1]);

    minX = std::max(0, minX);
    minY = std::max(0, minY);

    maxX = (((frame.width - 1) < (maxX)) ? (frame.width - 1) : (maxX));
    maxY = (((frame.height - 1) < (maxY)) ? (frame.height - 1) : (maxY));


    double deltaX1 = Triangle.point[1].gl_Position[0] - Triangle.point[0].gl_Position[0];
    double deltaX2 = Triangle.point[2].gl_Position[0] - Triangle.point[1].gl_Position[0];
    double deltaX3 = Triangle.point[0].gl_Position[0] - Triangle.point[2].gl_Position[0];

    double deltaY1 = Triangle.point[1].gl_Position[1] - Triangle.point[0].gl_Position[1];
    double deltaY2 = Triangle.point[2].gl_Position[1] - Triangle.point[1].gl_Position[1];
    double deltaY3 = Triangle.point[0].gl_Position[1] - Triangle.point[2].gl_Position[1];

    double edgeF1 = (minY - (Triangle.point[0].gl_Position[1] - 0.5)) * deltaX1 - (minX - (Triangle.point[0].gl_Position[0] - 0.5)) * deltaY1;
    double edgeF2 = (minY - (Triangle.point[1].gl_Position[1] - 0.5)) * deltaX2 - (minX - (Triangle.point[1].gl_Position[0] - 0.5)) * deltaY2;
    double edgeF3 = (minY - (Triangle.point[2].gl_Position[1] - 0.5)) * deltaX3 - (minX - (Triangle.point[2].gl_Position[0] - 0.5)) * deltaY3;

    double area = triangleArea(Triangle.point[0].gl_Position[0], Triangle.point[0].gl_Position[1], Triangle.point[1].gl_Position[0], \
        Triangle.point[1].gl_Position[1], Triangle.point[2].gl_Position[0], Triangle.point[2].gl_Position[1]);

    std::vector<double> barycentrics(3);
    int pix;

    for (int y = minY; y <= maxY; y++) {
        bool even = ((y - minY) % 2 == 0);
        for (int x = ((even) ? minX : maxX); (even) ? (x <= maxX) : \
            (x >= minX); x += (even) ? 1 : -1) {
            if (edgeF1 >= 0 && edgeF2 >= 0 && edgeF3 >= 0) {
                InFragment inFragment;

                getBarycentrics(Triangle, barycentrics, area, x, y);
                createFragment(&inFragment, Triangle, prg, barycentrics, area, x, y);

                OutFragment outFragment;
                prg.fragmentShader(outFragment, inFragment, prg.uniforms);

                pix = y * frame.width + x;
                perFragmentOperations(&frame, outFragment, pix, inFragment.gl_FragCoord.z);
            }

            if (!((even && x == maxX) || (!even && x == minX))) {
                edgeF1 += (even) ? -deltaY1 : deltaY1;
                edgeF2 += (even) ? -deltaY2 : deltaY2;
                edgeF3 += (even) ? -deltaY3 : deltaY3;
            }
        }
        edgeF1 += deltaX1;
        edgeF2 += deltaX2;
        edgeF3 += deltaX3;
    }
}



void  VertexAssembly(GPUContext& ctx, InVertex& in)
{
    for (uint32_t i = 0; i < maxAttributes; i++)
    {
        const auto& att = ctx.vao.vertexAttrib[i];
        auto index = att.offset + in.gl_VertexID * att.stride;
        const auto& buffer = (uint8_t*)(att.bufferData) + index;
        switch (att.type)
        {
        case AttributeType::VEC4:
            in.attributes[i].v4 = *(glm::vec4*)(buffer);
            continue;
        case AttributeType::VEC3:
            in.attributes[i].v3 = *(glm::vec3*)(buffer);
            continue;
        case AttributeType::VEC2:
            in.attributes[i].v2 = *(glm::vec2*)(buffer);
            continue;
        case AttributeType::FLOAT:
            in.attributes[i].v1 = *(float*)(buffer);
            continue;
        case AttributeType::EMPTY:
            continue;
        }
    }
}

uint32_t computeVertexID(VertexArray const& vao, uint32_t shaderInvocation) {
    if (!vao.indexBuffer)return shaderInvocation;

    if (vao.indexType == IndexType::UINT32) {
        uint32_t* ind = (uint32_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }
    if (vao.indexType == IndexType::UINT16) {
        uint16_t* ind = (uint16_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }
    if (vao.indexType == IndexType::UINT8) {
        uint8_t* ind = (uint8_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }

}

auto VertexFetch(GPUContext& ctx, uint32_t index)
{
    InVertex in;
    in.gl_VertexID = computeVertexID(ctx.vao, index);
    VertexAssembly(ctx, in);
    return in;

}

void PrimitiveAssembly(GPUContext& ctx, triangle_t Triangle, uint32_t index)
{
    for (int i = 0; i < 3; i++)
    {
        InVertex in = VertexFetch(ctx, index + i);
        ctx.prg.vertexShader(Triangle.point[i], in, ctx.prg.uniforms);
    }
}

//! [drawImpl]
void drawImpl(GPUContext& ctx, uint32_t nofVertices) {
    /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
    /// ctx obsahuje aktuální stav grafické karty.
    /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
    /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
    (void)ctx;
    (void)nofVertices;

    for (uint32_t i = 0; i < nofVertices; i += 3) {//smyčka přes vrcholy
        triangle_t Triangle;
        PrimitiveAssembly(ctx, Triangle, i);

        perspectiveDivision(&Triangle);
        viewportTransformation(ctx.frame, &Triangle);
        rasterize(ctx.frame, Triangle, ctx.prg);
    }

}
//! [drawImpl]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const& texture, glm::vec2 uv) {
    if (!texture.data)return glm::vec4(0.f);
    auto uv1 = glm::fract(uv);
    auto uv2 = uv1 * glm::vec2(texture.width - 1, texture.height - 1) + 0.5f;
    auto pix = glm::uvec2(uv2);
    //auto t   = glm::fract(uv2);
    glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f);
    for (uint32_t c = 0; c < texture.channels; ++c)
        color[c] = texture.data[(pix.y * texture.width + pix.x) * texture.channels + c] / 255.f;
    return color;
}

/**
 * @brief This function clears framebuffer.
 *
 * @param ctx GPUContext
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void clear(GPUContext& ctx, float r, float g, float b, float a) {
    auto& frame = ctx.frame;
    auto const nofPixels = frame.width * frame.height;
    for (size_t i = 0; i < nofPixels; ++i) {
        frame.depth[i] = 10e10f;
        frame.color[i * 4 + 0] = static_cast<uint8_t>(glm::min(r * 255.f, 255.f));
        frame.color[i * 4 + 1] = static_cast<uint8_t>(glm::min(g * 255.f, 255.f));
        frame.color[i * 4 + 2] = static_cast<uint8_t>(glm::min(b * 255.f, 255.f));
        frame.color[i * 4 + 3] = static_cast<uint8_t>(glm::min(a * 255.f, 255.f));
    }
}

