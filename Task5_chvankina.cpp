#include <iostream>

using std::cout;
using std::endl;
using std::cin;

class matrix {
protected:
		int m;   //количество строк в массиве
		int n;   //количество столбцов в массиве
		int **mass; //массив
	public: 
		matrix();
		matrix(int *const*arr, int M, int N);
		matrix(const matrix &matr);
		matrix &operator=(const matrix &matr);
		~matrix() {// std::cout<<" Destr "<<std::endl;
				for (int i = 0; i < m; i++) delete[] mass[i];
				delete [] mass;  };
		void print(){ std::cout<<"m = "<<m<<", n = "<<n<<std::endl;
				for (int i = 0; i < m; i++) {
					for(int j = 0; j < n; j++) {  
						std::cout<<mass[i][j]<<" "; }
					std::cout<<endl;
				}
				std::cout<<std::endl;
		}
		int * operator [] (const int i) {
			if ((i < 0) || (i >= m)) {
				cout<<"Error in operator []"<<endl;
				return 0;
			}
			return mass[i];}

		const int * operator [] (const int i) const { //operator [] for const methods
			if ((i < 0) || (i >= m)) {
				cout<<"Error in operator []"<<endl;
				return 0;
			}
			return mass[i];}
};

matrix:: matrix(){
	//std::cout<<"Constr ()"<<std::endl;
	m = 2;
	n = 2;
	mass = new int* [m];
	for (int i = 0; i < m; i++) {
		mass[i] = new int[n];
		for (int j = 0; j < n; j++) mass[i][j] = 1;
	}
}

matrix:: matrix( int*const* arr, int M, int N){
	//std::cout<<"Constr init"<<std::endl;
	m = M;
	n = N;
	mass = new int* [m];
	for (int i = 0; i < m; i++) {
		mass[i] = new int[n];
		for (int j = 0; j < n; j++) mass[i][j] = arr[i][j];
	}
}

matrix:: matrix(const matrix &matr){
	//std::cout<<"Constr copy"<<std::endl;
	m = matr.m;
	n = matr.n;
	mass = new int* [m];
	for (int i = 0; i < m; i++) {
		mass[i] = new int[n];
		for (int j = 0; j < n; j++) mass[i][j] = matr.mass[i][j];
	}	
}

matrix & matrix::operator=(const matrix &matr){
	if (this != &matr) {
		//std::cout<<"Operator = "<<std::endl; 
		for (int i = 0; i < m; i++) delete[]this->mass[i];
		delete []this->mass;                       
		this->m = matr.m;     
		this->n = matr.n;                    
		this->mass = new int* [m];
		for (int i = 0; i < m; i++) {
			this->mass[i] = new int[n];
			for (int j = 0; j < n; j++) this->mass[i][j] = matr.mass[i][j];       
		}
	}
	return *this;
}

int main(){


	int **p;
	int m = 2;
	int n = 3;
	p = new int*[m];
	for (int i = 0; i < m; i++) {
		p[i] = new int[n];
		for ( int j = 0; j < n; j++) p[i][j] = i + j + 2;
	}
	
	matrix arr1, arr2(p, m, n), arr3(arr2);
		arr1.print();
		arr2.print();
		arr3.print();
		arr1 = arr3;
		cout<<"arr1: "<<endl; arr1.print();
		cout<<"arr1[1][1] = "<< arr1[1][1]; cout<<endl;
		arr1[0][2] = 8;
		cout<<"arr1: "<<endl; arr1.print();

		arr1[5][2]; //expected error in operator []

	const matrix matr;
	cout<<"matr[0][1] = "<<matr[0][1]<<endl;;
//	matr[1][0] = 10; //error in compilation

	for (int i = 0; i < m; i++) delete[] p[i];
	delete [] p;
	return 0;
}
