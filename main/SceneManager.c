#ifndef __SCENE_MANAGER_C__

#define __SCENE_MANAGER_C__
#include "SceneManager.h"



/// @brief 
/// @param sceneManager менеджер сцен
/// @param context графический контекст 
void SceneManagerInit(SceneManager* sceneManager, void* context) {
    sceneManager->stackPointer = -1;
    sceneManager->context = context;
    memset(sceneManager->stackScenes, 0, sizeof(sceneManager->stackScenes));
}

void SceneManagerPushScene(SceneManager* sm, Scene* scene) {
    if (sm->stackPointer >= MAX_SCENE_DEPTH) {
        return;
    }

    sm->stackPointer++;
    sm->stackScenes[sm->stackPointer] = scene;
    
    if (scene->on_enter_scene) {
        scene->on_enter_scene(scene, sm->context);
    }
}

void SceneManagerPopScene(SceneManager* sm) {
    if (sm->stackPointer <= 0) {
        return;
    }

    Scene* currentScene = sm->stackScenes[sm->stackPointer];

    if (currentScene->on_exit_scene) {
        currentScene->on_exit_scene(currentScene, sm->context);
    }
    
    sm->stackScenes[sm->stackPointer] = NULL;
    sm->stackPointer--;
}


void SceneManagerReplaceScene(SceneManager* sm, Scene* newScene) {
    if (sm->stackPointer >= 0) {
        SceneManagerPopScene(sm);
    }
    SceneManagerPushScene(sm, newScene);
}

void SceneManagerDispose(SceneManager* sm) {

    while (sm->stackPointer > 0)
    {
        SceneManagerPopScene(sm);
    }

    free(sm->stackScenes);
}

Scene* SceneManagerGetScene(SceneManager* sm) {
    if (sm->stackPointer < 0)
    {
        return NULL;
    }
    
    return sm->stackScenes[sm->stackPointer];
}

void SceneManagerUpdate(SceneManager* sm) {
    Scene* scene = SceneManagerGetScene(sm);
    if (scene && scene->on_update_scene)
    {
        scene->on_update_scene(scene, sm->context);
    }
};

void SceneManagerHandleInput(SceneManager* sm, int button_id) {
    Scene* scene = SceneManagerGetScene(sm);
    if (scene && scene->on_input_scene) {
        scene->on_input_scene(scene, sm->context, button_id);
    }
};

#endif //!__SCENE_MANAGER_C__