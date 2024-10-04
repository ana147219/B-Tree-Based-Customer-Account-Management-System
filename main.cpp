#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <string>
#include <algorithm>

using namespace std;
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
typedef struct Account
{
	long long int id,B_id,C_id;
	int TAX_st;
	string name;
	double bal;
}account;
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
class node
{
public:
	long long int* keys;
	int size;
	node* parent;
	node** children;
	account** acc;
	bool is_leaf;
public:
	node(int m)
	{
		parent = nullptr;
		size = 0;
		keys = new long long int[m-1];
		children = new node*[m];
		acc = new account *[m - 1];
		for (int i = 0;i < m - 1;i++)
		{
			children[i] = nullptr;
			acc[i] = nullptr;
		}
		children[m - 1] = nullptr;
	}
	~node()
	{
		delete[] keys;
		delete[] children;
		delete[] acc;
		parent = nullptr;
	}
};
 //  - - - - -  - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void insert(node** queue, int* front, int* rear, long long int size, node* x)
{
	*rear = (*rear + 1);
	if (*front == 0) *front = 1;
	queue[*rear] = x;
}
node* get(node** queue, int* front, int* rear, long long int size)
{
	node* x = queue[*front];
	if (*front == *rear) *front = *rear = 0;
	else *front = (*front + 1);
	return x;
}
bool q_empty(node** queue, int* front, int* rear)
{
	if (*front == *rear) return true;
	else return false;
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void replicit(int child_index,long long int replicit, node* current)
{
	if (current->parent->children[child_index] == current)
		current->parent->keys[child_index] = replicit;
	else
	{
		int index = 0;
		node* help = current;
		while (help->parent != nullptr && help->parent->children[help->parent->size] == help)
			help = help->parent;
		if (help->parent != nullptr)
		{
			for (int i = 0;i < help->parent->size;i++)
			{
				if (help == help->parent->children[i])
				{
					index = i;
					break;
				}
			}
			help->parent->keys[index] = replicit;
		}
	}
	return;
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class Bplus_tree
{
public:
    node* root;
	Bplus_tree()
	{
		root = nullptr;
	}
	void erase()
	{
		root = nullptr;
	}
public:
	node* get_root()
	{
		return root;
	}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	void insert(int m,long long int key, account* ac)
	{
		if (root == nullptr)
		{
			root = new node(m);
			root->parent = nullptr;
			root->acc[0] = ac;
			root->keys[0] = key;
			root->size++;
			root->is_leaf = true;
		}
		else
		{
			node* current = root;
			node* parent=nullptr;
			while (current->is_leaf == false)
			{
				parent = current;
				for (int i = 0;i < current->size;i++)
				{
					if (key < current->keys[i])
					{
						current = current->children[i];
						break;
					}
					if (i == current->size - 1)
					{
						current = current->children[i + 1];
						break;
					}
				}
			}
			if (current->size < m - 1)
			{
				int pom = 0;
				while (key > current->keys[pom] && pom < current->size) pom++;
				for (int i = current->size;i > pom;i--) 
				{
					current->keys[i] = current->keys[i - 1];
					current->acc[i] = current->acc[i - 1];
				}
				current->keys[pom] = key;
				current->acc[pom] = ac;
				current->size++;
			}
			else
			{
				long long int* mask = new long long int[m];
				account** mask_acc = new account* [m];
				for (int i = 0;i < current->size;i++)
				{
					mask[i] = current->keys[i];
					mask_acc[i] = current->acc[i];
				}
				int pom = 0;
				while (key > mask[pom] && pom < current->size) pom++;
				for (int i = current->size;i > pom;i--)
				{
					mask[i] = mask[i - 1];
					mask_acc[i] = mask_acc[i - 1];
				}
				mask[pom] = key;
				mask_acc[pom] = ac;
				node* leaf = new node(m);
				if ((m % 2) == 0) current->size = m / 2;
				else current->size = m / 2 + 1;
				leaf->size = m - current->size;
				for (int i = 0;i < current->size;i++)
				{
					current->keys[i] = mask[i];
					current->acc[i] = mask_acc[i];
				}
				for (int i =0,j=current->size;j < m;i++,j++)
				{
					leaf->keys[i] = mask[j];
					leaf->acc[i] = mask_acc[j];
				}
				leaf->parent = current->parent;
				leaf->is_leaf = true;
				leaf->children[0] = current->children[0];
				current->children[0] = leaf;
				long long int fantom_key = current->keys[current->size - 1];
				node*child = leaf;
				delete[] mask;
				delete[] mask_acc;
				if (current == root)
				{
					node* newroot = new node(m);
					newroot->parent = nullptr;
					newroot->keys[0] = fantom_key;
					newroot->children[0] = current;
					newroot->children[1] = leaf;
					newroot->size = 1;
					newroot->is_leaf = false;
					current->parent = newroot;
					leaf->parent = newroot;
					root = newroot;
				}
				else
				{
					while (true)
					{
						if (parent->size < m - 1)
						{
							int pom = 0;
							while (fantom_key > parent->keys[pom] && pom < parent->size) pom++;
							for (int i = parent->size;i > pom;i--)
							{
								parent->keys[i] = parent->keys[i - 1];
								parent->children[i+1] = parent->children[i];
							}
							parent->keys[pom] = fantom_key;
							parent->children[pom + 1] = child;
							parent->size++;
							break;
						}
						else
						{
							long long int* mask = new long long int[m];
							node** mask_children = new node * [m + 1];
							for (int i = 0;i < m - 1;i++)
							{
								mask[i] = parent->keys[i];
								mask_children[i] = parent->children[i];
							}
							mask_children[m - 1] = parent->children[m - 1];
							int pom = 0;
							while (fantom_key > mask[pom] && pom < m-1) pom++;
							for (int i = m - 1;i > pom; i--)
							{
								mask[i] = mask[i - 1];
								mask_children[i + 1] = mask_children[i];
							}
							mask[pom] = fantom_key;
							mask_children[pom + 1] = child;
							fantom_key = mask[(m-1) / 2];
							node* intern = new node(m);
							intern->is_leaf = false;
							parent->size =( m-1) / 2;
							intern->size = (m - 1) - (m - 1) / 2;
							int i,j;
							for (int i = 0;i < parent->size;i++)
							{
								parent->keys[i] = mask[i];
								parent->children[i] = mask_children[i];
								mask_children[i]->parent = parent;
							}
							parent->children[parent->size] = mask_children[parent->size];
							mask_children[parent->size]->parent = parent;
							for (i = 0,j = (m - 1) / 2 + 1;j <m; i++, j++)
							{
								intern->keys[i] = mask[j];
								intern->children[i] = mask_children[j];
								mask_children[j]->parent = intern;
							}
							intern->children[intern->size] =mask_children[j];
							intern->children[intern->size]->parent = intern;
							intern->parent = parent->parent;
							delete [] mask;
							delete[] mask_children;
							if (parent == root)
							{
								node* newroot = new node(m);
								newroot->is_leaf = false;
								newroot->parent = nullptr;
								newroot->keys[0] = fantom_key;
								newroot->children[0] = parent;
								newroot->children[1] = intern;
								newroot->size = 1;
								parent->parent = newroot;
								intern->parent = newroot;
								root = newroot;
								break;
							}
							parent = parent->parent;
							child = intern;
						}
					}
				}
			}
		}
	}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	void remove_intern(int m,int index, node* parent_node)
	{
		node* current = parent_node; //brisemo razdvojni kljuc iz parenta
		node* parent = current->parent;
		while (true)
		{
			for (int i = index;i < current->size-1;i++)
			{
				current->keys[i] = current->keys[i + 1];
				current->children[i + 1] = current->children[i + 2];
			}
			current->size--;
			if (current == root) return;
			int parity = (m % 2);
			int filled = parity * (m / 2) + abs(~(parity) * (m / 2 - 1));
			if (current->size >= filled) return;
			int current_index = 0;
			for (int i = 0;i < parent->size+1;i++)
			{
				if (parent->children[i] == current) current_index = i;
			}
			int left = current_index - 1;
			int right = current_index + 1;
			if (right <= parent->size && parent->children[right]->size > filled)
			{
				current->keys[current->size] = parent->keys[current_index];
				current->size++;
				current->children[current->size] = parent->children[right]->children[0];
				parent->keys[current_index] = parent->children[right]->keys[0];
				node* right_sibling = parent->children[right];
				for (int i = 0;i < right_sibling->size;i++)
				{
					right_sibling->keys[i] = right_sibling->keys[i + 1];
					right_sibling->children[i] = right_sibling->children[i + 1];
				}
				right_sibling->size--;
				current->children[current->size]->parent = current;
				return;
			}
			else if (left >= 0 && parent->children[left]->size > filled)
			{
				node* left_sibling = parent->children[left];
				for (int i = current->size;i > 0;i--)
				{
					current->keys[i] = current->keys[i - 1];
					current->children[i + 1] = current->children[i];
				}
				current->children[1] = current->children[0];
				current->keys[0] = parent->keys[current_index];
				current->children[0] = left_sibling->children[left_sibling->size];
				current->size++;
				parent->keys[current_index] = left_sibling->keys[left_sibling->size - 1];
				current->children[0]->parent = current;
				left_sibling->size--;
				return;
			}
			else if (right <= parent->size)
			{
				node* right_sibling = parent->children[right];
				current->keys[current->size] = parent->keys[current_index];
				current->size++;
				for (int i = current->size, j = 0;j < right_sibling->size;i++, j++)
				{
					current->keys[i] = right_sibling->keys[j];
					current->children[i] = right_sibling->children[j];
				}
				current->size = current->size + right_sibling->size;
				current->children[current->size] = right_sibling->children[right_sibling->size];
				if (parent == root && parent->size==1)
				{
					node* tmp = root;
					root = current;
					delete tmp;
					delete right_sibling;
					return;
				}
				index = current_index;
				current = parent;
				parent = current->parent;
			}
			else if (left >= 0)
			{
				node* left_sibling = parent->children[left];
				left_sibling->keys[left_sibling->size] = parent->keys[current_index-1];
				left_sibling->size++;
				for (int i = left_sibling->size, j = 0;j < current->size;i++, j++)
				{
					left_sibling->keys[i] = current->keys[j];
					left_sibling->children[i] = current->children[j];
				}
				left_sibling->size = left_sibling->size + current->size;
				left_sibling->children[left_sibling->size] = current->children[current->size];
				if (parent == root && parent->size == 1)
				{
					node* tmp = root;
					root = left_sibling;
					delete tmp;
					delete current;
					return;
				}
				if (current_index == parent->size-1) index = current_index;
				else index = current_index - 1;
				current = parent;
				parent = current->parent;
			}
		}
	}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	void remove(int m,long long int key)
	{
		node* current = root;
		int child_index = 0;
		int left=-1, right=-1;
		while (current->is_leaf == false)
		{
			for (int i = 0;i < current->size;i++)
			{
				if (key <= current->keys[i])
				{
					left = i - 1;
					right = i + 1;
					child_index = i;
					current = current->children[i];
					break;
				}
				if (i == current->size - 1)
				{
					left = i;
					right = i + 2;
					child_index = i; //indeks razdvojnog kljuca u roditelju
					current = current->children[i + 1];
					break;
				}
			}
		}
		int pom;
		bool flag = false;
		for (int i = 0;i < current->size;i++)
		{
			if (key == current->keys[i])
			{
				pom = i;
				flag = true;
			}
		}
		if (!flag)
		{
			cout << "Zapis ne postoji u stablu";
			return;
		}
		for (int i = pom;i < current->size;i++) 
		{
			current->keys[i] = current->keys[i + 1];
			current->acc[i] = current->acc[i + 1];
		}
		current->size--;
		if (current == root)
		{
			if (current->size == 0)
			{
				cout << "Uklonjen je koren stabla";
				delete current;
			}
			return;
		}
		if (pom==current->size) replicit(child_index, current->keys[current->size - 1], current);
		if (current->size >= (m / 2)) return;
		node* parent = current->parent;
		if (right<=parent->size && parent->children[right]->size > m/2)
		{
			current->keys[current->size] = current->children[0]->keys[0];
			current->acc[current->size] = current->children[0]->acc[0];
			parent->keys[child_index] = parent->children[right]->keys[0];
			current->size++;
			for (int i = 0;i < parent->children[right]->size;i++)
			{
				parent->children[right]->keys[i] = parent->children[right]->keys[i + 1];
				parent->children[right]->acc[i] = parent->children[right]->acc[i + 1];
			}
			parent->children[right]->size--;
			parent->keys[child_index] = current->keys[current->size - 1];
		}
		else if (left >= 0 && parent->children[left]->size >m/2)
		{
			for (int i = current->size;i > 0;i--)
			{
				current->keys[i] = current->keys[i - 1];
				current->acc[i] = current->acc[i - 1];
			}
			current->keys[0] = parent->children[left]->keys[parent->children[left]->size - 1];
			current->acc[0] = parent->children[left]->acc[parent->children[left]->size-1];
			parent->keys[child_index] = parent->children[left]->keys[parent->children[left]->size - 1];
			current->size++;
			parent->children[left]->size--;
			parent->keys[left] = parent->children[left]->keys[parent->children[left]->size - 1];
		}
		else if (right <= parent->size)
		{
			node* right_sibling = parent->children[right];
			for (int i = current->size,j=0;j < right_sibling->size;i++,j++)
			{
				current->keys[i] = right_sibling->keys[j];
				current->acc[i] = right_sibling->acc[j];
			}
			current->size = current->size + right_sibling->size;
			current->children[0] = right_sibling->children[0];
			delete right_sibling;
			remove_intern(m,child_index, parent);	
		}
		else if (left >= 0)
		{
			node* left_sibling = parent->children[left];
			for (int i = left_sibling->size, j = 0;j < current->size;i++, j++)
			{
				left_sibling->keys[i] = current->keys[j];
				left_sibling->acc[i] = current->acc[j];
			}
			left_sibling->size = left_sibling->size + current->size;
			left_sibling->children[0] = current->children[0];
			if (child_index != parent->size - 1) child_index--;
			remove_intern(m, child_index, parent);
			delete current;
		}
			
	}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	account* search(int m, long long int key,int option,int* n)
	{
		int steps=1;
		node* current =this->get_root();
		while (current->is_leaf == false)
		{
			for (int i = 0;i < current->size;i++)
			{
				if (key <= current->keys[i])
				{
					steps++;
					current = current->children[i];
					break;
				}
				if (i == current->size - 1)
				{
					steps++;
					current = current->children[i + 1];
					break;
				}
			}
		}
		for (int i = 0;i < current->size;i++)
		{
			if (current->keys[i] == key)
			{
				if (option == 4)
				{
					cout << current->acc[i]->id << endl;
					cout << current->acc[i]->B_id << endl;
					cout << current->acc[i]->C_id << endl;
					cout << current->acc[i]->name << endl;
					cout << current->acc[i]->TAX_st << endl;
					cout << current->acc[i]->bal << endl;
					cout << "Ukupan broj koraka uspesne pretrage: " << steps << endl;
				}
				*n = steps;
				return current->acc[i];
			}
		}
		if(option==4) 
			cout << "Zapis ne postoji u tabeli" << endl;
		return nullptr;
	}
};
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void print(int m,node*root)
{
	long long int size = 100000000;
	bool flag=true;
	node** queue = new node * [size];
	int front = 0, rear = 0;
	node* next = root;
	insert(queue, &front, &rear, size, next);
	insert(queue, &front, &rear, size, nullptr);
	while (queue[front] != nullptr)
	{
		flag = true;
		next = get(queue, &front, &rear, size);
		cout << "|";
		for (int i = 0;i < next->size;i++)
		{
			/*
			if (flag && next->parent!=nullptr)
			{
				cout<<"parent " << next->parent->keys[0]<<": | ";
				flag = false;
			}
			*/
			cout << next->keys[i] << "|";
		}
		if (next->is_leaf == false)
		{
			for (int i = 0;i < next->size + 1;i++) insert(queue, &front, &rear, size, next->children[i]);
		}
		if (queue[front] == nullptr)
		{
			cout << endl;
			next = get(queue, &front, &rear, size);
			insert(queue, &front, &rear, size, next);
		}
		else cout << "->";
	}
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
account* split(string line, char del,account*current)
{
	stringstream ss(line);
	string word;
	int len = 0;
	while (!ss.eof())
	{
		getline(ss, word, del);
		switch (len)
		{
		case 0:
			current->id = stoull(word, nullptr, 10);
			break;
		case 1:
			current->B_id = stoull(word, nullptr, 10);
			break;
		case 2:
			current->C_id = stoull(word, nullptr, 10);
			break;
		case 3:
			current->name = word;
			break;
		case 4:
			current->TAX_st = stoi(word);
			break;
		case 5:
			current->bal = stod(word, nullptr);
			break;
		}
		len++;
	}
	return current;
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void add(int m,Bplus_tree element)
{
	account* acc = new account;
	cout << "Unesite primarni kljuc: ";
	cin >>acc->id;
	cout << "Unesite B_id: ";
	cin >> acc->B_id;
	cout << "Unesite C_id: ";
	cin >>acc-> C_id;
	cout << "Unesite ime vlasnika racuna: ";
	cin >> acc->name;
	cout << "Unesite TAX_st kljuc: ";
	cin >> acc->TAX_st;
	cout << "Unesite stanje na racunu: ";
	cin >> acc->bal;
	element.insert(m, acc->id, acc);
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
account** search_k(int m,Bplus_tree element,int option,int n)
{
	long long int x;
	bool flag=false;
	long long int* array = new long long int[n];
	for (int i = 0;i <n;i++)
	{
		cout << "Unesite element niza: ";
		cin >> x;
		for (int j = 0;j < i;j++)
		{
			if (x == array[j])
			{
				i--;
				flag = true;
			}
		}
		if (flag)
		{
			cout << "Ponovljena vrednost kljuca;ponovo unesite element" << endl;
			flag = false;
		}
		else
			array[i] = x;
	}
	sort(array, array + n);
	account** accounts = new account * [n];
	int len = 0,st=0;
	for (int i = 0;i < n;i++)
	{
		accounts[len++]= element.search(m, array[i], option,&st);
	}
	cout << "Ukupan broj koraka zavrsene pretrage: " << (st * n);
	return accounts;
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void write_file(int len,account**accounts,string file_name)
{
	fstream my_file;
	my_file.open(file_name, ios::out);
	if (!my_file) cout << "Nepostojeci fajl";
	else
	{
		for (int i = 0;i < len;i++)
		{
			if (accounts[i] != nullptr) my_file << accounts[i]->id << '|' << accounts[i]->B_id << '|' << accounts[i]->C_id << '|' << accounts[i]->name << '|' << accounts[i]->TAX_st << '|' << accounts[i]->bal << endl;
			else my_file << "Ne postoji zapis" << endl;
		}
		my_file.close();
	}
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
account**search_succ(int start, int len, node* root)
{
	account** accounts = new account * [len];
	int steps = 1;
	node* current = root;
	while (current->is_leaf == false)
	{
		for (int i = 0;i < current->size;i++)
		{
			if (start <= current->keys[i])
			{
				steps++;
				current = current->children[i];
				break;
			}
			if (i == current->size - 1)
			{
				steps++;
				current = current->children[i + 1];
				break;
			}
		}
	}
	int i = 0;
	while(i<current->size)
	{
		if (current->keys[i] == start)
		{
			int j = 0;
			while (current != nullptr && j < len)
			{
				accounts[j] = current->acc[i];
				if (i == current->size - 1)
				{
					current = current->children[0];
					i = 0;
					if (j != len - 1) steps++;
				}
				else
				{
					i++;
				}
				j++;
			}
			break;
		}
		else i++;
	}
	cout << "Ukupan broj koraka: " << steps;
	return accounts;
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void tree_die(int m, Bplus_tree root)
{
	bool flag = true;
	long long int size = 100000000;
	node** queue = new node * [size];
	int front = 0, rear = 0;
	node* next = root.get_root();
	insert(queue, &front, &rear, size, next);
	while (queue[front] != nullptr)
	{
		next = get(queue, &front, &rear, size);
		if (next->is_leaf == false)
		{
			for (int i = 0;i < next->size + 1;i++) insert(queue, &front, &rear, size, next->children[i]);
		}
		else if (next->is_leaf == true && flag)
		{
			insert(queue, &front, &rear, size, nullptr);
			flag = false;
		}
		delete next;
	}
	cout << "Stablo je obrisano";
}
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void main()
{
	int m;
	cout << "Unesite red stabla: ";
	cin >> m;
	account* accounts = new account[100000];
	int len = 0;
	ifstream file;
	file.open("CustomerAccount22.txt");
	string line;
	if (file.is_open())
	{
		while (getline(file,line) && !line.empty())
		{
			account* current = new account;
			current= split(line, '|',current);
			accounts[len++] = *current;
		}
	}
	else cout << "Datoteka ne postoji";
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	Bplus_tree* element=new Bplus_tree;
	for (int i = 0;i < len;i++) element->insert(m, accounts[i].id, &accounts[i]);
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 	
	cout << "1. Ispis indexa" << endl;
	cout << "2. Dodavanje novog zapisa" << endl;
	cout << "3. Brisanje zapisa" << endl;
	cout << "4. Pretrazivanje jednog podatka" << endl;
	cout << "5. Pretrazivanje k podataka" << endl;
	cout << "6. Pretrazivanje k sukcesivnih podataka" << endl;
	cout << "7. Brisanje indexa" << endl;
	cout << "8. Prekid programa" << endl;
	int option;
	while (true)
	{
		cout << endl << "Odaberite jednu od sledecih opcija:" << endl;
		cin >> option;
		switch (option)
		{
		case 1:
		{
			if (element->get_root() == nullptr)
			{
				cout << "Stablo ne postoji";
				break;
			}
			print(m, element->get_root());
			break;
		}
		case 2:
		{
			add(m, *element);
			break;
		}
		case 3:
		{
			long long int key;
			cout << "Unesite primarni kljuc: ";
			cin >> key;
			element->remove(m, key);
			break;
		}
		case 4:
		{
			if (element->get_root() == nullptr)
			{
				cout << "Stablo ne postoji";
				break;
			}
			long long int key;
			int m;
			cout << "Unesite primarni kljuc: ";
			cin >> key;
			account* current = element->search(m, key, option,&m);
			break;
		}
		case 5:
		{
			if (element->get_root() == nullptr)
			{
				cout << "Stablo ne postoji";
				break;
			}
			int k;
			cout<< "Unesite broj primarnih kljuceva: ";
			cin >> k;
			account** accounts = search_k(m, *element, option,k);
			write_file(k, accounts, "Ana_account.txt");
			break;
		}
		case 6:
		{
			if (element->get_root() == nullptr)
			{
				cout << "Stablo ne postoji";
				break;
			}
			int k,start;
			cout << "Unesite broj primarnih kljuceva: ";
			cin >> k;
			cout << endl << "Unesite pocetni kljuc: ";
			cin >> start;
			account** accounts = search_succ(start, k, element->get_root());
			write_file(k, accounts, "Ana_7.txt");
			break;
		}
		case 7:
		{
			tree_die(m,*element);
			element->erase();
			break;
		}
		case 8:
			exit(0);
		}
	}
}
