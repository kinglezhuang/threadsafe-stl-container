//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 RingCentral. All rights reserved.
//

#pragma once

#include <list>
#include <mutex>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <typename _Tp, typename _Allocator = allocator<_Tp>>
    class threadsafe_list
    {
    private:
        typedef std::list<_Tp, _Allocator> __list_type;
        
        mutable std::shared_timed_mutex __mutex_;
        __list_type __internal_list_;
        
    public:
        typedef _Tp                                             value_type;
        typedef _Allocator                                      allocator_type;
        
        typedef          __list_type                            list_type;
        typedef typename __list_type::reference                 reference;
        typedef typename __list_type::const_reference           const_reference;
        typedef typename __list_type::size_type                 size_type;
        typedef typename __list_type::difference_type           difference_type;
        typedef typename __list_type::pointer                   pointer;
        typedef typename __list_type::const_pointer             const_pointer;
        typedef typename __list_type::iterator                  iterator;
        typedef typename __list_type::const_iterator            const_iterator;
        typedef typename __list_type::reverse_iterator          reverse_iterator;
        typedef typename __list_type::const_reverse_iterator    const_reverse_iterator;
        
    public:
        threadsafe_list() : __internal_list_() {}
        explicit threadsafe_list(size_type __n) : __internal_list_(__n) {}
        threadsafe_list(size_type __n, const value_type& __v) : __internal_list_(__n, __v) {}
        threadsafe_list(const list_type& __l) : __internal_list_(__l) {}
        threadsafe_list(list_type&& __l) : __internal_list_(std::move(__l)) {}
        threadsafe_list(initializer_list<value_type> __il) : __internal_list_(__il) {}
        
        template <class _InputIterator>
        threadsafe_list(_InputIterator __f, _InputIterator __l) : __internal_list_(__f, __l) {}
        
        threadsafe_list(const threadsafe_list&) = delete;
        threadsafe_list& operator=(const threadsafe_list&) = delete;
        threadsafe_list(threadsafe_list&&) = delete;
        threadsafe_list& operator=(threadsafe_list&&) = delete;
        
    public:
        template <class _InputIterator>
        void assign(_InputIterator __f, _InputIterator __l)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.assign(__f, __l);
        }
        
        void assign(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.assign(__n, __v);
        }
        
        void assign(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.assign(__il);
        }
        
        bool empty() const
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_list_.empty();
        }
        
        size_type size() const
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_list_.size();
        }
        
        size_type max_size() const
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_list_.max_size();
        }
        
        void resize(size_type __n)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.resize(__n);
        }
        
        void resize(size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.resize(__n, __v);
        }
        
        void operator=(const list_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_ = __v;
        }
        
        void operator=(initializer_list<list_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_ = __il;
        }
        
        list_type value()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_list_;
        }

        const value_type& front()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_list_.front());
        }
        
        const value_type& back()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_list_.back());
        }
        
        void push_front(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.push_front(__v);
        }
        
        void push_back(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.push_back(__v);
        }
        
        void pop_front()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.pop_front();
        }
        
        void pop_back()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.pop_back();
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.clear();
        }
        
        void remove(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.remove(__v);
        }
        
        template <class Pred>
        void remove_if(Pred __pred)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.remove_if(__pred);
        }
        
        void erase(std::function<bool(const value_type&)> __comp)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const_iterator it = __internal_list_.begin(); it != __internal_list_.end();)
            {
                if (__comp(*it))
                {
                    it = __internal_list_.erase(it);
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
            const_iterator it = std::find_if(__internal_list_.begin(), __internal_list_.end(), __pred);
            if (it != __internal_list_.end())
            {
                __internal_list_.erase(it);
                return std::make_pair(*it, true);
            }
            
            return std::make_pair(value_type(), false);
        }
        
        void insert(std::function<const_iterator(const list_type&)> __pos, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            
            const_iterator pos = __pos(__internal_list_);
            
            __internal_list_.insert(pos, __v);
        }
        
        void insert(std::function<const_iterator(const list_type&)> __pos, size_type __n, const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            
            const_iterator pos = __pos(__internal_list_);
            
            __internal_list_.insert(pos, __n, __v);
        }
        
        template <class _InputIterator>
        void insert(std::function<const_iterator(const list_type&)> __pos, _InputIterator __f, _InputIterator __l)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            
            const_iterator pos = __pos(__internal_list_);
            
            __internal_list_.insert(pos, __f, __l);
        }
        
        void insert(std::function<const_iterator(const list_type&)> __pos, initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            
            const_iterator pos = __pos(__internal_list_);
            
            __internal_list_.insert(pos, __il);
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_list_)
            {
                __bl(v);
            }
        }
        
        template <typename _Compare>
        void sort(_Compare __comp)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_list_.sort(__comp);
        }
    };
}
