### ECS

Component, Entity, Manager

Component { Entity, init, update, draw, ~Component }
Manager { entities, groupedEntities, update(), draw(), refresh(), addToGroup, getGroup, addEntity }

### BirchEngine

Manager:
  entities -> unique_ptr<Entity>[]
  groupedEntities -> array<vector<Entity*>, maxGroups>

  update -> for entity in entities, entity.update
  draw -> for entity in entities, entity.draw
  refresh ->
    ```
      for i < maxGroups:
        for all elements in groupedEntities[i]:
          remove if not active or not has group(i)
      Remove from entities if a certain entity is not active
    ```
  addToGroup(entity, group) -> groupedEntities[group].append(entity)
  getGroup(group) -> groupedEntities[group]
  addEntity() -> { e = new Entity; uniquePtr uPtr {e}; entities.add(uptr); return *e }

Entity:
  { manager, components, componentArray, componentBitset, groupBitset }
  components: vector<ptr<Component>
  componentArray: componentArray

  update: for component in components, component.update
  draw: for component in components, component.draw
  hasComponent: componentBitset[getComponentTypeID<T>()]
  addComponent(args) ``` { } ```

Component:
  Component { Entity, init, update, draw, ~Component }
  init, update, draw -> virtual
