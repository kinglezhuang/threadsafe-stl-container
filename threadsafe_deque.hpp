//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <mutex>
#include <deque>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <typename _Tp, typename _Allocator = allocator<_Tp>>
    class threadsafe_deque
    {
    private:
        typedef std::deque<_Tp, _Allocator> __deque_type;
        
        std::shared_timed_mutex __mutex_;
        __deque_type __internal_queue_;
        
    public:
        typedef _Tp                                             value_type;
        typedef _Allocator                                      allocator_type;
        
        typedef typename __deque_type::reference                reference;
        typedef typename __deque_type::const_reference          const_reference;
        typedef typename __deque_type::size_type                size_type;
        typedef typename __deque_type::difference_type          difference_type;
        typedef typename __deque_type::pointer                  pointer;
        typedef typename __deque_type::const_pointer            const_pointer;
        typedef typename __deque_type::iterator                 iterator;
        typedef typename __deque_type::const_iterator           const_iterator;
        typedef typename __deque_type::reverse_iterator         reverse_iterator;
        typedef typename __deque_type::reverse_iterator         const_reverse_iterator;
        
    public:
        threadsafe_deque() : __internal_queue_() {}
        explicit threadsafe_deque(size_type __n) : __internal_queue_(__n) {}
        threadsafe_deque(size_type __n, const value_type& __v) : __internal_queue_(__n, __v) {}
        threadsafe_deque(const __deque_type& __l) : __internal_queue_(__l) {}
        threadsafe_deque(__deque_type&& __l) : __internal_queue_(std::move(__l)) {}
        threadsafe_deque(initializer_list<value_type> __il) : __internal_queue_(__il) {}
        
        template <class _InputIterator>
        threadsafe_deque(_InputIterator __f, _InputIterator __l) : __internal_queue_(__f, __l) {}
        
        threadsafe_deque(const threadsafe_deque&) = delete;
        threadsafe_deque& operator=(const threadsafe_deque&) = delete;
        threadsafe_deque(threadsafe_deque&&) = delete;
        threadsafe_deque& operator=(threadsafe_deque&&) = delete;
        
    public:
        template <class _InputIterator>
        void assign(_InputIterator __f, _InputIterator __l)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.assign(__f, __l);
        }
        
        void assign(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.assign(__n, __v);
        }
        
        void assign(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.assign(__il);
        }
        
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_queue_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_queue_.size();
        }
        
        void resize(size_type __n)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.resize(__n);
        }
        
        void resize(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.resize(__n, __v);
        }
        
        void shrink_to_fit()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.shrink_to_fit();
        }
        
        const value_type& front()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_queue_.front());
        }
        
        const value_type& back()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_queue_.back());
        }
        
        void push_front(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.push_front(__v);
        }
        
        void push_back(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.push_back(__v);
        }
        
        void pop_front()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.pop_front();
        }
        
        void pop_back()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.pop_back();
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_.clear();
        }
        
        const value_type& operator[](size_type __n)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_queue_[__n];
        }
        
        const value_type& at(size_type __n)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_queue_.at(__n);
        }
        
        void set(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_queue_[__n] = __v;
        }
        
        void erase(std::function<bool(const value_type&)> __comp)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const_iterator it = __internal_queue_.begin(); it != __internal_queue_.end();)
            {
                if (__comp(*it))
                {
                    it = __internal_queue_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        
        template <typename _Predicate>
        std::pair<const value_type, bool> find_and_erase(_Predicate __pred)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            const_iterator it = std::find_if(__internal_queue_.begin(), __internal_queue_.end(), __pred);
            if (it != __internal_queue_.end())
            {
                value_type r = *it;
                __internal_queue_.erase(it);
                return std::make_pair(r, true);
            }
            
            return std::make_pair(value_type(), false);
        }
        
        template <typename _Predicate>
        bool insert(_Predicate __pred, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            const_iterator it = std::find_if(__internal_queue_.begin(), __internal_queue_.end(), __pred);
            if (it != __internal_queue_.end())
            {
                __internal_queue_.insert(it, __v);
                return true;
            }
            
            return false;
        }
        
        template <typename _Predicate>
        bool insert(_Predicate __pred, size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            const_iterator it = std::find_if(__internal_queue_.begin(), __internal_queue_.end(), __pred);
            if (it != __internal_queue_.end())
            {
                __internal_queue_.insert(it, __n, __v);
                return true;
            }
            
            return false;
        }
        
        template <typename _Predicate, typename _InputIterator>
        bool insert(_Predicate __pred, _InputIterator __f, _InputIterator __l)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            const_iterator it = std::find_if(__internal_queue_.begin(), __internal_queue_.end(), __pred);
            if (it != __internal_queue_.end())
            {
                __internal_queue_.insert(it, __f, __l);
                return true;
            }
            
            return false;
        }
        
        template <typename _Predicate>
        bool insert(_Predicate __pred, initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            const_iterator it = std::find_if(__internal_queue_.begin(), __internal_queue_.end(), __pred);
            if (it != __internal_queue_.end())
            {
                __internal_queue_.insert(it, __il);
                return true;
            }
            
            return false;
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_queue_)
            {
                __bl(v);
            }
        }
    };
}