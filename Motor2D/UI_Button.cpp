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
	else if (type == LINK)
	{
		text->localPosition.x = localPosition.x + section.w / 2 - text->tex_width / 2;
		text->localPosition.y = localPosition.y + section.h / 2 - text->tex_height / 2;
		
		text->BlitElement();
	}
}
