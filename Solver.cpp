#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Data{
  int rows;
  int columns;
  vector<vector<int>> Matrix;
  string filename;
};


struct Sparse_Node{
	public: 
		int Data;
		Sparse_Node *Right;
		Sparse_Node *Below;
};

class Sparse_List{
	public:
		Sparse_Node *First_Element;
		int Rows;
		int Columns;
		
		Sparse_List() : First_Element(NULL) {}

		void Print_Sparse_List();
		void Save(string filename);
};

void Sparse_List::Print_Sparse_List(){
	Sparse_Node *Beginning = First_Element;
	Sparse_Node *Temp = First_Element;


	while(Beginning->Below != NULL){
		while(Temp->Right != NULL){
			cout << Temp->Data << " ";
			Temp = Temp->Right;
		}
		cout << Temp->Data << " ";
		Temp = Beginning->Below;
		Beginning = Beginning->Below;
		cout << endl;
	}

	while(Temp->Right != NULL){
		cout << Temp->Data << " ";
		Temp = Temp->Right;
	}
	cout << Temp->Data << " ";
	cout << endl;

	return;
}

void Sparse_List::Save(string filename){
	Sparse_Node *Beginning = First_Element;
	Sparse_Node *Temp = First_Element;

	ofstream myfile (filename);
	if (myfile.is_open()){

		while(Beginning->Below != NULL){
			while(Temp->Right != NULL){
				myfile << Temp->Data << " ";
				Temp = Temp->Right;
			}
			myfile << Temp->Data << " ";
			Temp = Beginning->Below;
			Beginning = Beginning->Below;
			myfile << endl;
		}

		while(Temp->Right != NULL){
			myfile << Temp->Data << " ";
			Temp = Temp->Right;
		}
		myfile << Temp->Data << " ";

		myfile.close();

	} else {
		cout << "Unable to open file";	
	}

	return;
}

int GetRows(string filename){
	ifstream file;
	string line;

	file.open(filename);
	int rows = 0;
	while (!file.eof()){
		while (getline(file, line)) {
    		stringstream ss(line);
			int i;
			while (ss >> i){
				if (ss.peek() == '\0'){
					ss.ignore();
				}
			}
			rows = rows + 1;
		}
	}
	file.close();
	return rows;
}

int GetColumns(string filename){
	ifstream file;
	string line;

	vector <int> Columns_length;

	file.open(filename);
	int columns = 0;
	while (!file.eof()){
		while (getline(file, line)) {
			columns = 0;
    		stringstream ss(line);
			int i;

			while (ss >> i){
				columns = columns + 1;
				if (ss.peek() == '\0'){
					ss.ignore();
				}
			}

			Columns_length.push_back(columns);
		}
	}

	file.close();

	if(Columns_length.size() == 0){
		return 0;
	}else{
		int n = Columns_length[0];
		for(int i = 0; i < Columns_length.size(); i ++){
			if(Columns_length[i] != n){
				return 0;
			}
		}
	}

	return columns;
}

//This function is capable of reading and returning an vector of max size of 100 x 100.
vector<vector<int>> Read_Matrix_From_File(string filename, int rows, int columns){
	ifstream file(filename);
	
	vector<vector<int>> Matrix (rows);

	for (int i = 0; i < rows; ++i) {
		Matrix[i] = vector<int>(columns);

		for (int j = 0; j < columns; ++j) {
		file >> Matrix[i][j];
		}
	}

	return Matrix;
}

//Function converts a Matrix into a Sparse Linked List.
Sparse_List Convert_Data_To_List(Data Received_Matrix){
	Sparse_List Matrix_List = Sparse_List();

	vector<Sparse_Node*> Nodes;

	for (int R = 0; R < Received_Matrix.rows; R ++){
		for (int C = 0; C < Received_Matrix.columns; C ++){
			Sparse_Node *NSN = new Sparse_Node;
			NSN->Data = Received_Matrix.Matrix[R][C];
			NSN->Right = NULL;
			NSN->Below = NULL;
			Nodes.push_back(NSN);
		}
	}

	for(int N = 0; N < Nodes.size(); N ++){
		//Ultimo elemento de Matriz, Inferior Izqierdo.
		if (N == (Nodes.size() - 1)){
			Nodes[N]->Right = NULL;
			Nodes[N]->Below = NULL;
		//Ultima columna de Matriz.
		}else if((N + 1) % Received_Matrix.columns == 0){
			if(N != 0){
				Nodes[N]->Right = NULL;
				Nodes[N]->Below = Nodes[N + Received_Matrix.columns];
			}
		//Ultima fila de Matriz.
		}else if (N >= (Nodes.size() - Received_Matrix.columns)){
			Nodes[N]->Right = Nodes[N + 1];
			Nodes[N]->Below = NULL;
		}else{
			Nodes[N]->Right = Nodes[N + 1];
			Nodes[N]->Below = Nodes[N + Received_Matrix.columns];
		}

	}

	Matrix_List.First_Element = Nodes[0];
	Matrix_List.Rows = Received_Matrix.rows;
	Matrix_List.Columns = Received_Matrix.columns;

	return Matrix_List;
}

