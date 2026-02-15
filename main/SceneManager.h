#ifndef __SCENE_MANAGER_H__

#define __SCENE_MANAGER_H__

#define MAX_SCENE_DEPTH 8

#include "Button.h"
#include <string.h>

typedef struct Scene Scene;
typedef struct GlobalContext GlobalContext;

typedef struct Scene
{
    void (*on_enter_scene)(Scene* self, GlobalContext* context);
    void (*on_exit_scene)(Scene* self, GlobalContext* context);
    void (*on_update_scene)(Scene* self, GlobalContext* context);
    void (*on_input_scene)(Scene* self, GlobalContext* context, int buttonId);

    void* data;
    /* data */
} Scene;

typedef struct SceneManager
{
    Scene* stackScenes[MAX_SCENE_DEPTH];
    int stackPointer;
    struct GlobalContext* context;

} SceneManager;


typedef struct GlobalContext
{
    SceneManager* sceneManager;
} GlobalContext;


void SceneManagerInit(SceneManager* sceneManager, void* context);

void SceneManagerUpdate(SceneManager* sm);

void SceneManagerHandleInput(SceneManager* sm, int button_id);

void SceneManagerPushScene(SceneManager* sm, Scene* scene);

void SceneManagerPopScene(SceneManager* sm);

void SceneManagerReplace(SceneManager* sm, Scene* scene);

Scene* SceneManagerGetScene(SceneManager* sm);

void SceneManagerDispose(SceneManager* sm);

#endif //!__SCENE_MANAGER_H__