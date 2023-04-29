#ifndef __WASTILS_COROUTINE_HPP
#define __WASTILS_COROUTINE_HPP

/*
Credit: https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
*/

#include <coroutine>
#include <iostream>

namespace was{
    struct Future {
        struct promise_type {
            int value_;

            Future get_return_object() {
                return {
                .h_ = std::coroutine_handle<promise_type>::from_promise(*this)
                };
            }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void unhandled_exception() {}
            std::suspend_always yield_value(int value) {
                value_ = value;
                return {};
            }
            void return_void() {}
        };

    std::coroutine_handle<promise_type> h_;

    void destroy(){h_.destroy(); }
    int operator()(){
        h_();
        return h_.promise().value_;
    }
    };
}

#endif