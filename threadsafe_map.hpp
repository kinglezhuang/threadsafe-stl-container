//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <map>
#include <mutex>
#include <memory>
#include <utility>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <
              typename _Key, typename _Tp,
              typename _Compare = less<_Key>,
              typename _Allocator = allocator<pair<const _Key, _Tp>>
             >
    class threadsafe_map
    {
    public:
        typedef _Key                                     key_type;
        typedef _Tp                                      mapped_type;
        typedef pair<const key_type, mapped_type>        value_type;
        typedef _Compare                                 key_compare;
        typedef _Allocator                               allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
    
        class value_compare
        : public std::binary_function<value_type, value_type, bool>
        {
            friend class threadsafe_map;
        
        protected:
            key_compare __comp_;
        
            value_compare(key_compare __c) : __comp_(__c) {}
        
        public:
            bool operator()(const value_type& __x, const value_type& __y) const
            { return __comp_(__x.first, __y.first); }
        };
    
    private:
        typedef std::map<key_type, mapped_type, key_compare, allocator_type> __map_type;
    
        std::shared_timed_mutex __mutex_;
        __map_type __internal_map_;
    
    public:
        typedef typename __map_type::pointer                pointer;
        typedef typename __map_type::const_pointer          const_pointer;
        typedef typename __map_type::size_type              size_type;
        typedef typename __map_type::difference_type        difference_type;
        typedef typename __map_type::iterator               iterator;
        typedef typename __map_type::const_iterator         const_iterator;
        typedef typename __map_type::reverse_iterator       reverse_iterator;
        typedef typename __map_type::const_reverse_iterator const_reverse_iterator;
    
    public:
        threadsafe_map() : __internal_map_() {}
        threadsafe_map(const __map_type& __m) : __internal_map_(__m) {}
        threadsafe_map(__map_type&& __m) : __internal_map_(std::move(__m)) {}
        threadsafe_map(initializer_list<value_type> __il) : __internal_map_(__il) {}
    
        template <class _InputIterator>
        threadsafe_map(_InputIterator __f, _InputIterator __l) : __internal_map_(__f, __l) {}
    
        threadsafe_map(const threadsafe_map&) = delete;
        threadsafe_map& operator=(const threadsafe_map&) = delete;
        threadsafe_map(threadsafe_map&&) = delete;
        threadsafe_map& operator=(threadsafe_map&&) = delete;
    
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
              typename _Compare = less<_Key>,
              typename _Allocator = allocator<pair<const _Key, _Tp>>
             >
    class threadsafe_multimap
    {
    public:
        typedef _Key                                     key_type;
        typedef _Tp                                      mapped_type;
        typedef pair<const key_type, mapped_type>        value_type;
        typedef _Compare                                 key_compare;
        typedef _Allocator                               allocator_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
    
        class value_compare
        : public std::binary_function<value_type, value_type, bool>
        {
            friend class threadsafe_multimap;
            
        protected:
            key_compare __comp_;
            
            value_compare(key_compare __c) : __comp_(__c) {}
            
        public:
            bool operator()(const value_type& __x, const value_type& __y) const
            { return __comp_(__x.first, __y.first); }
        };
    
    private:
        typedef std::multimap<key_type, mapped_type, key_compare, allocator_type> __map_type;
    
        std::shared_timed_mutex __mutex_;
        __map_type __internal_map_;
    
    public:
        typedef typename __map_type::pointer                pointer;
        typedef typename __map_type::const_pointer          const_pointer;
        typedef typename __map_type::size_type              size_type;
        typedef typename __map_type::difference_type        difference_type;
        typedef typename __map_type::iterator               iterator;
        typedef typename __map_type::const_iterator         const_iterator;
        typedef typename __map_type::reverse_iterator       reverse_iterator;
        typedef typename __map_type::const_reverse_iterator const_reverse_iterator;
        
    public:
        threadsafe_multimap() : __internal_map_() {}
        threadsafe_multimap(const __map_type& __m) : __internal_map_(__m) {}
        threadsafe_multimap(__map_type&& __m) : __internal_map_(std::move(__m)) {}
        threadsafe_multimap(initializer_list<value_type> __il) : __internal_map_(__il) {}
        
        template <class _InputIterator>
        threadsafe_multimap(_InputIterator __f, _InputIterator __l) : __internal_map_(__f, __l) {}
        
        threadsafe_multimap(const threadsafe_multimap&) = delete;
        threadsafe_multimap& operator=(const threadsafe_multimap&) = delete;
        threadsafe_multimap(threadsafe_multimap&&) = delete;
        threadsafe_multimap& operator=(threadsafe_multimap&&) = delete;
    
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
