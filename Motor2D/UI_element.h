#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "SDL\include\SDL_mouse.h"

struct SDL_Texture;

enum element_type
{
	TEXT,
	IMAGE,
	BUTTON,
	//SLIDER,
	INPUTTEXT,
	WINDOW
};

enum element_state
{
	STANDBY,
	MOUSEOVER,
	CLICKED
};

class UI_element
{
public:

	UI_element()
	{}

	UI_element(int x, int y, element_type type, SDL_Rect section, j1Module* callback, SDL_Texture* texture = nullptr): localPosition({x, y}), element_type(type), section(section), callback(callback), texture(texture)
	{}

	virtual ~UI_element()
	{
		if (texture != nullptr && texture != App->gui->GetAtlas())
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

	virtual void setOutlined(bool isOutlined)
	{}

	iPoint calculateAbsolutePosition() const
	{
		if (parent != nullptr)
			return (localPosition + parent->calculateAbsolutePosition());
		else
			return localPosition;
	}

	virtual void appendChild(int x, int y, UI_element* child)
	{}

	virtual void appendChildAtCenter(UI_element* child)
	{}

	virtual void BlitElement()
	{}


	void Mouse_Drag()
	{
		iPoint MousePos;
		//iPoint globalPos = calculateAbsolutePosition();
		App->input->GetMousePosition(MousePos.x, MousePos.y);
		this->localPosition.x = (MousePos.x - Click_Pos.x);
		this->localPosition.y = (MousePos.y - Click_Pos.y);
		Click_Pos = MousePos;
	}
public:

	SDL_Texture* texture = nullptr;
	iPoint localPosition;
	SDL_Rect section;
	element_type element_type;
	element_state state = STANDBY;
	j1Module* callback = nullptr;
	UI_element* parent = nullptr;
	bool hovering = false;
	
protected:
	iPoint Click_Pos{ 0,0 };
};

#endif // !__UI_ELEMENT__
