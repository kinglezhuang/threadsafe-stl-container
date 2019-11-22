//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <set>
#include <mutex>
#include <memory>
#include <utility>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <
              typename _Key,
              typename _Compare = less<_Key>,
              typename _Allocator = allocator<_Key>
             >
    class threadsafe_set
    {
    public:
        typedef _Key                                     key_type;
        typedef key_type                                 value_type;
        typedef _Compare                                 key_compare;
        typedef key_compare                              value_compare;
        typedef _Allocator                               allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
        
    private:
        typedef std::set<value_type, value_compare, allocator_type> __set_type;
        
        std::shared_timed_mutex __mutex_;
        __set_type __internal_set_;
        
    public:
        typedef typename __set_type::pointer                pointer;
        typedef typename __set_type::const_pointer          const_pointer;
        typedef typename __set_type::size_type              size_type;
        typedef typename __set_type::difference_type        difference_type;
        typedef typename __set_type::iterator               iterator;
        typedef typename __set_type::const_iterator         const_iterator;
        typedef typename __set_type::reverse_iterator       reverse_iterator;
        typedef typename __set_type::const_reverse_iterator const_reverse_iterator;
        
    public:
        threadsafe_set() : __internal_set_() {}
        threadsafe_set(const __set_type& __s) : __internal_set_(__s) {}
        threadsafe_set(__set_type&& __s) : __internal_set_(std::move(__s)) {}
        threadsafe_set(initializer_list<value_type> __il) : __internal_set_(__il) {}
        
        template <class _InputIterator>
        threadsafe_set(_InputIterator __f, _InputIterator __l) : __internal_set_(__f, __l) {}
        
        threadsafe_set(const threadsafe_set&) = delete;
        threadsafe_set& operator=(const threadsafe_set&) = delete;
        threadsafe_set(threadsafe_set&&) = delete;
        threadsafe_set& operator=(threadsafe_set&&) = delete;
        
    public:
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.size();
        }
        
        bool insert(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.insert(__v).second;
        }
        
        void insert(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_set_.insert(__il);
        }
        
        const std::pair<const value_type, bool> get(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_set_.find(__k);
            if (it == __internal_set_.end())
            {
                return std::make_pair(value_type(), false);
            }
            else
            {
                return std::make_pair(*it, true);
            }
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_set_.clear();
        }
        
        bool contains(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_set_.find(__k);
            return it != __internal_set_.end();
        }
        
        size_type erase(const key_type& __k)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.erase(__k);
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_set_)
            {
                __bl(v);
            }
        }
    };
    
    
    template <
              typename _Key,
              typename _Compare = less<_Key>,
              typename _Allocator = allocator<_Key>
             >
    class threadsafe_multiset
    {
    public:
        typedef _Key                                     key_type;
        typedef key_type                                 value_type;
        typedef _Compare                                 key_compare;
        typedef key_compare                              value_compare;
        typedef _Allocator                               allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
        
    private:
        typedef std::multiset<value_type, value_compare, allocator_type> __set_type;
        
        std::shared_timed_mutex __mutex_;
        __set_type __internal_set_;
        
    public:
        typedef typename __set_type::pointer                pointer;
        typedef typename __set_type::const_pointer          const_pointer;
        typedef typename __set_type::size_type              size_type;
        typedef typename __set_type::difference_type        difference_type;
        typedef typename __set_type::iterator               iterator;
        typedef typename __set_type::const_iterator         const_iterator;
        typedef typename __set_type::reverse_iterator       reverse_iterator;
        typedef typename __set_type::const_reverse_iterator const_reverse_iterator;
        
    public:
        threadsafe_multiset() : __internal_set_() {}
        threadsafe_multiset(const __set_type& __s) : __internal_set_(__s) {}
        threadsafe_multiset(__set_type&& __s) : __internal_set_(std::move(__s)) {}
        threadsafe_multiset(initializer_list<value_type> __il) : __internal_set_(__il) {}
        
        template <class _InputIterator>
        threadsafe_multiset(_InputIterator __f, _InputIterator __l) : __internal_set_(__f, __l) {}
        
        threadsafe_multiset(const threadsafe_multiset&) = delete;
        threadsafe_multiset& operator=(const threadsafe_multiset&) = delete;
        threadsafe_multiset(threadsafe_multiset&&) = delete;
        threadsafe_multiset& operator=(threadsafe_multiset&&) = delete;
        
    public:
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.size();
        }
        
        void insert(const value_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_set_.insert(__v);
        }
        
        void insert(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_set_.insert(__il);
        }
        
        const std::pair<const value_type, bool> get(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_set_.find(__k);
            if (it == __internal_set_.end())
            {
                return std::make_pair(value_type(), false);
            }
            else
            {
                return std::make_pair(*it, true);
            }
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_set_.clear();
        }
        
        bool contains(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_set_.find(__k);
            return it != __internal_set_.end();
        }
        
        size_type erase(const key_type& __k)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_set_.erase(__k);
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_set_)
            {
                __bl(v);
            }
        }
        
        void for_each(const key_type& __k, std::function<void(const std::pair<iterator, iterator>&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            std::pair<iterator, iterator> r = __internal_set_.equal_range(__k);
            __bl(r);
        }
    };
}
