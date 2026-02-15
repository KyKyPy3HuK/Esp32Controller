#ifndef __SNAKE_GAME_SCENE_H__
#define __SNAKE_GAME_SCENE_H__

#include "drawing.h"
#include "SceneManager.h"
#include "LinkedList.h"

void InitSnakeGameScene(Scene* scene, Font8* font);

void on_enter_SnakeGameScene(Scene* self, GlobalContext* context);

void on_exit_SnakeGameScene(Scene* self, GlobalContext* context);

void on_update_SnakeGameScene(Scene* self, GlobalContext* context);

void on_input_SnakeGameScene(Scene* self, GlobalContext* context, int buttonId);


#endif  //!__SNAKE_GAME_SCENE_H__