#pragma once

#include <delta/delta.hh>

namespace Skeleton {

class Scene {
public:
  Scene(std::shared_ptr<Delta::Instance> delta, std::shared_ptr<Delta::RenderTarget> final_render_target);

public:
  void Render();

private:
  std::shared_ptr<Delta::FrameBuffer> scene_frame_buffer_;

  std::shared_ptr<Delta::RenderPass> first_pass_render_pass_;
  std::shared_ptr<Delta::RenderPass> post_process_render_pass_;
};

}
