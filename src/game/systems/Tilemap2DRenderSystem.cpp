#include "Tilemap2DRenderSystem.h"

#include "game/components/Camera.h"
#include "game/components/Position.h"
#include "game/components/RenderLayer.h"
#include "game/components/Texture.h"
#include "game/components/TextureAtlas.h"
#include "game/components/TextureTileMap.h"
#include "game/components/Transform.h"

#include <format>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

Tilemap2DRenderSystem::Tilemap2DRenderSystem(gfx::Window &window,
                                             gfx::Renderer &renderer,
                                             gfx::DrawCallQueue &drawCallQueue,
                                             float pixelsPerUnit)
    : window(window), renderer(renderer), drawCallQueue(drawCallQueue),
      pixelsPerUnit(pixelsPerUnit) {}

void Tilemap2DRenderSystem::fixedUpdate(ecs::Registry &,
                                        std::chrono::duration<float>) {}

void Tilemap2DRenderSystem::update(ecs::Registry &registry,
                                   std::chrono::duration<float>, float) {

  for (auto [e, transform, texture, textureAtlas, textureTileMap, renderLayer] :
       registry.view<Transform, Texture, TextureAtlas, TextureTileMap,
                     RenderLayer>()) {

    int screenWidth = window.getWidth();
    int screenHeight = window.getHeight();

    auto cameraPosition = glm::vec2{0, 0};
    float cameraZoom = 1.0f;

    if (auto result = registry.find<Camera, Position>()) {
      auto &[camera, position] = *result;
      cameraPosition.x = position.x;
      cameraPosition.y = position.y;
      cameraZoom = camera.zoom;
    }

    glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-cameraPosition * pixelsPerUnit * cameraZoom, 0.0f));

    glm::mat4 projection = glm::ortho(
        -(float)screenWidth * 0.5f, (float)screenWidth * 0.5f,
        -(float)screenHeight * 0.5f, (float)screenHeight * 0.5f, -1.0f, 1.0f);

    glm::mat4 VP = projection * view;

    int tileWidth =
        renderer.getTextureWidth(texture.textureId) / textureAtlas.cols;
    int tileHeight =
        renderer.getTextureHeight(texture.textureId) / textureAtlas.rows;

    for (int tileY = 0; tileY < textureTileMap.height; tileY++) {
      for (int tileX = 0; tileX < textureTileMap.width; tileX++) {

        glm::vec4 worldPosition(
            (transform.position.x + static_cast<float>(tileX)) * pixelsPerUnit *
                cameraZoom,
            (transform.position.y + static_cast<float>(tileY)) * pixelsPerUnit *
                cameraZoom,
            0.0f, 1.0f);

        glm::vec4 screenPos = VP * worldPosition;
        screenPos /= screenPos.w;

        gfx::Rect dstRect;
        dstRect.x = (screenPos.x * 0.5f + 0.5f) * (float)screenWidth;
        dstRect.y = (screenPos.y * 0.5f + 0.5f) * (float)screenHeight;
        dstRect.w = transform.size.x * pixelsPerUnit * cameraZoom;
        dstRect.h = transform.size.y * pixelsPerUnit * cameraZoom;

        int tileIndex =
            textureTileMap.tiles[tileY * textureTileMap.width + tileX];

        int col = tileIndex % textureAtlas.cols;
        int row = tileIndex / textureAtlas.cols;

        gfx::Rect srcRect;
        srcRect.x = static_cast<float>(col * tileWidth);
        srcRect.y = static_cast<float>(row * tileHeight);
        srcRect.w = static_cast<float>(tileWidth);
        srcRect.h = static_cast<float>(tileHeight);

        auto drawCall = gfx::DrawCall{.textureId{texture.textureId},
                                      .src{srcRect},
                                      .dst{dstRect},
                                      .renderLayer{1}};

        drawCallQueue.submit(drawCall);
      }
    }
  }
}
