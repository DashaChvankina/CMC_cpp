#include <iostream>

using std::cout;
using std::endl;
using std::cin;

class list;
class elem {     //
	int val; 	//значение поля данных
	elem *next;  //указатель на следующий эелемент
	elem *prev;  //указатель на предыдущий элемент

public:
	elem(int x): val(x){
		//cout<<"Constr elem"<<endl;
		next = NULL;
		prev = NULL;
	};
	friend list;
	~elem(){//cout<<"Destr elem"<<endl;
	}
};

class list {
	elem *l; //указатель на список  
public:
	list(){
		l = NULL;
	}
	list(int x){
		l = new elem(5);
	};
	void print();
	void push_front(int x);
	void push_back(int x);
	int front();
	int back();
	void pop_front();
	void pop_back();
	void insert(int y,int x);
	void erase(int x);
	int empty();
	int size();
	~list(){
		elem *p = l;
		while (l != NULL) {
			l = l->next;
			delete p;
			p = l;
		}
	}

};

void list::print(){ //вывод информации об элементах списка
	elem* p = l;
	while (p!=NULL) {
		cout<<p->val<<" ";
		p = p->next;
	}
	cout<<endl;
}

void list::push_front(int x){ //добавления элемента в начало
	elem *a;
	a = new elem(x);
	if (l != NULL) {
		a->next = l; 
		l->prev = a; 
	}
	l = a; 
}

void list::push_back(int x){ //добавления элемента в конец
	elem *a, *p = l;
	a = new elem(x);
	if (l == NULL) l = a;
	else {
		while (p->next != NULL)  p = p->next;
		a->prev = p;
		p->next = a;
	}

}

int list::front(){ //чтение первого элемента списка
	if (l == NULL) {
		cout<<"The list is empty"<<endl;
		return 0;
	}
	else return l->val; 
}

int list::back(){ //чтение последнего элемента списка
	elem *p = l;
	if (l == NULL) {
		cout<<"The list is empty"<<endl;
		return 0;
	}
	else{
		while (p->next!=NULL) p = p->next;
		return p->val;
	}
}

void list::pop_front(){ //удаление первого элемента списка
	elem *p = l;
	if (l != NULL){
		l = l->next;
		if (l != NULL) l->prev = NULL;
		delete p;
	}
}

void list::pop_back(){ //удаление последнего элемента списка
	elem *p = l;
	if (l != NULL) {
		while (p->next != NULL) p = p->next;
		if (p->prev != NULL) {
			p->prev->next = NULL;
			delete p;
		}
		else { delete p; l = NULL; }
	}
}

void list::insert(int y,int x){ //добавление элемента x перед позицией y 
	//если y превышает количество элементов в списке, 
	//то добавляем элемент в конец списка
	elem *p = l;        //указатель на текущий эемент
	elem *a;            //указатель на создаваемый эелемент
	int i = 0;          //номер текущего эелемента
	if (y > 0) {
		if (y == 1) this->push_front(x);
		else{
			for (i = 0; i != y-1; i++) {
				if (p != NULL) p = p->next; 
				else break;
			}

			if (p == NULL) this->push_back(x);
			else{ 
				a = new elem(x); 
				p->prev->next = a;
				a->next = p;
				a->prev = p->prev;
				p->prev = a;
			}
		}
	}
}

void list::erase(int x){ //удаление элемента из позиции x
	//если х превышает количество элементов в списке, 
	//то удаляем последний элемент
	if (x > 0) {
		elem *p = l;
		for (int i = 0; i < x-1; i++){
			if (p != NULL) p = p->next;
			else break;
		}
		if (x == 1) this->pop_front();
		else {
			if ((p != NULL)&&(p->next != NULL)){
				p->prev->next = p->next;
				p->next->prev = p->prev;
				delete p;
			}
			else this->pop_back();
		}
	}
	
}

int list::empty(){ //проверка списка на пустоту
	//возвращает 0, если список пуст
    //возвращает 1, если список не пустой
	if (l == NULL) return 0;
	else return 1;
}

int list::size(){ //текущее число элементов
	int count = 0;
	elem *p = l;
	while (p != NULL){
		count+=1;
		p = p->next;
	}
	return count;
}


int main(){
 
	list lst(5); lst.print();
	list lst_emp; lst_emp.print();

	cout<<endl<<"Testing push_front and push_back"<<endl;
	lst.push_front(3); lst.print(); 
	lst.push_back(7); lst.print();
	lst_emp.push_front(1); lst_emp.print(); lst_emp.pop_front();

	cout<<endl<<"Testing pop_front and pop_back"<<endl;
	lst.print();
	lst.pop_front(); lst.print(); 
	lst.pop_back(); lst.print();
	lst.pop_front(); lst.print(); 
	lst.push_front(3); lst.print(); 
	lst.pop_back(); lst.print();
	lst_emp.pop_front();
	lst_emp.pop_back();

	lst.push_front(3); lst.push_front(7); lst.push_front(5); lst.push_front(4);

	cout<<endl<<"Testing front and back"<<endl;
	lst.print();
	cout<<"the first element is "<<lst.front()<<endl;
	cout<<"The latest element is "<<lst.back()<<endl;
	cout<<"the first element is "<<lst_emp.front()<<endl;
	cout<<"The latest element is "<<lst_emp.back()<<endl;

	cout<<endl<<"Testing empty"<<endl;
	cout<<lst.empty()<<endl; 		//expected 1
	cout<<lst_emp.empty()<<endl; 	//expected 0

	cout<<endl<<"Testing insert"<<endl;
	lst.print();
	lst.insert(2, 1); lst.print();
	lst.insert(1, 0); lst.print();
	lst.insert(9, 8); lst.print();
	lst.insert(7, 2); lst.print();
	lst_emp.insert(5, 4); lst_emp.print();
	lst_emp.pop_front();

	cout<<endl<<"Testing size"<<endl;
	lst.print();
	cout<<lst.size()<<endl;
	cout<<lst_emp.size()<<endl;

	cout<<endl<<"Testing erase"<<endl;
	lst.print();
	lst.erase(2); lst.print();
	lst.erase(6); lst.print();
	lst.erase(1); lst.print();
	lst.erase(5); lst.print();
	lst.erase(10); lst.print();
	lst_emp.erase(3);



	return 0;
}
