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

                Cell<T> *nextRowHead = rowHead->down;
                Cell<T> *nextPointer = nextRowHead->right;
                Cell<T> *currPointer = rowHead->right;

                Cell <T> *newRowHead = new Cell<T>();
                nextRowHead->up = newRowHead;
                newRowHead->down = nextRowHead;
                newRowHead->up = rowHead;
                rowHead->down = newRowHead;

                Cell<T> *newRowPointer = newRowHead;

                for (int i = 1; i < cols; i++){

                    Cell<T> *newCell = new Cell<T>();
                    newRowPointer->right = newCell;
                    newCell->left = newRowPointer;

                    nextPointer->up = newCell;
                    newCell->down = nextPointer;

                    currPointer->down = newCell;
                    newCell->up = currPointer;

                    newRowPointer = newCell;
                    nextPointer = nextPointer->right;
                    currPointer = currPointer->right;

                }
            }
            else{
                // this is the last row
                makeNewRowAtBottom(rowHead);

            }
            rows++;
        }

        void makeNewRowAtBottom(Cell<T> *rowHead){
            Cell<T> *newRowHead = new Cell<T>();
            Cell<T> *temp = rowHead->right;
            rowHead->down = newRowHead;
            newRowHead->up = rowHead;

            Cell<T> *curr = newRowHead;
            for (int i = 1; i < cols; i++){
                Cell<T> *newCell = new Cell<T>();
                newCell->left = curr;
                curr->right = newCell;
                temp->down = newCell;
                newCell->up = temp;
                temp = temp->right;
                curr = newCell;
            }
        }

        void insertRight(){
            
            // get the first cell of the selected column
            Cell<T> *colHead = selected;
            while (colHead->up != nullptr){
                colHead = colHead->up;
            }

            if (colHead->right){
                // if there is a column to the right of the selected column

                Cell<T> *nextColHead = colHead->right;
                Cell<T> *nextPointer = nextColHead->down;
                Cell<T> *currPointer = colHead->down;
                Cell<T> *newColHead = new Cell<T>();
                newColHead->left = colHead;
                colHead->right = newColHead;
                newColHead->right = nextColHead;
                nextColHead->left = newColHead;

                for (int i = 1; i < rows; i++){
                    Cell<T> *newCell = new Cell<T>();
                    newColHead->down = newCell;
                    newCell->up = newColHead;
                    nextPointer->left = newCell;
                    newCell->right = nextPointer;
                    currPointer->right = newCell;
                    newCell->left = currPointer;

                    newColHead = newCell;
                    nextPointer = nextPointer->down;
                    currPointer = currPointer->down;
                }
            }
            else{
                // if it is the last column
                makeNewColumnAtRight(colHead);
            }

            cols++;
        }

        void makeNewColumnAtRight(Cell<T> *colHead){
            Cell<T> *newColHead = new Cell<T>();
            Cell<T> *temp = colHead->down;
            colHead->right = newColHead;
            newColHead->left = colHead;
            Cell<T> *curr = newColHead;

            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                curr->down = newCell;
                newCell->up = curr;
                temp->right = newCell;
                newCell->left = temp;

                curr = newCell;
                temp = temp->down;
            }
        }

        void insertLeft(){

            // get the first cell of the selected column
            Cell<T> *colHead = selected;
            while (colHead->up != nullptr){
                colHead = colHead->up;
            }

            if (colHead->left){
                // if there is a column to the left of the selected column

                Cell<T> *prevColHead = colHead->left;
                Cell<T> *prevPointer = prevColHead->down;
                Cell<T> *currPointer = colHead->down;
                Cell<T> *newColHead = new Cell<T>();
                newColHead->right = colHead;
                colHead->left = newColHead;
                newColHead->left = prevColHead;
                prevColHead->right = newColHead;

                for (int i = 1; i < rows; i++){
                    Cell<T> *newCell = new Cell<T>();
                    newColHead->down = newCell;
                    newCell->up = newColHead;
                    prevPointer->right = newCell;
                    newCell->left = prevPointer;
                    currPointer->left = newCell;
                    newCell->right = currPointer;

                    newColHead = newCell;
                    prevPointer = prevPointer->down;
                    currPointer = currPointer->down;
                }
            }
            else{
                // if this is the first column
                makeNewColumnAtLeft(colHead);
                
            }
            cols++;
        }

        void makeNewColumnAtLeft(Cell<T> *colHead){
            Cell<T> *newColHead = new Cell<T>();
            Cell<T> *temp = colHead->down;
            colHead->left = newColHead;
            newColHead->right = colHead;
            head = newColHead;

            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                newColHead->down = newCell;
                newCell->up = newColHead;
                temp->left = newCell;
                newCell->right = temp;

                newColHead = newCell;
                temp = temp->down;
            }
        }

        void insertCellByRightShift(){

            // insert a new column at the right most end first
            Cell<T> *temp = selected;
            
            // getting to the right most end top cell
            while (temp->right != nullptr){
                temp = temp->right;
            }

            while (temp->up != nullptr){
                temp = temp->up;
            }

            // now temp is the top right most cell
            // inserting a new column at the right most end
            makeNewColumnAtRight(temp);

            // from selected cell, right shift all the cells
            Cell<T> *current = selected;
            temp = current->left;

            Cell<T> *newCell = new Cell<T>();
            if (temp){
                temp->right = newCell;
                newCell->left = temp;
            }
            newCell->right = current;
            current->left = newCell;

            temp = newCell;

            // if it is the first row
            if (!current->up){
                Cell<T> *below = current->down;

                if (selected == head){
                    head = newCell;
                }

                while (below){
                    temp->down = below;
                    below->up = temp;

                    temp = temp->right;
                    below = below->right;
                }

            }
            else if (!current->down){
                // if it is the last row
                Cell<T> *above = current->up;

                while (above){
                    temp->up = above;
                    above->down = temp;

                    temp = temp->right;
                    above = above->right;
                }

            } 
            else {
                // if it is a middle row
                Cell<T> *above = current->up;
                Cell<T> *below = current->down;

                while (above){
                    temp->up = above;
                    above->down = temp;
                    below->up = temp;
                    temp->down = below;

                    temp = temp->right;
                    above = above->right;
                    below = below->right;
                }

            }
            temp->left->right = nullptr;
            cols++;
        }

        void insertCellByDownShift(){
            // insert a new row at the bottom most end first
            Cell<T> *temp = selected;

            // get to the bottom most end left cell
            while (temp->down != nullptr){
                temp = temp->down;
            }

            while (temp->left != nullptr){
                temp = temp->left;
            }

            // now temp is the bottom left most cell
            // inserting a new row at the bottom most end
            makeNewRowAtBottom(temp);

            // from selected cell, down shift all the cells
            Cell<T> *current = selected;
            temp = current->up;

            Cell<T> *newCell = new Cell<T>();
            if (temp){
                temp->down = newCell;
                newCell->up = temp;
            }
            newCell->down = current;
            current->up = newCell;

            temp = newCell;

            if (!current->left){
                // it is the first column
                Cell<T> *right = current->right;

                if (selected == head){
                    head = newCell;
                }

                while (right){
                    temp->right = right;
                    right->left = temp;

                    temp = temp->down;
                    right = right->down;
                }
            }
            else if (!current->right){
                // it is the last column
                Cell<T> *left = current->left;

                while (left){
                    temp->left = left;
                    left->right = temp;

                    temp = temp->down;
                    left = left->down;
                }
            }
            else{
                // it is a middle column
                Cell<T> *left = current->left;
                Cell<T> *right = current->right;

                while (right){
                    temp->right = right;
                    right->left = temp;
                    left->right = temp;
                    temp->left = left;

                    temp = temp->down;
                    right = right->down;
                    left = left->down;
                }
            }

            temp->up->down = nullptr;
            rows++;
        }

        void deleteCellByLeftShift(){
            Cell<T> *current = selected;

            if (!current->left){
                // it is the first column
            }
            else if (!current->right){
                // it is the last column
            }
            else{
                // it is a middle column
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
    cout << "Press R to insert column to the right of selected cell" << endl;
    cout << "Press L to insert column to the left of selected cell" << endl;
    cout << "Press CTRL to insert cells by right shift" << endl;
    cout << "Press Shift to insert cells by down shift" << endl;
    cout << "Press Alt to delete cells by left shift" << endl;
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
            cin.ignore();

            modify = true;
        }
        else if (GetAsyncKeyState('A')){
            excel->insertAbove();
            modify = true;
        }
        else if (GetAsyncKeyState('B')){
            excel->insertBelow();
            modify = true;
        }
        else if (GetAsyncKeyState('R')){
            excel->insertRight();
            modify = true;
        }
        else if (GetAsyncKeyState('L')){
            excel->insertLeft();
            modify = true;
        }
        else if (GetAsyncKeyState(VK_CONTROL)){
            excel->insertCellByRightShift();
            modify = true;
        }
        else if (GetAsyncKeyState(VK_SHIFT)){
            excel->insertCellByDownShift();
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