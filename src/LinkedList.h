#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
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

private:

    struct Item
    {
        Type value;
        Item * prev;
        Item * next;
    };

    Item * first;
    Item * last;

    void createSentinels()
    {
        first = new Item;
        last = new Item;
        first->prev=nullptr;
        first->next=last;
        last->prev=first;
        last->next=nullptr;
    }


public:

  LinkedList()
  {
    createSentinels();
  }

  LinkedList(std::initializer_list<Type> l)
  {
    createSentinels();
    for (auto iter=l.begin();iter!=l.end();iter++)
        append(*iter);
  }

  LinkedList(const LinkedList& other)
  {
    createSentinels();
    for (auto iter=other.begin();iter!=other.end();iter++)
        append(*iter);
  }

  LinkedList(LinkedList&& other)
  {
    first=other.first;
    last=other.last;
    other.first=nullptr;
    other.last=nullptr;
    other.createSentinels();
  }

  ~LinkedList()
  {
    if(!(isEmpty()))
        erase(begin(),end());
    delete first;
    delete last;
    first->next=nullptr;
    last->prev=nullptr;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(!(isEmpty()))
        erase(begin(),end());
    for (auto iter=other.begin();iter!=other.end();iter++)
        append(*iter);
    return(*this);
  }

  LinkedList& operator=(LinkedList&& other)
  {
    first=other.first;
    last=other.last;
    other.first=nullptr;
    other.last=nullptr;
    other.createSentinels();
    return (*this);
  }

  bool isEmpty() const
  {
      if (first->next==last)
        return true;
      return false;
  }

  size_type getSize() const
  {
    size_type i=0;
    for (auto iter=this->begin();iter!=this->end();iter++)
        i++;
    return i;
  }

  void append(const Type& val)
  {
    insert(end(),val);
  }

  void prepend(const Type& val)
  {
    insert(begin(),val);
  }

  void insert(const const_iterator& insertPosition, const Type& val)
  {
    Item * new_item = new Item;
    Item * next_item = insertPosition.ptr;
    Item * prev_item = next_item->prev;
    new_item->value=val;
    prev_item->next=new_item;
    new_item->prev=prev_item;
    next_item->prev=new_item;
    new_item->next=next_item;
  }

  Type popFirst()
  {
    Type to_return=begin().ptr->value;
    erase(begin());
    return to_return;
  }

  Type popLast()
  {
    Type to_return=(end()-1).ptr->value;
    erase(end()-1);
    return to_return;
  }

  void erase(const const_iterator& position)
  {
    if(isEmpty() || position==end())
        throw std::out_of_range("");
    Item * to_delete=position.ptr;
    Item * next_item=to_delete->next;
    Item * prev_item=to_delete->prev;
    to_delete->next=nullptr;
    to_delete->prev=nullptr;
    delete to_delete;
    next_item->prev=prev_item;
    prev_item->next=next_item;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(isEmpty() || firstIncluded==end())
        throw std::out_of_range("");
    Item * actual = firstIncluded.ptr;
    Item * previous = actual->prev;
    Item * next;
    while (actual!=lastExcluded.ptr)
    {
        next=actual->next;
        delete actual;
        actual->next=nullptr;
        actual->prev=nullptr;
        actual=next;
    }
    previous->next=lastExcluded.ptr;
    lastExcluded.ptr->prev=previous;
  }

  iterator begin()
  {
    Iterator iter;
    iter.ptr=first->next;
    return iter;
  }

  iterator end()
  {
    Iterator iter;
    iter.ptr=last;
    return iter;
  }

  const_iterator cbegin() const
  {
    ConstIterator iter;
    iter.ptr=first->next;
    return iter;
  }

  const_iterator cend() const
  {
    ConstIterator iter;
    iter.ptr=last;
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
class LinkedList<Type>::ConstIterator
{
protected:
Item * ptr;

public:
    friend class LinkedList;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  explicit ConstIterator()
  {
    ptr=nullptr;
  }

  reference operator*() const
  {
    if (ptr->next==nullptr)
        throw std::out_of_range("");
    return ptr->value;
  }

  ConstIterator& operator++()
  {
     if (ptr->next==nullptr)
        throw std::out_of_range("");
    ptr=ptr->next;

    return *this;
  }

  ConstIterator operator++(int)
  {
     if (ptr->next==nullptr)
        throw std::out_of_range("");
    ConstIterator orig=(*this);
    ++(*this);
    return orig;
  }

  ConstIterator& operator--()
  {
    if (ptr->prev->prev==nullptr)
        throw std::out_of_range("");
    ptr=ptr->prev;
    return (*this);
  }

  ConstIterator operator--(int)
  {
    if (ptr->prev->prev==nullptr)
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
    if (this->ptr==other.ptr)
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if (this->ptr==other.ptr)
        return false;
    return true;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

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

#endif // AISDI_LINEAR_LINKEDLIST_H
