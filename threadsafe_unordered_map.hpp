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
#include <unordered_map>

namespace std
{
    template <
              typename _Key, typename _Tp,
              typename _Hash = hash<_Key>,
              typename _Pred = equal_to<_Key>,
              typename _Alloc = allocator<pair<const _Key, _Tp>>
             >
    class threadsafe_unordered_map
    {
    public:
        typedef _Key                                           key_type;
        typedef _Tp                                            mapped_type;
        typedef _Hash                                          hasher;
        typedef _Pred                                          key_equal;
        typedef _Alloc                                         allocator_type;
        typedef pair<const key_type, mapped_type>              value_type;
        typedef value_type&                                    reference;
        typedef const value_type&                              const_reference;

    private:
        typedef std::unordered_map<key_type, mapped_type, hasher, key_equal, allocator_type> __map_type;
        std::shared_timed_mutex __mutex_;
        __map_type __internal_map_;
    
    public:
        typedef typename __map_type::pointer                pointer;
        typedef typename __map_type::const_pointer          const_pointer;
        typedef typename __map_type::size_type              size_type;
        typedef typename __map_type::difference_type        difference_type;
        typedef typename __map_type::iterator               iterator;
        typedef typename __map_type::const_iterator         const_iterator;
    
    public:
        threadsafe_unordered_map() : __internal_map_() {}
        threadsafe_unordered_map(const __map_type& __m) : __internal_map_(__m) {}
        threadsafe_unordered_map(__map_type&& __m) : __internal_map_(std::move(__m)) {}
        threadsafe_unordered_map(initializer_list<value_type> __il) : __internal_map_(__il) {}
        
        template <class _InputIterator>
        threadsafe_unordered_map(_InputIterator __f, _InputIterator __l) : __internal_map_(__f, __l) {}
        
        threadsafe_unordered_map(const threadsafe_unordered_map&) = delete;
        threadsafe_unordered_map& operator=(const threadsafe_unordered_map&) = delete;
        threadsafe_unordered_map(threadsafe_unordered_map&&) = delete;
        threadsafe_unordered_map& operator=(threadsafe_unordered_map&&) = delete;
    
    public:
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.size();
        }
        
        bool insert(const key_type& __k, const mapped_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.insert(std::make_pair(__k, __v)).second;
        }
        
        void insert(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_.insert(__il);
        }
        
        const mapped_type& operator[](const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_[__k];
        }
        
        const mapped_type& at(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.at(__k);
        }
        
        void set(const key_type& __k, const mapped_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_[__k] = __v;
        }
        
        const std::pair<const mapped_type, bool> get(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_map_.find(__k);
            if (it == __internal_map_.end())
            {
                return std::make_pair(mapped_type(), false);
            }
            else
            {
                return std::make_pair(it->second, true);
            }
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_.clear();
        }
        
        bool contains(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_map_.find(__k);
            return it != __internal_map_.end();
        }
        
        size_type erase(const key_type& __k)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.erase(__k);
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_map_)
            {
                __bl(v);
            }
        }
    };
    
    
    template <
              typename _Key, typename _Tp,
              typename _Hash = hash<_Key>,
              typename _Pred = equal_to<_Key>,
              typename _Alloc = allocator<pair<const _Key, _Tp>>
             >
    class threadsafe_unordered_multimap
    {
    public:
        typedef _Key                                           key_type;
        typedef _Tp                                            mapped_type;
        typedef _Hash                                          hasher;
        typedef _Pred                                          key_equal;
        typedef _Alloc                                         allocator_type;
        typedef pair<const key_type, mapped_type>              value_type;
        typedef value_type&                                    reference;
        typedef const value_type&                              const_reference;
        
    private:
        typedef std::unordered_multimap<key_type, mapped_type, hasher, key_equal, allocator_type> __map_type;
        std::shared_timed_mutex __mutex_;
        __map_type __internal_map_;
        
    public:
        typedef typename __map_type::pointer                pointer;
        typedef typename __map_type::const_pointer          const_pointer;
        typedef typename __map_type::size_type              size_type;
        typedef typename __map_type::difference_type        difference_type;
        typedef typename __map_type::iterator               iterator;
        typedef typename __map_type::const_iterator         const_iterator;
        
    public:
        threadsafe_unordered_multimap() : __internal_map_() {}
        threadsafe_unordered_multimap(const __map_type& __m) : __internal_map_(__m) {}
        threadsafe_unordered_multimap(__map_type&& __m) : __internal_map_(std::move(__m)) {}
        threadsafe_unordered_multimap(initializer_list<value_type> __il) : __internal_map_(__il) {}
        
        template <class _InputIterator>
        threadsafe_unordered_multimap(_InputIterator __f, _InputIterator __l) : __internal_map_(__f, __l) {}
        
        threadsafe_unordered_multimap(const threadsafe_unordered_multimap&) = delete;
        threadsafe_unordered_multimap& operator=(const threadsafe_unordered_multimap&) = delete;
        threadsafe_unordered_multimap(threadsafe_unordered_multimap&&) = delete;
        threadsafe_unordered_multimap& operator=(threadsafe_unordered_multimap&&) = delete;
        
    public:
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.size();
        }
        
        void insert(const key_type& __k, const mapped_type& __v)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_.insert(std::make_pair(__k, __v));
        }
        
        void insert(initializer_list<value_type> __il)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_.insert(__il);
        }
        
        const std::pair<const mapped_type, bool> get(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_map_.find(__k);
            if (it == __internal_map_.end())
            {
                return std::make_pair(mapped_type(), false);
            }
            else
            {
                return std::make_pair(it->second, true);
            }
        }
        
        void clear()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_map_.clear();
        }
        
        bool contains(const key_type& __k)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            auto it = __internal_map_.find(__k);
            return it != __internal_map_.end();
        }
        
        size_type erase(const key_type& __k)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_map_.erase(__k);
        }
        
        void for_each(std::function<void(const value_type&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            for (const auto& v : __internal_map_)
            {
                __bl(v);
            }
        }
    
        void for_each(const key_type& __k, std::function<void(const std::pair<iterator, iterator>&)> __bl)
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            std::pair<iterator, iterator> r = __internal_map_.equal_range(__k);
            __bl(r);
        }
    };
}