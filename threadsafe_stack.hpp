//
//  threadsafe_stack.hpp
//  stl_extension
//
//  Created by Kingle Zhuang on 11/20/19.
//  Copyright © 2019 MZD. All rights reserved.
//

#pragma once

#include <stack>
#include <mutex>
#include <memory>
#include <functional>
#include <shared_mutex>

namespace std
{
    template <typename _Tp, typename _Container = deque<_Tp>>
    class threadsafe_stack
    {
    private:
        typedef std::stack<_Tp, _Container> __stack_type;
        
        std::shared_timed_mutex __mutex_;
        __stack_type __internal_stack_;
        
    public:
        typedef typename __stack_type::container_type  container_type;
        typedef typename __stack_type::value_type      value_type;
        typedef typename __stack_type::reference       reference;
        typedef typename __stack_type::const_reference const_reference;
        typedef typename __stack_type::size_type       size_type;
        
    public:
        threadsafe_stack() : __internal_stack_() {}
        explicit threadsafe_stack(const container_type& __c) : __internal_stack_(__c) {}
        explicit threadsafe_stack(container_type&& __c) : __internal_stack_(std::move(__c)) {}
        
        threadsafe_stack(const threadsafe_stack&) = delete;
        threadsafe_stack& operator=(const threadsafe_stack&) = delete;
        threadsafe_stack(threadsafe_stack&&) = delete;
        threadsafe_stack& operator=(threadsafe_stack&&) = delete;
        
    public:
        bool empty()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_stack_.empty();
        }
        
        size_type size()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return __internal_stack_.size();
        }
        
        const value_type& top()
        {
            std::shared_lock<std::shared_timed_mutex> lock(__mutex_);
            return static_cast<const value_type&>(__internal_stack_.top());
        }
        
        void push(const value_type& __x)
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_stack_.push(__x);
        }
        
        void pop()
        {
            std::unique_lock<std::shared_timed_mutex> lock(__mutex_);
            __internal_stack_.pop();
        }
    };
}