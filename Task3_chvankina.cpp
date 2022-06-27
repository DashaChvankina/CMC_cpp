#include <iostream>
#include <cstring>

using std::cout;
using std::endl;
using std::cin;

class list;

class elem {     //
	char *lastname; //фамилия клиента
	int time; 	//значение поля данных
	elem *next;  //указатель на следующий эелемент
	elem *prev;  //указатель на предыдущий элемент

public:
	elem(int t, const char* str): time(t){
		//cout<<"Constr elem"<<endl;
		lastname = new char[strlen(str) + 1];
		lastname = strcpy(lastname, str);
		next = NULL;
		prev = NULL;
	};
	elem (const elem &x) {
		time = x.time;
		lastname = new char[strlen(x.lastname) + 1];
		lastname = strcpy(lastname, x.lastname);
		next = NULL;
		prev = NULL;
	}
	elem(int t): time(t){
		//cout<<"Constr elem"<<endl;
		lastname = new char[1];
		lastname[0] = '\0';
		next = NULL;
		prev = NULL;
	}
	void print_elem(){
		//cout<<"$"<<time<<endl;
		cout<<lastname<<" "<< time<<endl;
	}
	friend list;
	~elem(){//cout<<"Destr elem"<<endl;
		delete[] lastname; 
	}
};

class list {
	elem *l; //указатель на список  
public:
	list(){
		l = NULL;
	}
	list(const elem &x){
		l = new elem(x);
	};
	void print();
	void push_front(const elem &x);
	void push_back(const elem &x);
	elem front();
	elem back();
	void pop_front();
	void pop_back();
	void insert(int y,const elem &x);
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


class Queue:private list {
	static int cnt_all; //информация об общем количестве клиентов во всех очередях
	int cnt; 			//информация о текущем количестве клиентов в очереди
	int max_size;		//максимальный размер очереди
public:
	Queue(const elem &x, int size = 256): list::list(x){
		max_size = size;
		 cnt = 1; 
		 cnt_all++;
	}

	void back(const elem &x){list::push_back(x); cnt++; cnt_all++;} 	//добавления элемента в конец очереди 
	void pop(){list::pop_front(); cnt--; cnt_all--;} 		//удаление первого элемента очереди 
	int empty(){ return list::empty();} //проверка очереди на пустоту 
	int size() { return cnt;} 			//текущее число элементов
	void print() {list::print(); cout<<cnt<<" "<<max_size<<" "<<cnt_all<<endl;} 		//вывод информации об элементах очереди без ее изменения 
	int full(); 						// проверка, что очередь целиком заполнена 
	elem front() {return list::front();}
	//~Queue(){};
};

int Queue::full() {
	//возвращает 0, если очередь переполнена и 1, если размер очереди <= максимального размера
	if (cnt > max_size) return 0;
	else return 1;
}

void list::print(){ //вывод информации об элементах списка
	elem* p = l; int i =1;
	while (p!=NULL) {
		cout<<"el"<<i<<"= ";
		i++;
		p->print_elem();
		p = p->next;
	}
}

void list::push_front(const elem& x){ //добавления элемента в начало
	elem *a;
	a = new elem(x);
	if (l != NULL) {
		a->next = l; 
		l->prev = a; 
	}
	l = a; 
}

void list::push_back(const elem& x){ //добавления элемента в конец
	elem *a, *p = l;
	a = new elem(x);
	if (l == NULL) l = a;
	else {
		while (p->next != NULL)  p = p->next;
		a->prev = p;
		p->next = a;
	}

}

elem list::front(){ //чтение первого элемента списка
	if (l == NULL) {
		cout<<"The list is empty"<<endl;
		elem a(-1);
		return a;
	}
	else {elem a(*l);
	return a;
	} 
}

elem list::back(){ //чтение последнего элемента списка
	elem *p = l;
	if (l == NULL) {
		cout<<"The list is empty"<<endl;
		elem a(-1);
		return a;
	}
	else{
		while (p->next!=NULL) p = p->next;
		elem a(*p);
		return a;
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

void list::insert(int y,const elem &x){ //добавление элемента x перед позицией y 
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

int Queue:: cnt_all = 0;

int main(){

	Queue q1(0, 5); q1.print();
	elem e1(1, "abc");
	Queue q2(e1, 10); q2.print();

	q1.back(e1); q1.print();
	q1.pop(); q1.print();
	q1.pop();

	elem e2(q1.front()); e2.print_elem();
	elem e3(q2.front()); e3.print_elem();

	cout<<endl<<"Testing empty"<<endl;
	cout<<q1.empty()<<endl; 		//expected 0
	cout<<q2.empty()<<endl; 	//expected 1

	cout<<endl<<"Testing size"<<endl;
	cout<<q1.size()<<endl; 	
	q2.back(e1); q2.back(e1); q2.back(e1);	
	cout<<q2.size()<<endl; 

	cout<<endl<<"Testing full"<<endl;
	cout<<q2.full()<<endl; //expected 1
	q1.back(e1); q1.back(e1);  q1.back(e1);  q1.back(e1);  q1.back(e1);  q1.back(e1);
	cout<<q1.full()<<endl; //expected 0

	return 0;
}
