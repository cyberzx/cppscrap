#pragma once
#include <functional>

template<typename Func>
class RAIObj
{
public:
  RAIObj(Func const& on_scope_exit) : onScopeExit(on_scope_exit)
  {
  }

  RAIObj(RAIObj<Func>&& r) : onScopeExit(r.onScopeExit)
  {
    // the can't do nothing for reset const ref. so just mark move flag
    r.moved = true;
  }

  ~RAIObj()
  {
    if (!moved)
      onScopeExit();
  }

  RAIObj(const RAIObj<Func>&) = delete;
  void operator=(const RAIObj<Func>&) = delete;

private:
  Func const& onScopeExit;
  bool moved = false;
};

template<class Func>
RAIObj<Func> make_rai(Func const& on_scope_exit)
{
  return RAIObj<Func>(on_scope_exit);
}

#define CONCAT_IMPL(a, b) a ## b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define ON_SCOPE_EXIT(func) auto CONCAT(raiobj, __COUNTER__)(std::move(make_rai(func)))
#define INIT_SYS(init, shut) ON_SCOPE_EXIT(shut); init()

