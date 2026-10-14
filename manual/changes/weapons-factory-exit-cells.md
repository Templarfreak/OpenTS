---
title: Leave weapons factories through the door
category: fix
release: 0.2.0
targets:
- type: system
  id: production
  effect: changed
credit:
- Templarfreak
---

A non-tunnel product that does not use drive movement leaving a `WeaponsFactory=yes` building heads to the door cell first, as drive vehicles do, and uses the first passable cell of the building's exit list only when the door cell is blocked. Infantry produced there now leave through the door and walk to the rally point once they are outside the building.
