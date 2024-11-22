#pragma once

#define xstr(a) str(a)
#define str(a) #a

#define uncopyable(classname) \
  classname(const classname&) = delete; \
  classname& operator=(const classname&) = delete

#define unmovable(classname)  \
  classname(classname&&) = delete;  \
  classname& operator=(classname&&) = delete

#define using_as_super(superclass) \
  using Super = superclass
