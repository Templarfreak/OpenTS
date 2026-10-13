---
key: ProducedIn
summary: The production queue assigned to an object type.
see_also: ["system:production", "Produces"]
when_omitted:
  kind: computed
  note: "The type uses the built-in queue matching its object category."
---

`ProducedIn=` assigns an object type to one production queue. A BuildingType can produce the type only when its `Produces=` list includes the same queue name.

```ini title="rules.ini"
[HMMWV] ; example UnitType
ProducedIn=Ground

[MYWEAP] ; example BuildingType
Produces=Ground
```

Queue names match without regard to case. A name other than `none` or `<none>` creates a queue if no matching name exists. Set `ProducedIn=none` or `ProducedIn=<none>` to keep the type out of production.
