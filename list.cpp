#include "list.h"

list::list()
{
	head = nullptr;
	tail = nullptr;
	count = 0;
}

bool list::isIDExist(char * str)
{
	int i;
	node * temp = tail;
	while (temp != nullptr) {
		for (i=0; i<MAX_ID_LENGHT; i++)
			if (temp->id[i] != str[i]) break;
		if (i == MAX_ID_LENGHT) {
			return true;
		} else {
			temp = temp->next;
		}
	}
	return false;
}

int list::insert(char * str, int fd)
{
	if (isIDExist(str))
		return -1;
	
	node * temp = new node;
	temp->con = fd;
	for (int i=0; i<MAX_ID_LENGHT; i++)
		temp->id[i] = str[i];
	if (count == 0) {
		temp->next = nullptr;
		temp->back = nullptr;
		head = temp;
		tail = temp;
	} else {
		temp->next = tail;
		temp->back = nullptr;
		tail->back = temp;
		tail = temp;
	}
	count ++;
	return 0;
}

int list::remove(char * str)
{
	int i;
	node * temp = tail;
	while (temp != nullptr) {
		for (i=0; i<MAX_ID_LENGHT; i++)
			if (temp->id[i] != str[i]) break;
		if (i == MAX_ID_LENGHT) {
			if (temp->back != nullptr) temp->back->next = temp->next;
			if (temp->next != nullptr) temp->next->back = temp->back;
			delete temp;
			count --;
			break;
		} else {
			temp = temp->next;
		}
	}
	return 0;
}

int list::getConnection(char * str)
{
	int i;
	node * temp = tail;
	while (temp != nullptr) {
		for (i=0; i<MAX_ID_LENGHT; i++)
			if (temp->id[i] != str[i]) break;
		if (i == MAX_ID_LENGHT) {
			return temp->con;
		} else {
			temp = temp->next;
		}
	}
	return -1;
}

char * list::getIDs()
{
	int size = MAX_ID_LENGHT * count + count;
	char * buff = new char[size];
	node * temp = tail;
	int i = 0;
	while (temp != nullptr) {
		for (int j = 0; j < MAX_ID_LENGHT; j++, i++)
			buff[i] = temp->id[j];
		buff[i] = '\n';
		i++;
		temp = temp->next;
	}
	return buff;
}
			

int list::getCount()
{
	return count;
}
