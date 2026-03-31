#ifndef ENTITY_H
#define ENTITY_H

#include "data/components/ccollision.h"
#include "data/components/cinput.h"
#include "data/components/clifespan.h"
#include "data/components/cscore.h"
#include "data/components/cshape.h"
#include "data/components/ctransform.h"

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

using ctuple = std::tuple<ctransform, cshape, ccollision, cinput, cscore, clifespan>; // TODO more
using entity_tag = std::string;

class entity
{
  public:
    template <typename T>
    T& get()
    {

    }
    
    template <typename T>
    void add(T&&)
    {

    }

    template <typename T>
    bool has()
    {
        
    }

    inline int id() { return id_; }

    inline bool is_active() { return active_; }

    inline entity_tag tag() { return tag_; }

    inline void destroy();

  private:
    ctuple components_;
    bool active_;
    int id_;
    entity_tag tag_;
};

using entity_p = std::shared_ptr<entity>;
using EntityVec = std::vector<entity>;
using EntityMap = std::map<entity_tag, entity>;

#endif


