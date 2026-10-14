---
title: Set rally-point surfaces by building
category: feature
release: 0.2.0
targets:
- type: key
  id: RallyPointOnWater
  effect: added
- type: key
  id: RallyPointOnLand
  effect: added
- type: system
  id: production
  effect: changed
breaking: true
migration:
- In each BuildingType section that relies on a water rally point, set `RallyPointOnWater=yes`.
credit:
- Templarfreak
---

Aircraft-only factories could previously accept water rally points without a setting. Water targets are now disabled by default; set `RallyPointOnWater=yes` in the BuildingType's `[rules.ini]` section to allow them. `RallyPointOnLand=no` separately rejects non-water targets, and land targets remain enabled by default.
