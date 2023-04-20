/*!
 * @file
 * @brief This file contains functions for model rendering
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <student/drawModel.hpp>
#include <student/gpu.hpp>


void drawNode(GPUContext& ctx, Model const& model, Node const& node, glm::mat4 matrix) {
    matrix = matrix * node.modelMatrix;

    if (node.mesh >= 0) {
        ctx.prg.uniforms.uniform[1].m4 = matrix;
        ctx.prg.uniforms.uniform[2].m4 = glm::transpose(glm::inverse(matrix));
        const Mesh& mesh = model.meshes[node.mesh];
        ctx.vao.indexBuffer = mesh.indices;
        ctx.vao.indexType = mesh.indexType;  
        ctx.vao.vertexAttrib[0] = mesh.position;
        ctx.vao.vertexAttrib[1] = mesh.normal;
        ctx.vao.vertexAttrib[2] = mesh.texCoord;
        if (mesh.diffuseTexture >= 0) {
            ctx.prg.uniforms.uniform[6].v1 = 1.0;
            ctx.prg.uniforms.textures[0] = model.textures[mesh.diffuseTexture];
            ctx.vao.vertexAttrib[2] = mesh.texCoord;
        }
        else
        {
            ctx.prg.uniforms.uniform[6].v1 = 0.0;
            ctx.prg.uniforms.textures[0] = Texture{};
            ctx.prg.uniforms.uniform[5].v4 = mesh.diffuseColor;
        }

       draw(ctx, mesh.nofIndices);
    }

    for (size_t i = 0; i < node.children.size(); ++i)
        drawNode(ctx, model,  node.children[i], matrix);
}

/**
 * @brief This function renders a model
 *
 * @param ctx GPUContext
 * @param model model structure
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position (unused)
 */
//! [drawModel]
void drawModel(GPUContext&ctx,Model const&model,glm::mat4 const&proj,glm::mat4 const&view,glm::vec3 const&light,glm::vec3 const&camera){
  (void)ctx;
  (void)model;
  (void)proj;
  (void)view;
  (void)light;
  (void)camera;
  /// \todo Tato funkce vykreslí model.<br>
  /// Vaším úkolem je správně projít model a vykreslit ho pomocí funkce draw (nevolejte drawImpl, je to z důvodu testování).
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  
  ctx.backfaceCulling = false;
  ctx.prg.uniforms.uniform[0].m4 = proj * view;
  ctx.prg.uniforms.uniform[3].v3 = light;
  ctx.prg.uniforms.uniform[4].v3 = camera;
  ctx.prg.vertexShader = drawModel_vertexShader;
  ctx.prg.fragmentShader = drawModel_fragmentShader;
  ctx.prg.vs2fs[0] = AttributeType::VEC3;
  ctx.prg.vs2fs[1] = AttributeType::VEC3;
  ctx.prg.vs2fs[2] = AttributeType::VEC2;

  for (size_t i = 0; i < model.roots.size(); i++)
  {
	  drawNode(ctx, model, model.roots[i], glm::mat4(1.f));
  }

}
//! [drawModel]

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
//! [drawModel_vs]
void drawModel_vertexShader(OutVertex&outVertex,InVertex const&inVertex,Uniforms const&uniforms){
  (void)outVertex;
  (void)inVertex;
  (void)uniforms;
  /// \todo Tato funkce reprezentujte vertex shader.<br>
  /// Vaším úkolem je správně trasnformovat vrcholy modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  
  const glm::mat4& projvieW = uniforms.uniform[0].m4;
  const glm::mat4& model = uniforms.uniform[1].m4;
  const glm::mat4& inv = uniforms.uniform[2].m4;
  outVertex.attributes[0].v3 = model * glm::vec4(inVertex.attributes[0].v3, 1.f);
  outVertex.attributes[1].v3 = inv * glm::vec4(inVertex.attributes[1].v3, 1.f);
  outVertex.attributes[2].v2 = inVertex.attributes[2].v2;
  outVertex.gl_Position = projvieW * glm::vec4(outVertex.attributes[0].v3, 1.f);

}
//! [drawModel_vs]

/**
 * @brief This functionrepresents fragment shader of texture rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
//! [drawModel_fs]
void drawModel_fragmentShader(OutFragment&outFragment,InFragment const&inFragment,Uniforms const&uniforms){
  (void)outFragment;
  (void)inFragment;
  (void)uniforms;
  /// \todo Tato funkce reprezentujte fragment shader.<br>
  /// Vaším úkolem je správně obarvit fragmenty a osvětlit je pomocí lambertova osvětlovacího modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  

  static constexpr float x = 0.2;
  const glm::vec4& y = (uniforms.uniform[6].v1 == 0) ? uniforms.uniform[5].v4 : read_texture(uniforms.textures[0], inFragment.attributes[2].v2);
  const glm::vec3& p = inFragment.attributes[0].v3;
  glm::vec3 L = glm::normalize(uniforms.uniform[3].v3 - p);
  glm::vec3 N = glm::normalize(inFragment.attributes[1].v3);

  if (uniforms.uniform[7].v1 > 0.0 && glm::dot(N, uniforms.uniform[4].v3 - p) < 0.0)
  {
      N *= -1.0;
  }
  float z = glm::clamp(glm::dot(L, N), 0.f, 1.f);

  glm::vec3 aL = x * y;
  glm::vec3 dL = z * y;

  outFragment.gl_FragColor = glm::vec4(aL + dL, y.a);
}
//! [drawModel_fs]

