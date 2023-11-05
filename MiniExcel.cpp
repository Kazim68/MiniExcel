#include <iostream>
#include <windows.h>
using namespace std;

template <typename T>
class Cell {
    public:
        T data;
        Cell<T> *up;
        Cell<T> *down;
        Cell<T> *left;
        Cell<T> *right;
        bool active;
        int row;
        int col;
        string color;

        Cell() {
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
            active = false;
            data = "0";
            color = "\33[37m";
        }

        Cell(T data) {
            this->data = data;
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
            active = true;
        }
};

template <typename T>
class Excel {
    public:
        int rows;
        int cols;
        Cell<T> *head;
        Cell<T> *selected;

        Excel(){
            rows = 5;
            cols = 5;
            head = new Cell<T>();
            selected = head;

            this->makeGrid();
        }

        makeGrid(){

            // after head cell
            Cell<T> *temp = head;
            
            // making first column
            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                temp->down = newCell;
                newCell->up = temp;
                temp = temp->down;
            }

            temp = head;

            // making first row
            for (int i = 1; i < cols; i++){
                Cell<T> *newCell = new Cell<T>();
                temp->right = newCell;
                newCell->left = temp;
                temp = temp->right;
            }

            temp = head;

            // now making the rest of the grid
            Cell<T> *prevPointer = head;
            Cell<T> *currPointer = head->right;
            
            for (int i = 1; i < cols; i++){
                Cell<T> *prev = prevPointer->down;
                Cell<T> *curr = currPointer;
                for (int j = 1; j < rows; j++){
                    Cell<T> *newCell = new Cell<T>();
                    prev->right = newCell;
                    newCell->left = prev;
                    prev = prev->down;
                    curr->down = newCell;
                    newCell->up = curr;
                    curr = newCell;
                }
                prevPointer = currPointer;
                currPointer = currPointer->right;
            }
        }

        void insertAbove(){

            // head onto the first cell of the selected row
            Cell<T> *rowHead = selected;
            while(rowHead->left != nullptr){
                rowHead = rowHead->left;
            }

            if (rowHead->up){
                // if there is a row above the selected row

                Cell<T> *prevRowHead = rowHead->up;
                Cell<T> *prevPointer = prevRowHead->right;
                Cell<T> *currPointer = rowHead->right;

                Cell <T> *newRowHead = new Cell<T>();
                prevRowHead->down = newRowHead;
                newRowHead->up = prevRowHead;
                newRowHead->down = rowHead;
                rowHead->up = newRowHead;

                Cell<T> *newRowPointer = newRowHead;

                for (int i = 1; i < cols; i++){

                    Cell<T> *newCell = new Cell<T>();
                    newRowPointer->right = newCell;
                    newCell->left = newRowPointer;

                    prevPointer->down = newCell;
                    newCell->up = prevPointer;

                    currPointer->up = newCell;
                    newCell->down = currPointer;

                    newRowPointer = newCell;
                    prevPointer = prevPointer->right;
                    currPointer = currPointer->right;

                }
            }
            else{
                // this is the first row
                
                Cell<T> *newRowHead = new Cell<T>();
                Cell<T> *temp = head->right;
                head->up = newRowHead;
                newRowHead->down = head;

                Cell<T> *curr = newRowHead;
                for (int i = 1; i < cols; i++){
                    Cell<T> *newCell = new Cell<T>();
                    newCell->left = curr;
                    curr->right = newCell;
                    temp->up = newCell;
                    newCell->down = temp;
                    temp = temp->right;
                    curr = newCell;
                }

                head = newRowHead;
            }
            rows++;
        }

        void insertBelow(){

            // head onto the first cell of the selected row
            Cell<T> *rowHead = selected;
            while(rowHead->left != nullptr){
                rowHead = rowHead->left;
            }

            if (rowHead->down){
                // if there is a row below the selected row
            }
            else{
                // this is the last row

            }
        }

        print(){
            Cell<T> *temp = head;
            Cell<T> *rowTraverser;
            for (int i = 0; i <= cols; i++){
                cout << i << "  ";
            }
            cout << endl;
            for (int i = 0; i < rows; i++){
                cout << i + 1 << " |";
                rowTraverser = temp;
                for (int j = 0; j < cols; j++){
                    cout << rowTraverser->color << rowTraverser->data << " |";
                    rowTraverser = rowTraverser->right;
                }
                cout << endl;
                temp = temp->down;
            }
        }
};

void printKeyManual(){
    cout << "Use arrow keys to navigate" << endl;
    cout << "Use space to enter data" << endl;
    cout << "Use escape to exit" << endl;
    cout << "Press A to insert row above selected cell" << endl;
    cout << "Press B to insert row below selected cell" << endl;
}

int main(){

    printKeyManual();

    Excel<string> *excel = new Excel<string>();
    excel->print();

    bool running = true, modify = false;

    while (running){
        if (GetAsyncKeyState(VK_UP) && excel->selected->up){
            excel->selected->color = "\33[37m";
            excel->selected = excel->selected->up;
            excel->selected->color = "\33[33m";
            modify = true;
        }
        if (GetAsyncKeyState(VK_DOWN) && excel->selected->down){
            excel->selected->color = "\33[37m";
            excel->selected = excel->selected->down;
            excel->selected->color = "\33[33m";
            modify = true;
        }
        if (GetAsyncKeyState(VK_LEFT) && excel->selected->left){
            excel->selected->color = "\33[37m";
            excel->selected = excel->selected->left;
            excel->selected->color = "\33[33m";
            modify = true;
        }
        if (GetAsyncKeyState(VK_RIGHT) && excel->selected->right){
            excel->selected->color = "\33[37m";
            excel->selected = excel->selected->right;
            excel->selected->color = "\33[33m";
            modify = true;
        }
        if (GetAsyncKeyState(VK_SPACE)){
            cout << "Enter data: ";
            string data;
            cin >> data;
            excel->selected->data = data;

            modify = true;
        }
        if (GetAsyncKeyState('A')){
            excel->insertAbove();
            modify = true;
        }
        if (GetAsyncKeyState(VK_ESCAPE)){
            running = false;
        }

        if (modify){
            system("cls");
            printKeyManual();
            excel->print();
            modify = false;
        }

        Sleep(100);
    }
    return 0;
}