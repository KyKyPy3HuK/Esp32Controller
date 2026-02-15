
#ifndef __MAIN_MENU_SCENE_C__
#define __MAIN_MENU_SCENE_C__

#include "MainMenuScene.h"

volatile uint8_t red = 0;
volatile uint8_t green = 0;
volatile uint8_t blue = 0;

Font8* mainMenuSceneFont;


char redBuf[10];
char greenBuf[10];
char blueBuf[10];

volatile Menu menu;


void ColorInc(void){
    switch (MenuGetFocusItemIndex(menu))
    {
    case 0:
        red++;
        break;
    case 1:
        green++;
        break;
    case 2:
        blue++;
        break;
    default:
        break;
    }
}
void ColorDec(void){
    switch (MenuGetFocusItemIndex(menu))
    {
    case 0:
        red--;
        break;
    case 1:
        green--;
        break;
    case 2:
        blue--;
        break;
    default:
        break;
    }
}

void MenuConfig(void){
    MenuItem rMenuItem;
    MenuItemInit(&rMenuItem, "Red");
    MenuItem gMenuItem;
    MenuItemInit(&gMenuItem,"Green");
    MenuItem bMenuItem;
    MenuItemInit(&bMenuItem, "Blue");
    MenuAddItem(&menu, rMenuItem);
    MenuAddItem(&menu, gMenuItem);
    MenuAddItem(&menu, bMenuItem);
}

void DrawMenu(int xPos, int yPos, Font8* font, uint16_t colorBase, uint16_t colorSelected){
    for (int i = 0; i < menu.itemsCount; i++)
	{
		//Вывод текущего пункта меню (на который указвает курсор)
		if (i == menu.focusItemIndex)
		{
            DrawString(menu.items[i].text, font, xPos ,i * 10 + yPos, colorSelected);
		}
		else
		{
			DrawString(menu.items[i].text,font, xPos ,i * 10 + yPos, colorBase);
		}
        if (menu.items[i].isActivated == 1)
        {
            DrawString("v",font,xPos + (8 * strlen(menu.items[i].text)), i * 10 + yPos, colorBase);
        }
        
	}
}


void InitMainMenuScene(Scene* scene, Font8* font) {
    scene->on_enter_scene = on_enter_scene;
    scene->on_exit_scene = on_exit_scene;
    scene->on_input_scene = on_input_scene;
    scene->on_update_scene = on_update_scene;
    mainMenuSceneFont = font;
    MenuConfig();
}


void on_enter_scene(Scene* self, GlobalContext* context) {

}

void on_exit_scene(Scene* self, GlobalContext* context) {

}

void on_update_scene(Scene* self, GlobalContext* context) {
    DrawString("Colors", mainMenuSceneFont, 24, 0, 0xffff);
    DrawMenu(8, 8, mainMenuSceneFont, 0xaaaa,0xffff);

    itoa( red, redBuf, 10);
    itoa( green, greenBuf, 10);
    itoa( blue, blueBuf, 10);
    DrawString(redBuf, mainMenuSceneFont, 140, 8, 0xffff);
    DrawString(greenBuf, mainMenuSceneFont, 140, 16, 0xffff);
    DrawString(blueBuf, mainMenuSceneFont, 140, 24, 0xffff);
    draw_rect_to_buffer(0,60,160, 68, toRgb565(red,green,blue));
}

void on_input_scene(Scene* self, GlobalContext* context, int buttonId) {
     // Определяем какая кнопка нажата
     switch (buttonId) {
        case BUTTON_UP:
            MenuPrevItem(&menu);
            break;
        case BUTTON_DOWN:
            MenuNextItem(&menu);
            break;
        case BUTTON_LEFT:
        ColorDec();
            break;
        case BUTTON_RIGHT:
        ColorInc();
            break;
        case BUTTON_SELECT:
            red=0;
            green=0;
            blue=0;
            break;
        case BUTTON_RETURN:
            red=1;
            green=2;
            blue=3;
            break;
    }
}


#endif