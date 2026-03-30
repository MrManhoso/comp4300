#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"

class entity_manager
{
  public:
    void init();
    void update();
    entity_p add_entity(const entity_tag& tag);
    EntityVec& get_entities();
    EntityVec& get_entities(const entity_tag& tag);

  private:
    EntityVec entities_;
    EntityMap entity_map_;
    EntityVec to_add_;
    int tot_entities_;
};

#endif