Sparse_List Load(){
	Sparse_List Result_List = Sparse_List();

	string filename;
	cout << "Please input the name of the file where The Matrix is defined: ";
	cin >> filename;
	ifstream file(filename);

	Data Matrix;

	if(file.good()){
		int rows = GetRows(filename);

		int columns = GetColumns(filename);

		if(columns == 0){
			return Result_List;
		}
		cout << "File found!" << endl;

		Matrix.filename = filename;
		Matrix.rows = rows;
		Matrix.columns = columns;
		Matrix.Matrix = Read_Matrix_From_File(filename, rows, columns);

	}else{
		cout << "The file you requested doesn't exist!" << endl;
		return Result_List;
	}

	Result_List = Convert_Data_To_List(Matrix);

	return Result_List;
}

Sparse_List Suma(Sparse_List A, Sparse_List B){
	Sparse_List Result_List = Sparse_List();

	if (A.Rows != B.Rows){
		cout << "The amount of rows in A doesn't match the amount of rows in B." << endl;
		cout << "No action will be performed." << endl;
	}else if (A.Columns != B.Columns){
		cout << "The amount of columns in A doesn't match the amount of columns in B." << endl;
		cout << "No action will be performed." << endl;
	}else {
		vector<vector<int>> Matrix (A.Rows);


		for (int i = 0; i < A.Rows; ++i) {
			Matrix[i] = vector<int>(A.Columns);
		}
		
		Sparse_Node *Beginning_A = A.First_Element;
		Sparse_Node *Temp_A = A.First_Element;


		Sparse_Node *Beginning_B = B.First_Element;
		Sparse_Node *Temp_B = B.First_Element;

		int Row = 0;
		int Column = 0;

		while(Beginning_A->Below != NULL){
			while(Temp_A->Right != NULL){
				Matrix[Row][Column] = Temp_A->Data + Temp_B->Data;
				Temp_A = Temp_A->Right;
				Temp_B = Temp_B->Right;
				Column = Column + 1;
			}
			Matrix[Row][Column] = Temp_A->Data + Temp_B->Data;
			Temp_A = Beginning_A->Below;
			Temp_B = Beginning_B->Below;
			Beginning_A = Beginning_A->Below;
			Beginning_B = Beginning_B->Below;
			Row = Row + 1;
			Column = 0;
		}

		while(Temp_A->Right != NULL){
			Matrix[Row][Column] = Temp_A->Data + Temp_B->Data;
			Temp_A = Temp_A->Right;
			Temp_B = Temp_B->Right;
			Column = Column + 1;
		}
		Matrix[Row][Column] = Temp_A->Data + Temp_B->Data;
	
		Data Temp_Data;
		Temp_Data.filename = "";
		Temp_Data.rows = A.Rows;
		Temp_Data.columns = B.Columns;
		Temp_Data.Matrix = Matrix;

		Result_List = Convert_Data_To_List(Temp_Data);
	}

	return Result_List;
}

