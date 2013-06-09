/*
 * The Arduino Header ArrayList
 * Written: Obed Isai Rios
 * Override: Guilherme D'Amoreira
 */
 
#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include "Arduino.h"
#include "Component.h"
 
class ComponentList {
	public:	
		ComponentList();
		void display_list();
		void add(Component* item);
		void set(Component* item, int index);
		void remove(int index);
		Component* get(int index);
		void empty_list();
		Component** get_list();
		int size();
	private:
		Component** list;
		int length;	
};
 
#endif