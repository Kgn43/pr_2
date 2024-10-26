#ifndef ARR_H
#define ARR_H

#include <iostream>
#include <sstream>
using namespace std;

template <typename T>
struct arr {
    T* data;      // Указатель на динамический массив
    size_t size;  // Текущий размер вектора
    size_t capacity; // Вместимость вектора

    //конструктор
    arr<T>() : data(nullptr), size(0), capacity(0) {}

    // Конструктор с заданной длиной
    arr<T>(size_t length) : size(length), capacity(length) {
        data = new T[capacity];  // Выделение памяти под массив
        // Инициализация массива значениями по умолчанию
        for (size_t i = 0; i < size; ++i) {
            data[i] = T(); //явное значение, если нужно, можно допилить
        }
    }

    void realoc();
    void push_back(const T& value);
    size_t getSize() const;
    int find(T input) const;
    void erase();
    void print();
    void del(size_t index);
    void clear();
    void sort();
    void resize(size_t newSize);


    // Перегрузка оператора вывода
    friend ostream& operator<<(ostream& os, const arr& arr) {
        os << "[";
        for (size_t i = 0; i < arr.size; ++i) {
            os << arr.data[i];
            if (i < arr.size - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    // Метод для получения элемента по индексу
    T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }


    bool operator==(const arr<T>& other) const {
        if (size != other.size) return false; // Сначала сравнить размеры
        for (size_t i = 0; i < size; ++i) {
            if (!(data[i] == other.data[i])) {
                return false; // Используем оператор == для сравнения элементов
            }
        }
        return true; // Все элементы равны
    }


    bool operator>(const arr<T>& other) const {
        for (size_t i = 0; i < this->size; ++i){

            if (this->data[i] > other[i]) { }
            else return false;
        }
        return true; // Все элементы равны
    }

};


template struct arr<int>;
template struct arr<arr<int>>;
template struct arr<arr<arr<int>>>;
template struct arr<string>;
template struct arr<arr<string>>;
template struct arr<arr<arr<string>>>;


arr<string> splitToArr(const string &input, char delimiter = ' ');
arr<string> splitToArr(const string& input, const string& delimiter);
string unsplit(const arr<string>& array, char delimiter = ' ');

#endif // ARR_H
