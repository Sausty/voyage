#include "sp_render_flow.h"

#include "../video/sp_video.h"

void sp_render_flow_init(sp_render_flow* flow)
{
    sp_forward_init(&flow->forward);
    sp_fxaa_init(&flow->fxaa);

    sp_buffer_create(&flow->update.scene_buffer, sizeof(glm::mat4) * 2, 0, sp_buffer_usage::uniform);
}   

void sp_render_flow_free(sp_render_flow* flow)
{
    sp_fxaa_free(&flow->fxaa);
    sp_forward_free(&flow->forward);
}

void sp_render_flow_update(sp_render_flow* flow)
{
    D3D11_VIEWPORT viewport{};
    viewport.Width = (FLOAT)sp_video_data.width;
    viewport.Height = (FLOAT)sp_video_data.height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    sp_video_data.device_ctx->RSSetViewports(1, &viewport);

    sp_buffer_set_data(&flow->update.scene_buffer, &flow->update.camera);
    sp_forward_update(&flow->forward, flow->update);
    sp_fxaa_update(&flow->fxaa, &flow->forward.rtv);
}

void sp_render_flow_render(sp_render_flow* flow)
{
    sp_video_data.device_ctx->CopyResource(sp_video_data.swap_chain_buffer, flow->fxaa.fxaa_texture.texture);
}

void sp_render_flow_resize(sp_render_flow* flow)
{
    sp_fxaa_resize(&flow->fxaa);
    sp_forward_resize(&flow->forward);
}