Sparse_List Resta(Sparse_List A, Sparse_List B){
	Sparse_List Result_List = Sparse_List();

	if (A.Rows != B.Rows){
		cout << "The amount of rows in A doesn't match the amount of rows in B." << endl;
		cout << "No action will be performed." << endl;
	}else if (A.Columns != B.Columns){
		cout << "The amount of columns in A doesn't match the amount of columns in B." << endl;
		cout << "No action will be performed." << endl;
	}else {
		vector<vector<int>> Matrix (A.Rows);


		for (int i = 0; i < A.Rows; ++i) {
			Matrix[i] = vector<int>(A.Columns);
		}
		
		Sparse_Node *Beginning_A = A.First_Element;
		Sparse_Node *Temp_A = A.First_Element;


		Sparse_Node *Beginning_B = B.First_Element;
		Sparse_Node *Temp_B = B.First_Element;

		int Row = 0;
		int Column = 0;
		
		int Temp1;
		int Temp2;

		while(Beginning_A->Below != NULL){
			while(Temp_A->Right != NULL){
				Temp1 = Temp_A->Data;
				Temp2 = Temp_B->Data;
				Matrix[Row][Column] = Temp1 - Temp2;
				Temp_A = Temp_A->Right;
				Temp_B = Temp_B->Right;
				Column = Column + 1;
			}
			Temp1 = Temp_A->Data;
			Temp2 = Temp_B->Data;
			Matrix[Row][Column] = Temp1 - Temp2;
			Temp_A = Beginning_A->Below;
			Temp_B = Beginning_B->Below;
			Beginning_A = Beginning_A->Below;
			Beginning_B = Beginning_B->Below;
			Row = Row + 1;
			Column = 0;
		}

		while(Temp_A->Right != NULL){
			Temp1 = Temp_A->Data;
			Temp2 = Temp_B->Data;
			Matrix[Row][Column] = Temp1 - Temp2;
			Temp_A = Temp_A->Right;
			Temp_B = Temp_B->Right;
			Column = Column + 1;
		}
		Temp1 = Temp_A->Data;
		Temp2 = Temp_B->Data;
		Matrix[Row][Column] = Temp1 - Temp2;
	
		Data Temp_Data;
		Temp_Data.filename = "";
		Temp_Data.rows = A.Rows;
		Temp_Data.columns = B.Columns;
		Temp_Data.Matrix = Matrix;

		Result_List = Convert_Data_To_List(Temp_Data);
	}

	return Result_List;
}

Sparse_List Multiplicacion(Sparse_List A, Sparse_List B){
	Sparse_List Result_List = Sparse_List();

	if (A.Rows != B.Columns){
		cout << "The amount of rows in A doesn't match the amount of columns in B." << endl;
		cout << "No action will be performed." << endl;
	}else {
		vector<vector<int>> Matrix_A (A.Rows);
		for (int i = 0; i < A.Rows; ++i) {
			Matrix_A[i] = vector<int>(A.Columns);
		}

		vector<vector<int>> Matrix_B (B.Rows);
		for (int i = 0; i < B.Rows; ++i) {
			Matrix_B[i] = vector<int>(B.Columns);
		}

		vector<vector<int>> Matrix_R (A.Rows);
		for (int i = 0; i < A.Rows; ++i) {
			Matrix_R[i] = vector<int>(B.Columns);
		}

		//Fill Matrix A.
		Sparse_Node *Beginning = A.First_Element;
		Sparse_Node *Temp = A.First_Element;
		int R = 0;
		int C = 0;

		while(Beginning->Below != NULL){
			while(Temp->Right != NULL){
				Matrix_A[R][C] = Temp->Data;
				C ++;
				Temp = Temp->Right;
			}
			Matrix_A[R][C] = Temp->Data;
			C = 0;
			Temp = Beginning->Below;
			Beginning = Beginning->Below;
			R ++;
		}

		while(Temp->Right != NULL){
			Matrix_A[R][C] = Temp->Data;
			C ++;
			Temp = Temp->Right;
		}
		Matrix_A[R][C] = Temp->Data;

		//Fill Matrix B.
		Sparse_Node *Beginning_B = B.First_Element;
		Sparse_Node *Temp_B = B.First_Element;
		R = 0;
		C = 0;

		while(Beginning_B->Below != NULL){
			while(Temp_B->Right != NULL){
				Matrix_B[R][C] = Temp_B->Data;
				C ++;
				Temp_B = Temp_B->Right;
			}
			Matrix_B[R][C] = Temp_B->Data;
			C = 0;
			Temp_B = Beginning_B->Below;
			Beginning_B = Beginning_B->Below;
			R ++;
		}

		while(Temp_B->Right != NULL){
			Matrix_B[R][C] = Temp_B->Data;
			C ++;
			Temp_B = Temp_B->Right;
		}
		Matrix_B[R][C] = Temp_B->Data;

		//Multiply.
		for(int i=0; i < A.Rows; ++i){
			for(int j=0; j < B.Columns; ++j){
				for(int k=0; k < A.Columns; ++k)
				{
					Matrix_R[i][j] += Matrix_A[i][k] * Matrix_B[k][j];
				}
			}
		}
		
		Data Temp_Data;
		Temp_Data.filename = "";
		Temp_Data.rows = A.Rows;
		Temp_Data.columns = B.Columns;
		Temp_Data.Matrix = Matrix_R;

		Result_List = Convert_Data_To_List(Temp_Data);
	}

	return Result_List;
}

