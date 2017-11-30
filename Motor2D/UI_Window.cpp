#include "UI_Window.h"
#include "p2Defs.h"

UI_Window::~UI_Window()
{
	p2List_item<UI_element*>* item;
	item = content.start;
	while (item)
	{
		RELEASE(item->data);
		item = item->next;
	}
	content.clear();
}

void UI_Window::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;
	content.add(child);
}

void UI_Window::BlitElement()
{
}
