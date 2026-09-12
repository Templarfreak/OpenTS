---
key: GDIHunterSeeker
summary: The type created by a hunter-seeker superweapon fired by a house acting as GDI.
see_also: [NodHunterSeeker, HSBuilding, "system:superweapons"]
when_omitted:
  kind: value
  value: none
---

The global fallback uses the firing house's [`ActsLike=`](/keys/actslike/): a house acting as GDI gets this type, and every other house gets [`NodHunterSeeker`](/keys/nodhunterseeker/). A side can override one Hunter Seeker superweapon with `<SuperWeaponName>HunterSeeker`, or provide a generic `HunterSeeker` value for Hunter Seeker superweapons without that per-superweapon side key. A Hunter Seeker superweapon can define its own `HunterSeeker` unit type in its rules section. The named type is created as a vehicle at the cell chosen from [`HSBuilding`](/keys/hsbuilding/), facing east, and then [acquires its own target](/systems/superweapons/#hunter-seeker).
