---
title: Assign types and factories to production queues
category: feature
release: 0.2.0
targets:
- type: key
  id: ProducedIn
  effect: added
- type: key
  id: Produces
  effect: added
- type: key
  id: Factory
  effect: changed
credit:
- Templarfreak
---

Set `ProducedIn=` in `[rules.ini]` type sections to assign object types to named queues, and list the queues a BuildingType serves with `Produces=`. A queue can contain types from different categories, and separate queues can build structures at the same time. `Factory=` remains the single built-in-queue alias, its short spellings now work for player houses too, and players can choose a primary factory for overlapping queues.
