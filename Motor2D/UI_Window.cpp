#include "UI_Window.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "j1Gui.h"

Window::~Window()
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

void Window::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;
	content.add(child);
	
}

void Window::BlitElement()
{
	App->render->Blit(texture, localPosition.x, localPosition.y, &section, false);
}
