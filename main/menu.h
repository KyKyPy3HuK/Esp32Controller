#ifndef __MENU_H__
#define __MENU_H__

#include "stdint.h"

//Структура Пункт меню
typedef struct {
	//Статус
	uint8_t isActivated;
	//Текст
	char* text;
} MenuItem;

//Структура Меню
typedef struct {
	//Колво элементов
	int itemsCount;
	//Текущий элемент
	int focusItemIndex;
	//Элементы
	MenuItem* items;
} Menu;

void MenuInit(Menu* menu);

void MenuSelectedItemTrigger(Menu *menu);

void MenuPrevItem(Menu* menu);

int MenuGetFocusItemIndex(Menu menu);

int MenuGetItemStatus(Menu menu, int index);

void MenuNextItem(Menu* menu);

void MenuAddItem(Menu* menu, MenuItem menuItem);

void MenuItemInit(MenuItem *menuitem, char* text);

#endif