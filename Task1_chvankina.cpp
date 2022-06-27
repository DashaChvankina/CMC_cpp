#include <iostream>

class matrix {
		int len;
		int *mass;
	public: 
		matrix();
		matrix(const int *arr, int N);
		matrix(const matrix &matr);
		matrix &operator=(const matrix &arr);
		~matrix() { std::cout<<" Destr "<<std::endl;
				delete [] this->mass;};
		void print(){ std::cout<<"  len = "<<len<<std::endl;
				for (int i = 0; i < len; i++) std::cout<<mass[i];
				std::cout<<std::endl;
		}
};

matrix:: matrix(){
	std::cout<<"Constr ()"<<std::endl;
	len = 0;
	mass = NULL;
}

matrix:: matrix(const int *arr, int N){
	std::cout<<"Constr init"<<std::endl;
	len = N;
	mass = new int[len];
	for (int i = 0; i < N; i++) mass[i] = arr[i];
}

matrix:: matrix(const matrix &matr){
	std::cout<<"Constr copy"<<std::endl;
	mass = new int[matr.len];
	len = matr.len;
	for (int i = 0; i < matr.len; i++) mass[i] = matr.mass[i];
}

matrix & matrix::operator=(const matrix &arr){
	if (this != &arr) {
		std::cout<<"Operator = "<<std::endl; 
		delete []this->mass;                       
		this->len = arr.len;                       
		this->mass = new int[arr.len];              
		for (int i = 0; i < arr.len; i++) this->mass[i] = arr.mass[i];            
	}
	return *this;
}

int main(){
	int *p, *x;
	p = new int[5];
	int N = 5;
	for (int i = 0; i < N; i++) p[i] = i + 1;
	
	x = new int[6];
	int n = 6;
	for (int i = 0; i < n; i++) x[i] = i + 3;
	matrix arr1, arr2(p, 5), arr3(arr2), arr4 = arr1, arr5(x, 6);
		arr1.print();
		arr2.print();
		arr3.print();
		arr4.print();
		arr5.print();
		arr5 = arr2;
		arr5.print();
	delete []p;
	delete []x;
	return 0;
}
