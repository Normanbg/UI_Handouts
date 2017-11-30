#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_InputBox.h"
#include "UI_Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		for (p2List_item<Button*>* item = buttons.start; item; item = item->next) //Buttons
		{
			if (x > item->data->localPosition.x && x < item->data->localPosition.x + item->data->standby.w && y > item->data->localPosition.y && y < item->data->localPosition.y + item->data->standby.h)
			{
				item->data->clicked = true;
				if (item->data->type == CHECKBOX)
				{
					item->data->tick = !item->data->tick;
				}
				break;
			}
		}
		for (p2List_item<inputBox*>* item = inputTexts.start; item; item = item->next) //Input Text
		{
			if (x > item->data->localPosition.x && x < item->data->localPosition.x + item->data->box.w && y > item->data->localPosition.y && y < item->data->localPosition.y + item->data->box.h)
			{
				item->data->reading = true;
				SDL_StartTextInput();
			}
		}
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		for (p2List_item<Button*>* item = buttons.start; item; item = item->next)
		{
			if (item->data->clicked)
			{
				item->data->clicked = false;
				break;
			}
		}
	}*/

	return true;
}

bool j1Gui::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next)
	{
		if (x > item->data->localPosition.x && x < item->data->localPosition.x + item->data->section.w && y > item->data->localPosition.y && y < item->data->localPosition.y + item->data->section.h)
		{
			if (!item->data->hovering)
			{
				item->data->hovering = true;
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_ENTER);
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_LEFT_CLICK);
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_LEFT_RELEASE);
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
			{
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_RIGHT_CLICK);
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
			{
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_RIGHT_RELEASE);
			}
		}
		else
		{
			if (item->data->hovering)
			{
				item->data->hovering = false;
				if (item->data->callback != nullptr)
					item->data->callback->OnUIEvent(item->data, MOUSE_LEAVE);
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{	
	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next)
	{
		item->data->BlitElement();
	}

	if (UI_Debug)
		DebugDraw();
	/*for (p2List_item<inputBox*>* item = inputTexts.start; item; item = item->next) //Input Text reading
	{
		if (item->data->reading)
		{
			item->data->readInput();
			break;
		}
	}*/

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_element*>* item;
	item = UI_elements.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	UI_elements.clear();

	return true;
}

void j1Gui::DebugDraw()
{
	for (p2List_item<UI_element*>* item = UI_elements.start; item; item = item->next)
	{
		SDL_Rect box;
		box.x = item->data->calculateAbsolutePosition().x;
		box.y = item->data->calculateAbsolutePosition().y;
		box.w = item->data->section.w;
		box.h = item->data->section.h;
		App->render->DrawQuad(box, 255, 0, 0, 255, false);
	}

}
// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

Text* j1Gui::createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback, bool addToList)
{
	Text* ret = new Text(text, x, y, font, color, callback);
	if (addToList)
		UI_elements.add(ret);
	
	return ret;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture, j1Module* callback, bool addToList)
{
	Image* ret = new Image(texture, x, y, {0, 0, 0, 0}, callback);
	if (addToList)
		UI_elements.add(ret);
	
	return ret;
}

Image* j1Gui::createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback, bool addToList)
{
	Image* ret = new Image(atlas, x, y, section, callback);
	if (addToList)
		UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createCheckBox(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnClick, SDL_Rect Tick, j1Module* callback, bool addToList)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnClick, Tick, CHECKBOX, callback);
	if (addToList)
		UI_elements.add(ret);

	return ret;
}

InputBox* j1Gui::createInputBox(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module* callback, bool addToList)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	InputBox* ret = new InputBox(font, color, x, y, usingTexture, section, callback);
	if (addToList)
		UI_elements.add(ret);

	return ret;
}

Window* j1Gui::createWindow(int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module * callback, bool addToList)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Window* ret = new Window(usingTexture, x, y, section, callback);
	if (addToList)
		UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback, bool addToList)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;
	
	Button* ret = new Button(x, y, usingTexture, standby, OnMouse, OnClick, LINK, callback);
	if (addToList)
		UI_elements.add(ret);
	
	return ret;
}

// class Gui ---------------------------------------------------
