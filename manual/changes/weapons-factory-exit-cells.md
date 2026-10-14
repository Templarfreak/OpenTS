---
title: Use passable exits for non-drive factory products
category: fix
release: 0.2.0
targets:
- type: system
  id: production
  effect: changed
credit:
- Templarfreak
---

A non-tunnel `UnitType` product that does not use drive movement leaving a `WeaponsFactory=yes` building used one fixed destination, even when another cell in the building's exit list was passable. It now heads to the first passable cell in that list.
