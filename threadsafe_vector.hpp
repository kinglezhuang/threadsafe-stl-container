//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <typename _Tp, typename _Allocator = allocator<_Tp>>
    class threadsafe_vector
    {
    private:
        typedef std::vector<_Tp, _Allocator> __vector_type;
        
        std::shared_timed_mutex __mutex_;
        __vector_type __internal_vector_;
        
    public:
        typedef _Tp                                             value_type;
        typedef _Allocator                                      allocator_type;

        typedef typename __vector_type::reference               reference;
        typedef typename __vector_type::const_reference         const_reference;
        typedef typename __vector_type::size_type               size_type;
        typedef typename __vector_type::difference_type         difference_type;
        typedef typename __vector_type::pointer                 pointer;
        typedef typename __vector_type::const_pointer           const_pointer;
        typedef typename __vector_type::iterator                iterator;
        typedef typename __vector_type::const_iterator          const_iterator;
        typedef typename __vector_type::reverse_iterator        reverse_iterator;
        typedef typename __vector_type::reverse_iterator        const_reverse_iterator;
        
    public:
        threadsafe_vector() : __internal_vector_() {}
        explicit threadsafe_vector(size_type __n) : __internal_vector_(__n) {}
        threadsafe_vector(size_type __n, const value_type& __v) : __internal_vector_(__n, __v) {}
        threadsafe_vector(const __vector_type& __v) : __internal_vector_(__v) {}
        threadsafe_vector(__vector_type&& __v) : __internal_vector_(std::move(__v)) {}
        threadsafe_vector(initializer_list<value_type> __il) : __internal_vector_(__il) {}
        
        template <class _InputIterator>
        threadsafe_vector(_InputIterator __f, _InputIterator __l) : __internal_vector_(__f, __l) {}
        
        threadsafe_vector(const threadsafe_vector&) = delete;
        threadsafe_vector& operator=(const threadsafe_vector&) = delete;
        threadsafe_vector(threadsafe_vector&&) = delete;
        threadsafe_vector& operator=(threadsafe_vector&&) = delete;
        
    public:
        template <class _InputIterator>
        void assign(_InputIterator __f, _InputIterator __l)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.assign(__f, __l);
        }
        
        void assign(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.assign(__n, __v);
        }
        
        void assign(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.assign(__il);
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_.size();
        }

        size_type capacity()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_.capacity();
        }
        
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_.empty();
        }
        
        void reserve(size_type __n)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.reserve(__n);
        }
        
        void shrink_to_fit()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.shrink_to_fit();
        }
        
        void resize(size_type __n)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.resize(__n);
        }
        
        void resize(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.resize(__n, __v);
        }
        
        const value_type& front()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_vector_.front());
        }

        const value_type& back()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_vector_.back());
        }

        const value_type* data()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_.data();
        }
        
        void push_back(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.push_back(__v);
        }
        
        void pop_back()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.pop_back();
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_.clear();
        }
        
        const value_type& operator[](size_type __n)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_[__n];
        }
        
        const value_type& at(size_type __n)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_vector_.at(__n);
        }
        
        void set(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_vector_[__n] = __v;
        }
        
        void erase(std::function<bool(const value_type&)> __comp)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const_iterator it = __internal_vector_.begin(); it != __internal_vector_.end();)
            {
                if (__comp(*it))
                {
                    it = __internal_vector_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_vector_)
            {
                __bl(v);
            }
        }
        
        void for_each(size_type __f, size_type __l, std::function<void(size_type, const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (size_type i = __f; i < __l; ++i)
            {
                __bl(i, __internal_vector_[i]);
            }
        }
        
        template <typename _Compare>
        void sort(_Compare __comp)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            std::sort(__internal_vector_.begin(), __internal_vector_.end(), __comp);
        }
    };
}
