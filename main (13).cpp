#include <iostream>

template<typename X>
class List {
private:
    // Внутренний класс Node
    class Node {
    public:
        X data;
        Node* next;
        
        // Конструктор узла
        Node(const X& d) : data(d), next(nullptr) {}
    };

    Node *headList;  // Указатель на голову
    Node *tailList;  // Указатель на последний элемент списка
    Node *currentList; // Указатель на текущий элемент списка

public:
    // Конструктор по умолчанию
    List() : headList(nullptr), tailList(nullptr), currentList(nullptr) {}

    // Конструктор копирования
    List(const List& other) {
        headList = tailList = currentList = nullptr;
        Node* temp = other.headList;
        while (temp != nullptr) {
            AddToLast(temp->data);
            temp = temp->next;
        }
    }

    // Оператор присвоения
    List<X>& operator=(const List& other) {
        if (this != &other) {
            Clear();
            Node* temp = other.headList;
            while (temp != nullptr) {
                AddToLast(temp->data);
                temp = temp->next;
            }
        }
        return *this;
    }

    // Деструктор
    ~List() {
        Clear();
    }

    // Добавление элемента в голову
    void AddToHead(const X& elem) {
        Node* newNode = new Node(elem);
        if (headList == nullptr) {
            headList = tailList = newNode;
        } else {
            newNode->next = headList;
            headList = newNode;
        }
    }

    // Добавление элемента в хвост
    void AddToLast(const X& elem) {
        Node* newNode = new Node(elem);
        if (tailList == nullptr) {
            headList = tailList = newNode;
        } else {
            tailList->next = newNode;
            tailList = newNode;
        }
    }

    // Добавление на текущую позицию
    void AddToCurrent(const X& elem) {
        if (currentList == nullptr) {
            // Если текущая позиция не установлена, добавляем в конец
            AddToLast(elem);
        } else {
            Node* newNode = new Node(elem);
            newNode->next = currentList->next;
            currentList->next = newNode;
            if (currentList == tailList) {
                tailList = newNode;
            }
        }
    }

    // Удаление "головы" списка
    void DelHead() {
        if (headList != nullptr) {
            Node* temp = headList;
            headList = headList->next;
            delete temp;
            if (headList == nullptr) {
                tailList = nullptr;
            }
        }
    }

    // Удаление "хвоста" списка
    void DelLast() {
        if (headList != nullptr) {
            if (headList == tailList) {
                delete headList;
                headList = tailList = nullptr;
            } else {
                Node* temp = headList;
                while (temp->next != tailList) {
                    temp = temp->next;
                }
                delete tailList;
                tailList = temp;
                tailList->next = nullptr;
            }
        }
    }

    // Удаление элемента на текущей позиции
    void DelCurrent() {
        if (currentList != nullptr) {
            if (currentList == headList) {
                DelHead();
            } else if (currentList == tailList) {
                DelLast();
            } else {
                Node* temp = headList;
                while (temp->next != currentList) {
                    temp = temp->next;
                }
                temp->next = currentList->next;
                delete currentList;
                currentList = temp->next;
            }
        }
    }

    // Удалить заданный элемент
    void Del(const X& elem) {
        Node* temp = headList;
        Node* prev = nullptr;
        while (temp != nullptr) {
            if (temp->data == elem) {
                if (temp == headList) {
                    DelHead();
                    temp = headList;
                } else if (temp == tailList) {
                    DelLast();
                    temp = nullptr;
                } else {
                    prev->next = temp->next;
                    delete temp;
                    temp = prev->next;
                }
            } else {
                prev = temp;
                temp = temp->next;
            }
        }
    }

    // Удаление элемента на заданной позиции
    bool Del(long position) {
        if (position < 0) {
            return false;
        }
        if (position == 0) {
            DelHead();
            return true;
        }
        long index = 0;
        Node* temp = headList;
        Node* prev = nullptr;
        while (temp != nullptr && index < position) {
            prev = temp;
            temp = temp->next;
            index++;
        }
        if (temp == nullptr) {
            return false;
        }
        prev->next = temp->next;
        if (temp == tailList) {
            tailList = prev;
        }
        delete temp;
        return true;
    }

    // Сдвиг указателя current на следующий элемент
    bool next() {
        if (currentList == nullptr || currentList->next == nullptr) {
            return false;
        }
        currentList = currentList->next;
        return true;
    }

    // Сдвиг указателя current на первый элемент
    void toBegin() {
        currentList = headList;
    }

    // Сдвиг указателя current на последний элемент
    void toEnd() {
        currentList = tailList;
    }

    // Возвращает позицию найденного элемента
    long Find(const X& elem) {
        long index = 0;
        Node* temp = headList;
        while (temp != nullptr) {
            if (temp->data == elem) {
                return index;
            }
            temp = temp->next;
            index++;
        }
        return -1;
    }

    // Метод обращения к текущему элементу
    X& getElement() {
        if (currentList == nullptr) {
            throw std::out_of_range("Current element is nullptr");
        }
        return currentList->data;
    }

    // Метод проверки списка на пустоту
    bool isEmpty() {
        return headList == nullptr;
    }

    // Метод очищения списка
    void Clear() {
        while (!isEmpty()) {
            DelHead();
        }
    }
};

// Функция для удаления повторяющихся дробей из списка
template<typename X>
void RemoveDuplicates(List<X>& myList) {
    List<X> uniqueList; // Создаем новый список для хранения уникальных элементов
    
    // Проходим по исходному списку
    myList.toBegin();
    while (myList.next()) {
        X currentElem = myList.getElement();
        // Если текущий элемент уже есть в уникальном списке, пропускаем его
        if (uniqueList.Find(currentElem) == -1) {
            uniqueList.AddToLast(currentElem);
        }
    }
    
    // Заменяем исходный список уникальным
    myList = uniqueList;
}

int main() {
    // Тестирование методов класса List
    List<int> myList;
    
    // Добавление элементов
    myList.AddToHead(1);
    myList.AddToHead(2);
    myList.AddToLast(3);
    myList.AddToCurrent(4);
    myList.AddToLast(5);
    
    // Перемещение текущего элемента и получение его значения
    myList.toBegin();
    std::cout << "Current element: " << myList.getElement() << std::endl;
    myList.next();
    std::cout << "Current element: " << myList.getElement() << std::endl;
    
    // Удаление элементов
    myList.Del(4);
    myList.toEnd();
    myList.DelLast();
    
    // Проверка на пустоту
    if (!myList.isEmpty()) {
        // Вывод всех элементов
        myList.toBegin();
        while (myList.next()) {
            std::cout << myList.getElement() << " ";
        }
        std::cout << myList.getElement() << std::endl;
    }
    
    // Тестирование функции RemoveDuplicates для удаления повторяющихся элементов из списка дробей
    List<double> fractionList;
    fractionList.AddToLast(1.5);
    fractionList.AddToLast(2.0);
    fractionList.AddToLast(1.5);
    fractionList.AddToLast(3.5);
    fractionList.AddToLast(2.0);
    
    RemoveDuplicates(fractionList);
    
    fractionList.toBegin();
    while (fractionList.next()) {
        std::cout << fractionList.getElement() << " ";
    }
    std::cout << fractionList.getElement() << std::endl;
    
    return 0;
}
