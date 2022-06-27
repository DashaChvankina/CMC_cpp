#include <iostream>
#include <cstring>

using std::cout;
using std::endl;
using std::cin;
/* Написать абстрактный класс (содержание произвольное) и несколько (>=2) производных
от него классов. Определить независимый от этой иерархии класс, который работает с
массивом объектов типа абстрактного класса.
Использование STL запрещено.
Функция main должна демонстрировать работу с объектами указанных классов.*/

class Products{
public:
	virtual void print()=0;
	virtual ~Products() {
		//cout<<"Destr Products"<<endl;
		 delete[] type; }
	int get_count() { return count;}
protected:
	static int count; //количетсво продуктов
	char* type; 	  //вид продукта
	int cost;   	  //цена продукта
};

class Milk: public Products{
	int time; //срок годности
public: 
	Milk (int x = 68){
		count++;
		type = new char[15];
		strcpy(type, "dairy products");
		time = 7;
		cost = x;
	}
	void print(){
		cout<<"Milk "<<type<<" "<<cost<<" rubles"<<" "<<time<<" days"<<endl;
	}
};

class Kefir: public Products {
	int time; //срок годности
public: 
	Kefir (int x = 70){
		count++;
		type = new char[15];
		strcpy(type, "dairy products");
		time = 3;
		cost = x;
	}
	void print(){
		cout<<"Kefir "<<type<<" "<<cost<<" rubles"<<" "<<time<<" days"<<endl;
	}
};

class Beef: public Products{
	char* store; //условия хранения
public: 
	Beef (int x = 269){
		count++;
		type = new char[5];
		strcpy(type, "Meat");
		cost = x;
		store = new char [32];
		strcpy(store, "To store in freezer for 30 days");
	}
	void print(){
		cout<<"Beef "<<type<<" "<<cost<<" rubles"<<" "<<store<<endl;
	}
	virtual ~Beef(){
		//cout<<"Destr Beef"<<endl;
		delete[] store;
	}
};

class pr_5ka { //магазин пятерочка 
	Products **mass;
	int count; //count of products in shop
public:
	//pr_5ka(){ mass = NULL;}
	pr_5ka(int x = 1, int y = 1, int z = 1 ){ //x-count of milk
											  //y - count of kefit
											  //z - count of beef
	    mass = new Products*[x+y+z];
		int i = 0;
		for (; i < x; i++) mass[i] = new Milk;            
		for (; i < x+y; i++) mass[i] = new Kefir; 
		for (; i < x+y+z; i++) mass[i] = new Beef;
		count = mass[0]->get_count();
	}                                                        
	void print(){
		if (mass != NULL) {
			cout<<count<<endl;
			for (int i = 0; i < mass[0]->get_count(); i++) mass[i]->print(); }
	}
	

	~pr_5ka(){ 
		if (mass != NULL) {
			for (int i = 0; i < mass[0]->get_count(); i++) delete mass[i]; 
		}
	delete[] mass;
	}

};

int Products:: count = 0;
int main(){
	pr_5ka x(3, 6,7);
	x.print();
	return 0;
}