int Determinante(Sparse_List A){
	int Result = -1;

	if(A.Rows != A.Columns){
		cout << "Matrix is not a squared matrix." << endl;
	}else if (A.Columns == 1){
		Result = A.First_Element->Data;
	}else if (A.Columns == 2){
		int D1 = A.First_Element->Data * A.First_Element->Right->Below->Data;
		int D2 = A.First_Element->Right->Data * A.First_Element->Below->Data;
		Result = D1 - D2;
	}else if (A.Columns == 3){
		int D11 = A.First_Element->Data * A.First_Element->Right->Below->Data * A.First_Element->Right->Below->Right->Below->Data;
		int D12 = A.First_Element->Right->Data * A.First_Element->Right->Below->Right->Data * A.First_Element->Below->Below->Data;
		int D13 = A.First_Element->Below->Data * A.First_Element->Below->Below->Right->Data * A.First_Element->Right->Right->Data;
		int D21 = A.First_Element->Right->Right->Data * A.First_Element->Right->Below->Data * A.First_Element->Below->Below->Data;
		int D22 = A.First_Element->Right->Data * A.First_Element->Below->Data * A.First_Element->Right->Right->Below->Below->Data;
		int D23 = A.First_Element->Right->Right->Below->Data * A.First_Element->Right->Below->Below->Data * A.First_Element->Data;
		Result = (D11 + D12 + D13) - (D21 + D22 + D23);
	}else {
		cout << "Matrix determinant to be solved using Laplace." << endl;
	}

	return Result;
}

