/*
 * Arduino Class for ComponentList
 * Written: Obed Isai Rios
 */
 
 
#include "Arduino.h"
#include "ComponentList.h"
#include "Component.h"

ComponentList::ComponentList() {
	this->length = 0; 
}

void ComponentList::add(Component* item) {
	Component **newlist = (Component**)malloc((this->length+1)*sizeof(Component*));
	for (int i=0; i < this->length; i++) {
		newlist[i] = this->list[i];
	}

	newlist[this->length] = item;
	this->list = newlist;
	this->length++;
}

void ComponentList::set(Component* item, int index) {
	this->list[index] = item;
}

void ComponentList::remove(int index) {
	Component **newlist = (Component**)malloc((this->length-1)*sizeof(Component*));
	for (int i = 0; i < index; i++) {
		newlist[i] = this->list[i]; 
	}
	for (int i = index; i <= this->length-1; i++) {
		newlist[i] = this->list[i+1];
	}
	this->list = newlist;
	this->length--;
}

Component* ComponentList::get(int index) {
	return this->list[index];
}

void ComponentList::empty_list() {
	this->length = 1;
	Component **newlist = (Component**)malloc((this->length)*sizeof(Component*));   
	this->list = newlist;
	this->list[0] = NULL;
}

void ComponentList::display_list() {
	for (int i = 0; i < this->length; i++) {
		Serial.print("Component[index=");
		Serial.print(i);
		Serial.print(", name=");
		Serial.print(this->get(i)->name);
		Serial.println("]");
	}
}

Component** ComponentList::get_list() {
	return this->list;
}

int ComponentList::size() {
	return this->length;
}