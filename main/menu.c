#ifndef __MENU_C__

#define __MENU_C__
#include "menu.h"
#include <stdio.h>

//Инициализация структуры Меню
void MenuInit(Menu* menu) {
	menu->focusItemIndex = 0;
	menu->itemsCount = 0;
	menu->items = 0;
}

//Переключить сотояние текущего пункта меню
void MenuSelectedItemTrigger(Menu *menu){
	int index = menu->focusItemIndex;
	if( menu->items[index].isActivated == 0){
		menu->items[index].isActivated = 1;
	}
	else{
		menu->items[index].isActivated = 0;
	}
	
}

//Переключить предыдущий пункт меню
void MenuPrevItem(Menu* menu) {
	menu->focusItemIndex--;
	if (menu->focusItemIndex < 0)
	{
		menu->focusItemIndex = menu->itemsCount - 1;
	}
}

//Получить индекс выбранного пункта
int MenuGetFocusItemIndex(Menu menu) {
	return menu.focusItemIndex;
}

//Получить состояние пункта меню по индексу
int MenuGetItemStatus(Menu menu, int index){
	return menu.items[index].isActivated;
}

//Переключить следующий пункт меню
void MenuNextItem(Menu* menu) {
	menu->focusItemIndex++;
	if (menu->focusItemIndex > menu->itemsCount - 1)
	{
		menu->focusItemIndex = 0;
	}
}

/**
 * Добавить пункт меню
 */
void MenuAddItem(Menu* menu, MenuItem menuItem) {
	//Изменение свойств
	menu->itemsCount++;
	int n = menu->itemsCount;
	MenuItem* newItems = calloc(n, sizeof(MenuItem));

	//Копирование старых элементов
	for (int i = 0; i < n - 1; ++i) {
		newItems[i] = menu->items[i];
	}

	//Новый элемент
	newItems[n - 1] = menuItem;
	free(menu->items);
	menu->items = newItems;
}

/**
 * Инициализация пункта меню
 */
void MenuItemInit(MenuItem *menuitem, char* text){
	menuitem->isActivated = 0;
	menuitem->text = text;
}

#endif