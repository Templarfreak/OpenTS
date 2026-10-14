---
key: RallyPointOnWater
scope: buildingtype
label: BuildingType
summary: Allows a BuildingType rally point to be set on water.
see_also: ["RallyPointOnLand", "system:production"]
when_omitted:
  kind: value
  value: "no"
---

Set `RallyPointOnWater=yes` in a `[rules.ini]` BuildingType section to allow rally-point targets on water. The clicked cell must be in the building's amphibious movement zone, and the engine records the nearest reachable water cell. `RallyPointOnLand=` controls non-water targets.
