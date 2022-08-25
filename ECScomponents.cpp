#ifndef _ECSCOMPONENTS_
#define _ECSCOMPONENTS_
#include "ECS.cpp"

class PositionComponent:public component {
private:
	int x=0, y=0;
public:
	PositionComponent() {}
	PositionComponent(int ix, int iy) {
		x = ix;
		y = iy;
	}
	void init()override
	{
		x = 0;
		y = 0;
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
#endif