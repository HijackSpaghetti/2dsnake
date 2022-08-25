#ifndef _ECSCOMPONENTS_
#define _ECSCOMPONENTS_
#include <windows.h>
#include "ECS.cpp"
#include "animation.cpp"
#include "input.cpp"


class PositionComponent:public component {
private:
	
public:
	int x = 0, y = 0;
	PositionComponent() {}
	PositionComponent(int ix, int iy) {
		x = ix;
		y = iy;
	}
	void init()override
	{

	}
	void set(int ix,int iy) {
		y = ix;
		x = iy;
	}

	void update()override
	{
		return;
	}
	void draw()override
	{
		return;
	}
	~PositionComponent() {}

};

class MovementComponent :public component {
private:
	int dx, dy;
public:
	void init()override
	{
		dx = 80;
		dy = 90;
	}
	void update()override
	{
		myEntity->getComponent<PositionComponent>().set(dx, dy);
		return;
	}
	void draw()override
	{
		return;
	}


};
class MouseInteractionComponent :public component {



};
class InputComponent :public component {
private:
	input* in;
public:
	InputComponent(input& inp) {
		in = &inp;
	}


	void update()override
	{
		if (in->isKeyDown(input::key::UP))
		{
			myEntity->getComponent<PositionComponent>().y--;
		}
		if (in->isKeyDown(input::key::DOWN))
		{
			myEntity->getComponent<PositionComponent>().y++;
		}
		if (in->isKeyDown(input::key::LEFT))
		{
			myEntity->getComponent<PositionComponent>().x--;
		}
		if (in->isKeyDown(input::key::RIGHT))
		{
			myEntity->getComponent<PositionComponent>().x++;
		}
	}
	
};

class AnimationComponent :public component {//entity must have sprite preloaded and have position component for this to work
private:
	animation* thisAnimation;
public:
	AnimationComponent(Renderer& render) {
		thisAnimation = new animation(render);
	}
	void setupFromFile(const char* filename, std::map<std::string, sprite*> collection, const char* spritename) {
		thisAnimation->getAnimationsListFromFile(filename, collection, spritename);
	}

	void init()override
	{
	}
	void nextAnim(std::string str) {
		thisAnimation->animate(str);
	}
	void nextAnimOnce(std::string str) {
		thisAnimation->animateOnce(str);
	}
	void resize(double i) {//nearest neighbour resizing
		thisAnimation->asize(i);
	}
	void rotate(double i) {//angle in radians
		thisAnimation->arotate(i);
	}
	void addhue(unsigned int i) {//ARGB format, 0xFF00FF00 - would color fully pure green
		thisAnimation->setHue(i);
	}
	void transparency(byte i) {//255-full transparency, 0-no changes
		thisAnimation->setTransparency(i);
	}
	void update()override
	{
		thisAnimation->position(myEntity->getComponent<PositionComponent>().x, myEntity->getComponent<PositionComponent>().y);
		int r = 0;
	}
	void draw()override
	{
		thisAnimation->animate(GetTickCount64());
	}
};
#endif