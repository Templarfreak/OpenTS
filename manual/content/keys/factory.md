---
key: Factory
summary: A legacy alias for one of a BuildingType's built-in production queues.
see_also: ["system:production", "Produces"]
when_omitted:
  kind: context-dependent
  note: "If Produces= is present, it supplies the queues; otherwise this type produces nothing."
---

`Factory=` assigns one of the four built-in production queues to a BuildingType. Use `Produces=` for named queues or to assign more than one queue.

```ini title="rules.ini"
[MYWEAP] ; example war factory BuildingType
Factory=UnitType
```

`Factory=` accepts `UnitType`, `InfantryType`, `AircraftType` and `BuildingType`, plus the short spellings `Unit`, `Infantry`, `Aircraft` and `Building`. Names match without regard to case. Both player and computer houses map these spellings to the same built-in queues.

If `Produces=` is present, the engine ignores `Factory=`, even when `Produces=` is empty. See [Production and factories](/systems/production/) for queue assignment, factory selection and build limits.

The short names `Unit`, `Infantry`, `Aircraft` and `Building` also parse, but only a computer house's structures produce with them. A player's structure with a short name never produces anything, although it still counts toward the multiple-factory speed-up. Any other value, including an unrecognized one, leaves the structure producing nothing. [What counts as a factory](/systems/production/#what-counts-as-a-factory) covers how a player's order picks among the house's factories.
