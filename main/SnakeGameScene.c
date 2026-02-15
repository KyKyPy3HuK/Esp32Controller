#ifndef __SNAKE_GAME_SCENE_C__
#define __SNAKE_GAME_SCENE_C__

#include "SnakeGameScene.h"
#include "utils.h"

Font8* snakeGameSceneFont;

Point2* snake;



void InitSnakeGameScene(Scene* scene, Font8* font) {
    scene->on_enter_scene = on_enter_SnakeGameScene;
    scene->on_exit_scene = on_exit_SnakeGameScene;
    scene->on_input_scene = on_input_SnakeGameScene;
    scene->on_update_scene = on_update_SnakeGameScene;
    snakeGameSceneFont = font;
};

void on_enter_SnakeGameScene(Scene* self, GlobalContext* context) {

};

void on_exit_SnakeGameScene(Scene* self, GlobalContext* context) {

};

void on_update_SnakeGameScene(Scene* self, GlobalContext* context) {

};

void on_input_SnakeGameScene(Scene* self, GlobalContext* context, int buttonId) {

};

#endif  //!__SNAKE_GAME_SCENE_C__