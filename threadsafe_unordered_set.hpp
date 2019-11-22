//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <mutex>
#include <memory>
#include <utility>
#include <functional>
#include <shared_mutex>
#include <unordered_set>

namespace std
{
    template <
              typename _Value,
              typename _Hash = hash<_Value>,
              typename _Pred = equal_to<_Value>,
              typename _Alloc = allocator<_Value>
             >
    class threadsafe_unordered_set
    {
    public:
        typedef _Value                                      key_type;
        typedef key_type                                    value_type;
        typedef _Hash                                       hasher;
        typedef _Pred                                       key_equal;
        typedef _Alloc                                      allocator_type;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        
    private:
        typedef std::unordered_set<value_type, hasher, key_equal, allocator_type> __set_type;
        
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
        threadsafe_unordered_set() : __internal_set_() {}
        threadsafe_unordered_set(const __set_type& __s) : __internal_set_(__s) {}
        threadsafe_unordered_set(__set_type&& __s) : __internal_set_(std::move(__s)) {}
        threadsafe_unordered_set(initializer_list<value_type> __il) : __internal_set_(__il) {}
        
        template <class _InputIterator>
        threadsafe_unordered_set(_InputIterator __f, _InputIterator __l) : __internal_set_(__f, __l) {}
        
        threadsafe_unordered_set(const threadsafe_unordered_set&) = delete;
        threadsafe_unordered_set& operator=(const threadsafe_unordered_set&) = delete;
        threadsafe_unordered_set(threadsafe_unordered_set&&) = delete;
        threadsafe_unordered_set& operator=(threadsafe_unordered_set&&) = delete;
        
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
              typename _Value,
              typename _Hash = hash<_Value>,
              typename _Pred = equal_to<_Value>,
              typename _Alloc = allocator<_Value>
             >
    class threadsafe_unordered_multiset
    {
    public:
        typedef _Value                                      key_type;
        typedef key_type                                    value_type;
        typedef _Hash                                       hasher;
        typedef _Pred                                       key_equal;
        typedef _Alloc                                      allocator_type;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        
    private:
        typedef std::unordered_multiset<value_type, hasher, key_equal, allocator_type> __set_type;
        
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
        threadsafe_unordered_multiset() : __internal_set_() {}
        threadsafe_unordered_multiset(const __set_type& __s) : __internal_set_(__s) {}
        threadsafe_unordered_multiset(__set_type&& __s) : __internal_set_(std::move(__s)) {}
        threadsafe_unordered_multiset(initializer_list<value_type> __il) : __internal_set_(__il) {}
        
        template <class _InputIterator>
        threadsafe_unordered_multiset(_InputIterator __f, _InputIterator __l) : __internal_set_(__f, __l) {}
        
        threadsafe_unordered_multiset(const threadsafe_unordered_multiset&) = delete;
        threadsafe_unordered_multiset& operator=(const threadsafe_unordered_multiset&) = delete;
        threadsafe_unordered_multiset(threadsafe_unordered_multiset&&) = delete;
        threadsafe_unordered_multiset& operator=(threadsafe_unordered_multiset&&) = delete;
        
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
