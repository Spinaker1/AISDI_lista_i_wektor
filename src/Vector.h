#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
private:
    Type * array;
    size_t size;
    const size_t n=10;
    size_t x;

public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  Vector()
  {
      size=0;
      x=n;
      array=new Type[n];
  }

  Vector(std::initializer_list<Type> l)
  {
      size=0;
      x=l.size()+n;
      array = new Type[x];
    for (auto iter=l.begin();iter!=l.end();iter++)
        append(*iter);
  }

  Vector(const Vector& other)
  {
    size=0;
    x=other.x;
    array = new Type[x];
    for (auto iter=other.begin();iter!=other.end();iter++)
        append(*iter);
  }

  Vector(Vector&& other)
  {
    array = other.array;
    size = other.size;
    x = other.x;
    other.array=new Type[n];
    other.size=0;
    other.x=n;
  }

  ~Vector()
  {
    delete[] array;
  }

  Vector& operator=(const Vector& other)
  {
    delete[] array;
    size=0;
    x=other.x;
    array = new Type[x];
    for (auto iter=other.begin();iter!=other.end();iter++)
        append(*iter);

    return (*this);
  }

  Vector& operator=(Vector&& other)
  {
    array = other.array;
    size = other.size;
    x=other.x;
    other.array=new Type[n];
    other.size=0;
    other.x=n;
    return (*this);
  }

  bool isEmpty() const
  {
    if (size==0)
        return true;
    return false;
  }

  size_type getSize() const
  {
    return size;
  }

  void append(const Type& item)
  {
      insert(end(),item);
  }

  void prepend(const Type& item)
  {
    insert(begin(),item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      if (size==x)
      {
            x=size+n;
          Type * new_array=new Type[x];
          for (unsigned int i=0;i<size;i++)
                new_array[i]=array[i];
            delete array;
          array = new_array;
      }
        if (!(isEmpty() || insertPosition==end()))
        {
            size++;
            unsigned int i=size-2;
            while(true)
            {
                array[i+1]=array[i];
                if (i==insertPosition.index)
                    break;
                i--;
            }
        }
        else
            size++;

        array[insertPosition.index]=item;
  }

  Type popFirst()
  {
      Type to_return=array[0];
      erase(begin());
      return to_return;
  }

  Type popLast()
  {
      Type to_return=array[size-1];
      erase(end()-1);
      return to_return;
  }

  void erase(const const_iterator& position)
  {
    if (isEmpty() || position==end())
        throw std::out_of_range("");
    if (position!=end()-1)
    {
        size--;
        for (unsigned int i=position.index;i!=size;i++)
            array[i]=array[i+1];
    }
    else
        size--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if (isEmpty())
        throw std::out_of_range("");
    int h=lastExcluded.index-firstIncluded.index;
    for (unsigned int i=firstIncluded.index;i!=size;i++)
            array[i]=array[i+h];
    size-=h;
  }

  iterator begin()
  {
    ConstIterator iter=cbegin();
    return (Iterator)iter;
  }

  iterator end()
  {
    ConstIterator iter=cend();
    return (Iterator)iter;
  }

  const_iterator cbegin() const
  {
    ConstIterator iter;
    iter.index=0;
    iter.vect=this;
    return iter;
  }

  const_iterator cend() const
  {
    ConstIterator iter;
    iter.index=size;
    iter.vect=this;
    return iter;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
protected:
    unsigned int index;
    const Vector * vect;

public:
    friend class Vector;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  explicit ConstIterator()
  {}

  reference operator*() const
  {
    if (vect->size<=index)
        throw std::out_of_range("");
    return vect->array[index];
  }

  ConstIterator& operator++()
  {
    if (vect->size<=index)
        throw std::out_of_range("");
    index++;
    return (*this);
  }

  ConstIterator operator++(int)
  {
    if (vect->size<=index)
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    ++(*this);
    return orig;
  }

  ConstIterator& operator--()
  {
    if (index==0)
        throw std::out_of_range("");
    index--;
    return (*this);
  }

  ConstIterator operator--(int)
  {
    if (index==0)
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    --(*this);
    return orig;
  }

  ConstIterator operator+(difference_type d) const
  {
    auto iter = *this;
    for (int i=1;i<=d;i++)
        ++iter;
    return iter;
  }

  ConstIterator operator-(difference_type d) const
  {
    auto iter = *this;
    for (int i=1;i<=d;i++)
        --iter;
    return iter;
  }

  bool operator==(const ConstIterator& other) const
  {
    if ((this->vect == other.vect) && (this->index == other.index))
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if ((this->vect == other.vect) && (this->index == other.index))
        return false;
    return true;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
