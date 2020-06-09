#define MAX_ID_LENGHT 6

struct node
{
	char id[MAX_ID_LENGHT];
	int con;
	node * next;
	node * back;
};

class list
{
		node * head;
		node * tail;
		unsigned int count;
		bool isIDExist(char * str);
	public:
		list();
		int insert(char * str, int fd);
		int remove(char * str);
		int getConnection(char * str);
		char * getIDs();
		int getCount();
};
