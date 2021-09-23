/* 
 * File:   List.h
 * Author: Bob
 *
 * Created on October 3, 2019, 1:21 PM
 */

#ifndef GSLIST_H
#define	GSLIST_H

#include "GSBCommon.h"
#include <cstdlib>

template <class T>
class list 
{
private:
    struct Node
    {
        Node(const T &item) : prev(null), next(null), elem(item)
        {           
        }
        Node *prev, *next;
        T elem;
    };
        
public:
    list() : _head(null), _tail(null), _size(0)
    {
    }
    ~list()
    {
        clear();
    }
    
    void clear()
    {
        while (_head)
        {
            Node *next = _head->next;
            delete _head;
            _head = next;
        }
        _head = _tail = null;
        _size = 0;
    }
    
    void push_front(const T &item)
    {
        Node *n = new Node(item);
        if (_head == null)
            _head = _tail = n;
        else
        {
            _head->prev = n;
            n->next = _head;
            _head = n;
        }
        ++_size;
    }
    
    void push_back(const T &item)
    {
        Node *n = new Node(item);
        if (_head == null)
            _head = _tail = n;
        else
        {
            _tail->next = n;
            n->prev = _tail;
            _tail = n;
        }
        ++_size;
    }
    
    class iterator
    {
    public:
        iterator() : _i(null) {}
        iterator(Node *i) : _i(i) {}
        iterator(const iterator &li) : _i(li._i) {}
        
        bool operator==(const iterator &b) {return _i == b._i;}
        bool operator!=(const iterator &b) {return _i != b._i;}
        iterator &operator++() 
        {
            if (_i)
                _i = _i->next;
            return *this;
        }
        iterator operator++(int) 
        {
            iterator rtn = *this;
            if (_i)
                _i = _i->next;
            return rtn;
        }
        iterator &operator--() 
        {
            if (_i)
                _i = _i->prev;
            return *this;
        }
        iterator operator--(int) 
        {
            iterator rtn = *this;
            if (_i)
                _i = _i->prev;
            return rtn;
        }
        T *operator->() {return &_i->elem;}
        T &operator*() {return _i->elem;}
        
    private:
        friend class list<T>;
        Node *_i;
    };
    
    
    iterator begin()
    {
        return iterator(_head);
    }
    iterator end()
    {
        return iterator(null);
    }
    
    T &back()
    {
        return _tail->elem;
    }

    const T &back() const
    {
        return _tail->elem;
    }
    
    iterator erase(iterator i)
    {
        if (i._i->prev)
            i._i->prev->next = i._i->next;
        else
            _head = i._i->next;
        if (i._i->next)
            i._i->next->prev = i._i->prev;
        else
            _tail = i._i->prev;
        iterator rtn(i._i->next);
        delete i._i;
        --_size;
        return rtn;
    }
    
    iterator insert(iterator i, const T &item)
    {
        if (i == begin())
        {
            push_front(item);
            return iterator(_head);
        }
        else if (i == end())
        {
            push_back(item);
            return iterator(_tail);
        }
        else
        {
            Node *n = new Node(item);
            iterator prev = i;
            --prev;
            n->next = i._i;
            n->prev = prev._i;
            i._i->prev = n;
            prev._i->next = n; 
            ++_size;
            return iterator(n);
        }
    }
    
    size_t size() const {return _size;}

private:
    Node *_head, *_tail;
    size_t _size;
};

#endif	/* GSLIST_H */

