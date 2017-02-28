#include <cstddef>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <cstdlib>

#include "Vector.h"
#include "LinkedList.h"


namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest()
{
  LinearCollection<std::string> collection;
  collection.append("TODO");
}

} // namespace


int main()
{
    aisdi::LinkedList<int> lista;
    aisdi::Vector<int> wektor;
    int x=200000;
    int tab[200000];

    for (int i=0; i<x; i++)
        tab[i]=rand()%100;
    clock_t czas = clock();

    for (int i=0; i<x; i++)
        lista.prepend(tab[i]);
    std::cout << "Operacja prepend() w kolekcji LinkedList wykonuje sie w czasie:" << clock()-czas << std::endl;
    czas=clock();

    for (int i=0; i<x; i++)
        wektor.prepend(tab[i]);
    std::cout << "Operacja prepend() w kolekcji Vector wykonuje sie w czasie:" << clock()-czas << std::endl;
    czas=clock();

    for (int i=0; i<x; i++)
        lista.popLast();
    std::cout << "Operacja popLast() w kolekcji LinkedList wykonuje sie w czasie:" << clock()-czas << std::endl;
    czas=clock();

    for (int i=0; i<x; i++)
        wektor.popLast();
    std::cout << "Operacja popLast() w kolekcji Vector wykonuje sie w czasie:" << clock()-czas << std::endl;


  return 0;
}
