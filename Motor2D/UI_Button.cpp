#include "UI_Button.h"
#include "j1App.h"
#include "j1Render.h"

Button::~Button()
{
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void Button::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void Button::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;
	text = (Text*)child;
}

void Button::BlitElement()
{
	switch (state)
	{
	case STANDBY:
		App->render->Blit(texture, localPosition.x, localPosition.y, &section, false);
		break;
	case MOUSEOVER:
		App->render->Blit(texture, localPosition.x, localPosition.y, &OnMouse, false);
		break;
	case CLICKED:
		App->render->Blit(texture, localPosition.x, localPosition.y, &OnClick, false);
		break;
	}

	if (type == CHECKBOX && tick)
	{
		App->render->Blit(texture, localPosition.x, localPosition.y, &Tick, false);
	}
	else if (type == LINK && text != nullptr)
	{		
		text->BlitElement();
	}
}
