---
key: RallyPointOnLand
scope: buildingtype
label: BuildingType
summary: Allows a BuildingType rally point to be set on non-water terrain.
see_also: ["RallyPointOnWater", "system:production"]
when_omitted:
  kind: value
  value: "yes"
---

`RallyPointOnLand=no` prevents rally-point targets on non-water cells, including bridge cells. With `RallyPointOnLand=yes`, the building's existing movement-zone and passability checks still apply; aircraft-only factories keep their existing land-target rules. `RallyPointOnWater=` controls water targets.
