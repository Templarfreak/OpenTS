---
key: Produces
summary: The production queues a BuildingType can serve.
see_also: ["system:production", "Factory", "ProducedIn"]
when_omitted:
  kind: context-dependent
  note: "If Factory= is also absent, this BuildingType produces nothing."
---

`Produces=` assigns one or more production queues to a BuildingType. Separate queue names with commas; a product type's `ProducedIn=` value must match one of them for this factory to build it.

```ini title="rules.ini"
[MYWEAP] ; example BuildingType
Produces=Ground,Air
```

Queue names match without regard to case. `Produces=` takes precedence over `Factory=` whenever the key is present. Use `Factory=` only for one of the built-in queues.
