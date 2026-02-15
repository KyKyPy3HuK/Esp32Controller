#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "SceneManager.h"
#include "menu.h"
#include "drawing.h"

void InitMainMenuScene(Scene* scene, Font8* font);

void on_enter_scene(Scene* self, GlobalContext* context);

void on_exit_scene(Scene* self, GlobalContext* context);

void on_update_scene(Scene* self, GlobalContext* context);

void on_input_scene(Scene* self, GlobalContext* context, int buttonId);

#endif