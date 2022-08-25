#ifndef _ECS_
#define _ECS_
#include <windows.h>
#include <map>
#include <vector>
#include <bitset>
#include <memory>
#include <array>
//entities are IDs, components are data and methods, manager is system. manager knows all entities, entities know all their components, components know entities they belong to
constexpr std::size_t  maxComponents = 32;
class component;
class ECS;
class entity;
inline std::size_t getNewCTypeID() {//generating new component type ID
	static std::size_t ID=0;
	return ID++;
}
template <typename T> inline std::size_t getCompTID() {//returning component type ID, based on class name
	static std::size_t type = getNewCTypeID();
	return type;
}

class component {
private:
	bool active = { true };
public:
	entity* myEntity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~component() {}
	bool isActive() {
		return active;
	}
	void del() {
		active = false;
	}
};

class entity {
private:
	bool active = { true };
	std::vector < std::unique_ptr<component>> components;
	std::bitset<maxComponents> componentsBitSet;
	std::array<component*, maxComponents> componentsArray;//array of pointers to components
public:
	void update() {
		for (auto& i: components) i->update();
	}
	void draw() {
		for (auto& i: components) i->draw();
	}
	bool isActive() {
		return active;
	}
	void destroy() {
		active = false;
	}
	template <typename T> bool hasComponent() {
		return componentsBitSet[getCompTID<T>()];
	}


	template <typename T,typename... TArgs> T& addComponent(TArgs&&... params) {//TArgs - use this to pass multiple various parameters or nothing at all to specific component constructor
		T* thiscomponent(new T(std::forward<TArgs>(params)...));
		thiscomponent->myEntity = this;
		std::unique_ptr<component> uPtr{ thiscomponent };
		components.push_back(std::move(uPtr));//unique pointers can only be moves, not copied
		componentsArray[getCompTID<T>()] = thiscomponent;
		componentsBitSet[getCompTID<T>()] = true;

		thiscomponent->init();
		return *thiscomponent;
	}
	template <typename T> T& getComponent() {
		auto p(componentsArray[getCompTID<T>()]);
		return *static_cast <T*>(p);
	}
	void refresh() {
		int d = 0;
		components.erase(std::remove_if(std::begin(components), std::end(components), [](std::unique_ptr<component>& thisComponent) {
			return !thisComponent->isActive(); }), std::end(components));
		int r = 0;
	}
	template <typename T> void deleteComponent() {
		component* c = componentsArray[getCompTID<T>()];
		c->del();
		componentsArray[getCompTID<T>()] = NULL;
		componentsBitSet[getCompTID<T>()] = false;
		refresh();

		
		
	}
};




class ECS {
private:
	std::vector<std::unique_ptr<entity>> entities;
public:



	void update() {
		for (auto& e : entities) e->update();
	}
	void draw() {
		for (auto& e : entities) e->draw();
	}
	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](std::unique_ptr<entity>& thisEntity) {
			return !thisEntity->isActive(); }), std::end(entities));
	}
	entity& addEntity()
	{
		entity* e = new entity();
		std::unique_ptr<entity> ePtr{e};
		entities.push_back(std::move(ePtr));
		return *e;
	}
};
#endif