int main() {
	int option = -1;
	Sparse_List A = Sparse_List();
	Sparse_List B = Sparse_List();
	Sparse_List Last_Result = Sparse_List();
	
	cout << "Matrices Solver" << endl;

	while(option != 7){
		cout << "Please select an option: " << endl;
		cout << "1. Load a Matrix From File" << endl;
		cout << "2. Matrices Addition" << endl;
		cout << "3. Matrices Substraction" << endl;
		cout << "4. Matrices Multiplication" << endl;
		cout << "5. Matrix Determinant" << endl;
		cout << "6. Print Matrix" << endl;
		cout << "7. Exit" << endl;

		cin >> option;

		if (option == 1){
			cout << "You chose to Load a Matrix From File" << endl;
			
			//Check availability.
			if(A.First_Element != NULL){
				cout << "Matrix A has already been loaded. You can choose to overwrite it." << endl;
			}

			if(B.First_Element != NULL){
				cout << "Matrix B has already been loaded. You can choose to overwrite it." << endl;
			}

			int selection = -1;
			while (selection != 3){
				cout << "Which Matrix do you which to load? " << endl;
				cout << "1. A" << endl;
				cout << "2. B" << endl;
				cout << "3. Cancel" << endl;
				cin >> selection;

				if (selection == 1){
					A = Load();
					if(A.First_Element != NULL){
						cout << "You loaded the following Matrix into A: " << endl;
						A.Print_Sparse_List();
					}else{
						cout << "Error loading matrix." << endl;
					}
					break;
				}else if (selection == 2){
					B = Load();
					if(B.First_Element != NULL){
						cout << "You loaded the following Matrix into B: " << endl;
						B.Print_Sparse_List();
					}else{
						cout << "Error loading matrix." << endl;
					}
					break;
				}else if (selection == 3){}else {
					cout << "Please enter a valid option." << endl;
				}
			}


		}else if (option == 2){
			cout << "You chose Matrices Addition" << endl;
			if (A.First_Element == NULL){
				cout << "Matrix A is empty or has not been loaded." << endl;
			}else if (B.First_Element == NULL){
				cout << "Matrix B is empty or has not been loaded." << endl;
			}else {
				Last_Result = Suma(A, B);
				if (Last_Result.First_Element != NULL){
					cout << "The operation yielded the following result: " << endl;
					Last_Result.Print_Sparse_List();
					string filename;
					cout << "Please input the name of the file where you will store the result: ";
					cin >> filename;
					Last_Result.Save(filename);
				}
			}

		}else if (option == 3){
			cout << "You chose Matrices Substraction" << endl;
			if (A.First_Element == NULL){
				cout << "Matrix A is empty or has not been loaded." << endl;
			}else if (B.First_Element == NULL){
				cout << "Matrix B is empty or has not been loaded." << endl;
			}else {
				Last_Result = Resta(A, B);
				if (Last_Result.First_Element != NULL){
					cout << "The operation yielded the following result: " << endl;
					Last_Result.Print_Sparse_List();
					string filename;
					cout << "Please input the name of the file where you will store the result: ";
					cin >> filename;
					Last_Result.Save(filename);
				}
			}

		}else if (option == 4){
			cout << "You chose Matrices Multiplication" << endl;if (A.First_Element == NULL){
				cout << "Matrix A is empty or has not been loaded." << endl;
			}else if (B.First_Element == NULL){
				cout << "Matrix B is empty or has not been loaded." << endl;
			}else {
				Last_Result = Multiplicacion(A, B);
				if (Last_Result.First_Element != NULL){
					cout << "The operation yielded the following result: " << endl;
					Last_Result.Print_Sparse_List();
					string filename;
					cout << "Please input the name of the file where you will store the result: ";
					cin >> filename;
					Last_Result.Save(filename);
				}
			}

		}else if (option == 5){
			cout << "You chose Matrix Determinant" << endl;
			int Determinant;
			cout << "Choose a Matrix: " << endl;

			int selection = -1;
			while (selection != 4){
				cout << "Which Matrix do you whish to work with? " << endl;
				cout << "1. A" << endl;
				cout << "2. B" << endl;
				cout << "3. Last Result" << endl;
				cout << "4. Cancel" << endl;
				cin >> selection;

				string filename;

				if (selection == 1){
					Determinant = Determinante(A);
					if(Determinant != -1){
						cout << "Determinante de A: " << Determinant << endl;
						cout << "Please input the name of the file where you will store the result: ";
						cin >> filename;
						ofstream myfile (filename);
						if (myfile.is_open()){
							myfile << "Deeterminant:" << Determinant;
						}
					}
				}else if (selection == 2){
					Determinant = Determinante(B);
					if(Determinant != -1){
						cout << "Determinante de B: " << Determinant << endl;
						cout << "Please input the name of the file where you will store the result: ";
						cin >> filename;
						ofstream myfile (filename);
						if (myfile.is_open()){
							myfile << "Deeterminant:" << Determinant;
						}
					}
				}else if (selection == 3){
					Determinant = Determinante(Last_Result);
					if(Determinant != -1){
						cout << "Determinante de LR: " << Determinant << endl;
						cout << "Please input the name of the file where you will store the result: ";
						cin >> filename;
						ofstream myfile (filename);
						if (myfile.is_open()){
							myfile << "Deeterminant:" << Determinant;
						}
					}
				}else if (selection == 4){}else {
					cout << "Please enter a valid option." << endl;
				}
			}
		}else if (option == 6){
			cout << "You chose Print Matrix" << endl;
			int Determinant;
			cout << "Choose a Matrix: " << endl;

			int selection = -1;
			while (selection != 4){
				cout << "Which Matrix do you whish to print? " << endl;
				cout << "1. A" << endl;
				cout << "2. B" << endl;
				cout << "3. Last Result" << endl;
				cout << "4. Cancel" << endl;
				cin >> selection;

				if (selection == 1){
					if(A.First_Element != NULL){
						A.Print_Sparse_List();
					}else{
						cout << "Matrix A is empty or has not been loaded." << endl;
					}
				}else if (selection == 2){
					if(B.First_Element != NULL){
						B.Print_Sparse_List();
					}else{
						cout << "Matrix B is empty or has not been loaded." << endl;
					}
				}else if (selection == 3){
					if(Last_Result.First_Element != NULL){
						Last_Result.Print_Sparse_List();
					}else{
						cout << "Matrix Last Result is empty or has not been loaded." << endl;
					}
				}else if (selection == 4){}else {
					cout << "Please enter a valid option." << endl;
				}
			}
		}else if (option == 7){}else {
			cout << "Please enter a valid option. No action will be excecuted." << endl;
		}

	}
	
}