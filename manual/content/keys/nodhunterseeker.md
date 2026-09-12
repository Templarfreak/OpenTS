---
key: NodHunterSeeker
summary: The type created by a hunter-seeker superweapon fired by any house not acting as GDI.
see_also: [GDIHunterSeeker, HSBuilding, "system:superweapons"]
when_omitted:
  kind: value
  value: none
---

This is the global fallback for a Hunter Seeker superweapon whose side-specific `<SuperWeaponName>HunterSeeker` key, side `HunterSeeker` value, and own `HunterSeeker` value are blank. A house that is not acting as GDI gets this type. The named type is created as a vehicle at the cell chosen from [`HSBuilding`](/keys/hsbuilding/), facing east, and then [acquires its own target](/systems/superweapons/#hunter-seeker).
