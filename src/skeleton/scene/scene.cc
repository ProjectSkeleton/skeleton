#include "scene.hh"

namespace Skeleton {

Scene::Scene(std::shared_ptr<Delta::Instance> delta, std::shared_ptr<Delta::RenderTarget> final_render_target) {
  scene_frame_buffer_ = delta->CreateFrameBuffer();
  final_render_target->SetResizeCallback([this](int w, int h){
    scene_frame_buffer_->Resize(w, h);
  });

  Delta::ShaderInfo first_pass_shader_info;
  first_pass_shader_info.render_target = scene_frame_buffer_;
  first_pass_shader_info.spv_paths.vert_path = "spv/pbr_first_pass.vert.spv";
  first_pass_shader_info.spv_paths.frag_path = "spv/pbr_first_pass.frag.spv";
  auto first_pass_shader = delta->CreateShader(first_pass_shader_info);

  Delta::RenderPassInfo first_pass_render_pass_info;
  first_pass_render_pass_info.render_target = scene_frame_buffer_;
  first_pass_render_pass_info.clear_color = { 0.2f, 0.6f, 0.4f, 1.0f };
  first_pass_render_pass_info.on_begin = Delta::RenderPassBeginOp::kClear;
  first_pass_render_pass_info.on_complete = Delta::RenderPassCompleteOp::kPreserveExistingContents;
  first_pass_render_pass_ = delta->CreateRenderPass(first_pass_render_pass_info);

  Delta::ShaderInfo post_process_shader_info;
  post_process_shader_info.render_target = final_render_target;
  post_process_shader_info.spv_paths.vert_path = "spv/pbr_post_process.vert.spv";
  post_process_shader_info.spv_paths.frag_path = "spv/pbr_post_process.frag.spv";
  auto post_process_shader = delta->CreateShader(post_process_shader_info);
  post_process_shader->GetSampler2d("u_frame_buffer")->SetTexture(scene_frame_buffer_);

  Delta::MeshInfo viewport_mesh_info;
  viewport_mesh_info.vertex_layout = post_process_shader->GetVertexInputLayout();
  viewport_mesh_info.vertices = {
  // x      y     z     u     v
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
  };
  viewport_mesh_info.indices = {
    0, 1, 3,
    1, 2, 3,
  };
  auto viewport_mesh = delta->CreateMesh(viewport_mesh_info);

  Delta::RenderPassInfo post_process_render_pass_info;
  post_process_render_pass_info.render_target = final_render_target;
  post_process_render_pass_info.enable_depth_test = false;
  post_process_render_pass_info.on_begin = Delta::RenderPassBeginOp::kDontCare;
  post_process_render_pass_info.on_complete = Delta::RenderPassCompleteOp::kSwapchainPresent;
  post_process_render_pass_ = delta->CreateRenderPass(post_process_render_pass_info);
  post_process_render_pass_->RecordBindShaderCommand(post_process_shader);
  post_process_render_pass_->RecordRenderMeshCommand(viewport_mesh);
}

void Scene::Render() {
  first_pass_render_pass_->Execute();
  post_process_render_pass_->Execute();
}

}
