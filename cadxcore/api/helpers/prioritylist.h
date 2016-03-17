/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#pragma once
#ifndef PRIORITYLIST_H
#define PRIORITYLIST_H

#include <list>
#include <iterator>

namespace GNC
{
namespace GCS
{

//region "Helpers"
// <editor-fold defaultstate="collapsed" desc="lista prioritaria">
template<class T>
class priority_list
{
        std::list<T> lst;
public:
        class iterator;

        friend class iterator;

        class iterator : public std::iterator< std::bidirectional_iterator_tag,T,ptrdiff_t>
        {
                typename std::list<T>* r;
                typename std::list<T>::iterator it;
        public:
                iterator(priority_list<T>& otro)
                {
                        *this = otro;
                }
                iterator(std::list<T>& lst, const typename std::list<T>::iterator& i) : r(&lst), it(i) {}

                bool operator==(const iterator& x) const
                {
                        return it == x.it;
                }
                bool operator!=(const iterator& x) const
                {
                        return !(*this == x);
                }
                typename std::list<T>::reference operator*() const
                {
                        return *it;
                }
                iterator& operator=(const iterator& otro)
                {
                        r = otro.r;
                        it = otro.it;
                        return *this;
                }
                iterator& operator++()
                {
                        ++it;
                        return *this;
                }
                iterator operator++(int)
                {
                        iterator tmp = *this;
                        ++*this;
                        return tmp;
                }
                iterator& operator--()
                {
                        --it;
                        return *this;
                }
                iterator operator--(int)
                {
                        iterator tmp = *this;
                        --*this;
                        return tmp;
                }
                iterator insert(const T& x)
                {
                        return iterator(*r, r->insert(it));
                }
                iterator erase()
                {
                        return iterator(*r, r->erase(it));
                }
        };

        void insertar(const T& x)
        {
                bool stop = false;
                for (typename std::list<T>::iterator it1 = lst.begin(); !stop && it1 != lst.end(); it1++) {
                        if ((*it1) > x) {
                                lst.insert(it1, x);
                                stop = true;
                        }
                }
                if (!stop) {
                        lst.push_back(x);
                }
        }
        void eliminar(const T& x)
        {
                bool stop = false;
                for (typename std::list<T>::iterator it1 = lst.begin(); !stop && it1 != lst.end(); it1++) {
                        if ((*it1) == x) {
                                lst.erase(it1);
                                stop = true;
                        }
                }
        }
        iterator begin()
        {
                return iterator(lst, lst.begin());
        }
        iterator end()
        {
                return iterator(lst, lst.end());
        }
        int size()
        {
                return lst.size();
        }
};
// </editor-fold>
//endregion
}
}
#endif
