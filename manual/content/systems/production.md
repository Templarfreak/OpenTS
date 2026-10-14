---
title: Production and factories
summary: "How production queues connect object types to factories and control what houses build."
category: buildings-economy
keys:
  - AltToRally
  - Armory
  - BuildLimit
  - BuildSpeed
  - BuildTime
  - BuildupTime
  - ConstructionYard
  - Cost
  - DoubleOwned
  - Factory
  - FreeUnit
  - GameSpeedBias
  - Gate
  - GateStages
  - Hospital
  - MaximumQueuedObjects
  - MinProductionSpeed
  - MultipleFactory
  - MultipleFactoryCap
  - Owner
  - PadAircraft
  - PlacementDelay
  - Prerequisite
  - PrerequisiteBarracks
  - PrerequisiteFactory
  - PrerequisiteGDIFactory
  - PrerequisiteNodFactory
  - PrerequisitePower
  - PrerequisiteRadar
  - PrerequisiteTech
  - ProducedIn
  - Produces
  - RallyPointOnLand
  - RallyPointOnWater
  - ScoldSound
  - SeparateAircraft
  - TechLevel
  - Wall
  - WallBuildSpeedCoefficient
  - WeaponsFactory
related:
  - type: system
    id: ai-base-building
  - type: system
    id: power
---

Each object type belongs to one production queue. [`ProducedIn=`](/keys/producedin/) assigns the queue; when it is omitted, the type uses the built-in queue matching its category. A BuildingType's [`Produces=`](/keys/produces/) lists the queues its structures can serve, and [`Factory=`](/keys/factory/) remains an alias for one built-in queue. A queue can contain products of different kinds.

A player's house has one production slot for each queue. Objects assigned to the same queue share its active build and waiting orders, even when their kinds differ. Separate queues build independently. Extra factories still shorten build time by the product's category, as [More than one factory](#more-than-one-factory) explains.

A computer house keeps one production process per factory and queue. A structure that produces several queues can build one object from each at the same time. The AI selects a product for each queue from team needs or its base plan; a queue stays idle when the AI has no matching product to build. The AI selects only one structure for production at a time and waits for it to be placed before selecting another.

Four built-in queues cover aircraft, infantry, vehicles and structures. Rules can add up to 64 custom queues. Queue names match without regard to case. After that limit, a new name is not registered, so a type assigned that name has no production queue and a factory listing it does not serve it.

In both cases an object takes 54 production steps, and part of its price is charged as the steps advance. The finished object then waits out of play until a factory lets it out.

## What counts as a factory

A BuildingType is a factory when `Produces=` or the legacy `Factory=` assigns it at least one production queue. The queue name, rather than the product's kind, determines which player slot receives an order.

```ini title="rules.ini"
[MEDIC] ; example InfantryType
ProducedIn=Army

[HMMWV] ; example UnitType
ProducedIn=Army

[MYFAC] ; example BuildingType
Produces=Army,AircraftType
WeaponsFactory=yes
Owner=GDI,Nod
```

When a player orders an object, the house looks for a structure that passes all of these tests, in this order:

- it is on the map;
- it is owned by this house;
- its [`Produces=`](/keys/produces/) includes the product's [`ProducedIn=`](/keys/producedin/) queue;
- it is switched on;
- it is neither being sold nor queued to be sold;
- the product has not reached its [build limit](#build-limits);
- the factory type's [`Owner=`](/keys/owner/) shares at least one country with the product's `Owner=`;
- **Any of:**
  - the factory's type is not listed in [`BuildConst`](/keys/buildconst/);
  - [`MultiMCV=yes`](/keys/multimcv/) is set;
  - the structure acts for a country that is one of the product's owners.

A structure keeps the country it acts for when it changes hands, so the last test lets a captured construction yard keep building its original owner's structures.

The search takes the first [primary factory](#the-primary-factory) it finds that passes. Without one, it takes the last passing structure. For an aircraft, a pad already in radio contact with an aircraft, such as one parked on it, is used only when no free pad passes.

A computer house does not search for a player slot. Each factory-and-queue pair builds only the product selected for its queue by the AI. The AI selects vehicles, infantry and aircraft from team needs, and structures from its base plan.

Use `Produces=` for named or multiple queues. `Factory=` accepts the built-in names `UnitType`, `InfantryType`, `AircraftType` and `BuildingType`, as well as their short spellings. The short spellings now map to the same built-in queues for player and computer houses. If `Produces=` is present, it takes precedence over `Factory=`.

### The primary factory

A player's finished objects leave from the primary factory for their queue whenever it passes the factory search.

Toggling a structure that is primary for all its queues clears those flags. Otherwise, it becomes primary for every queue it produces and clears those queue flags on competing structures of the same house. EVA announces a newly set primary to a player-controlled house.

A structure loses the flag when it is captured. An [`IsMobileWar=yes`](/keys/ismobilewar/) structure toggles the flag on itself when it opens.

The cursor that offers the toggle appears only over a structure that passes all of these, in this order:

- it is not stunned by an [EM pulse](/systems/emp-pulse/);
- it produces at least one queue;
- it belongs to the local player;
- another structure of the house produces at least one of the same queues.

## What a house may build

Four gates decide whether a house may build an object type. They are checked in this order, and a type must pass all four.

With [`RecheckPrerequisites=yes`](/keys/recheckprerequisites/), [the sidebar sweep](/systems/sidebar/#what-removes-a-cameo) applies all four gates again and cancels production of any type that fails.

### Tech level

[`TechLevel=-1`](/keys/techlevel/#scope-aircrafttype) makes a type unbuildable. Otherwise the type's level must not exceed the house's tech level. A house's level comes from [its section in the map](/keys/techlevel/#scope-house-per-scenario) and defaults to the scenario number. Outside campaigns, every house the session sets up gets the session's chosen level instead, which starts from [`[MultiplayerDefaults] TechLevel`](/keys/techlevel/#scope-global-rules).

### Prerequisites

Every entry in the type's [`Prerequisite=`](/keys/prerequisite/) list must be satisfied. An entry naming a BuildingType requires the house to own at least one live structure of exactly that type. A structure counts from the moment it is placed, so the prerequisite unlocks during its buildup animation. It still counts while switched off.

Seven group names are satisfied by owning any structure on the matching rules list:

| `Prerequisite=` entry | List read |
| --- | --- |
| `POWER` | [`PrerequisitePower`](/keys/prerequisitepower/) |
| `FACTORY` | [`PrerequisiteFactory`](/keys/prerequisitefactory/) |
| `BARRACKS` | [`PrerequisiteBarracks`](/keys/prerequisitebarracks/) |
| `RADAR` | [`PrerequisiteRadar`](/keys/prerequisiteradar/) |
| `TECH` | [`PrerequisiteTech`](/keys/prerequisitetech/) |
| `GDIFACTORY` | [`PrerequisiteGDIFactory`](/keys/prerequisitegdifactory/) |
| `NODFACTORY` | [`PrerequisiteNodFactory`](/keys/prerequisitenodfactory/) |

:::caution[An upgrade prerequisite checks only the newest structure]
When a prerequisite names an upgrade (a type with [`PowersUpBuilding=`](/keys/powersupbuilding/)), one structure decides the test: the house's newest structure that is on the map, switched on and not being sold, whatever its type. The prerequisite is met only if that structure has the upgrade installed. The same upgrade installed on any other structure does not count.
:::

### Ownership

This gate applies only to structures. A BuildingType passes when both of these hold:

- its `Owner=` list is not empty;
- the house owns a construction yard that is on the map, switched on, not being sold, and acting for one of the countries in that list.

[`MultiMCV=yes`](/keys/multimcv/) removes the construction-yard requirement, both here and in the factory search. [`DoubleOwned=yes`](/keys/doubleowned/) opens the type to every country, but only outside campaign games.

Vehicles, infantry and aircraft skip this gate. The factory search enforces their ownership instead, by requiring the factory's `Owner=` to share a country with the product's.

### Build-limit gate

The last gate is the [build limit](#build-limits). It is the only gate the factory search applies again when an order is placed.

### Computer houses

A computer house faces only the tech-level gate. Every type that passes it counts as buildable, and [base planning](/systems/ai-base-building/) decides what the house actually produces.

## Build limits

The sign of [`BuildLimit=`](/keys/buildlimit/) chooses what the limit counts.

| `BuildLimit=` | Counted | Effect |
| --- | --- | --- |
| Above zero | Objects of the type the house owns now, including one still in production | Losing one frees room for another |
| Zero | Nothing | The type can never be built |
| Below zero | Objects of the type the house has ever produced | Losing one frees nothing |

A positive limit counts more than the objects of the type itself:

- A UnitType with a [`DeploysInto=`](/keys/deploysinto/) structure also counts the house's structures of that type, so construction yards use up an MCV's limit.
- An InfantryType with [`VehicleThief=yes`](/keys/vehiclethief/) also counts the house's vehicles that infantry of its type has entered.

A positive limit counts an object from the moment its production starts. A type the house is already building still counts as buildable, so the build is not canceled when its unfinished object brings the count to the limit.

On the sidebar, a type that has reached a positive limit is drawn darkened. A type whose zero or negative limit is used up loses its cameo.

A player's orders are checked against the limit in two places:

1. The factory search rejects every structure while the product is at its limit, so a new order fails.
2. An order that would join a [queue](#the-queue) is refused when the objects owned or produced, plus the object under construction and everything already queued, reach the limit. At a positive limit, the object under construction is already among the objects owned, so the check counts it only once. With `BuildLimit=2`, nothing finished and one in production, one more order joins the queue and the next is refused. For a vehicle, this check does not count deployed structures. It has no case for structures, which are never queued.

:::caution[Build limits do not restrain a computer house]
A computer house's factories never check the limit, so the computer keeps producing a type past it. The limit still applies on a few other computer paths. The most visible is team creation. Outside campaigns, a computer team that names a member at its build limit cannot be created. In a campaign it can still form if the house already owns a recruitable member of that type.
:::

## How long it takes

Build time is counted in game frames. It is computed in this order and truncated to a whole number after each step:

1. Start from the object's [`Cost=`](/keys/cost/#scope-aircrafttype), multiplied by [`BuildSpeed`](/keys/buildspeed/) and by 0.9 frames per credit. A structure uses its `Cost=` as written, which includes the price of any [`FreeUnit`](/keys/freeunit/) or pad aircraft that comes with it. (Repair costs use the price without them.)
2. Multiply by the house's build-time multiplier. That multiplier is fixed when the house is given its difficulty, as the country's [`BuildTime=`](/keys/buildtime/#scope-housetype) times the difficulty's [`BuildTime=`](/keys/buildtime/#scope-difficulty-settings) times [`GameSpeedBias`](/keys/gamespeedbias/). Campaign games leave out the country's `BuildTime=`.
3. Divide by the house's [power multiplier](#power).
4. Multiply by the [multiple-factory adjustment](#more-than-one-factory).
5. For a [`Wall=yes`](/keys/wall/#scope-buildingtype) BuildingType, multiply by [`WallBuildSpeedCoefficient`](/keys/wallbuildspeedcoefficient/).

### Production steps

The result sets the pace of the build, not its exact length. Divided by 54 and truncated, it gives the number of frames between production steps, kept between 1 and 255. The object takes 54 of those steps.

With every multiplier at 1, a unit costing 1000 credits gives 900 frames. Dividing by 54 gives 16 frames per step after truncation, so the unit finishes in 864 frames. At 15 frames a second that is 57.6 seconds, not the full minute its price suggests.

The 1-to-255 range on the step sets the shortest build at 54 frames (3.6 seconds) and the longest at 13,770 frames (about 15.3 minutes). With every multiplier at 1, every object costing under 120 credits takes the minimum, and every object costing 15,300 credits or more takes the maximum.

### More than one factory

Extra factories can shorten build times, depending on [`MultipleFactory`](/keys/multiplefactory/). The house counts each structure once for every kind it can produce—aircraft, infantry, vehicles or structures—including structures switched off or still in buildup. A queue name does not affect this count. The build time is multiplied by `MultipleFactory` once for each factory past the first in the product's kind and truncated after each multiplication. [`MultipleFactoryCap`](/keys/multiplefactorycap/) limits how many factories count; at `0` every factory counts.

| Factories | Multiplier at `MultipleFactory=0.8` | With `MultipleFactoryCap=3` as well |
| ---: | --- | --- |
| 1 | 1 | 1 |
| 2 | 0.8 | 0.8 |
| 3 | 0.64 | 0.64 |
| 4 | 0.512 | 0.64 |

A value of `1` changes nothing, and a value above `1` makes each extra factory lengthen build times. A value of `0` or below skips the adjustment, as the stock rules do.

### Power

A house short of power builds more slowly. Its build time is divided by a power multiplier taken from a fixed ladder and never below [`MinProductionSpeed`](/keys/minproductionspeed/). [The production ladder](/systems/power/#production) lists the bands.

The multiplier is refreshed whenever the house's power is recalculated: when a structure is placed, lost, damaged, or switched on or off, even if the power balance does not change. A running build picks up the new step interval at that recalculation. A build also takes a fresh interval when it resumes from hold or suspension. Either way it keeps the step it had reached, so low power slows it in place without restarting it.

### Difficulty and campaign games

The country and difficulty multipliers are combined once, when the house is given its difficulty, not for each order. Campaign games leave out the country's multipliers for both build time and price. The `Cost=` and `BuildTime=` in a country section such as `[GDI]` or `[Nod]` therefore affect only skirmish and multiplayer games.

A difficulty's `Cost=` and `BuildTime=` are independent. `Cost=` changes what everything costs without changing build times, and `BuildTime=` changes build times without changing costs.

## Paying for it

The price is charged in installments as the steps advance. Each installment is the unpaid balance divided by the number of steps remaining, so rounding never gains or loses credits, and the balance is exactly zero when the object is finished.

A build stalls while the next installment is more than the house's credits plus its stored Tiberium. That step is undone, nothing is charged, and the step is tried again after the next interval, so the build continues as soon as the money arrives. A house may start or resume a build it cannot yet afford. The build makes no progress until the money is there.

Putting a build on hold charges nothing and keeps the step it had reached.

Canceling a build refunds what has been paid so far: the price minus the unpaid balance. It also deletes the unfinished object and frees its production slot. The refund uses the price at the moment of canceling, so if a price multiplier changed during the build, the refund does not match what was paid.

## The queue

Only a player's production slots have waiting queues. A computer house holds one object per factory and queue, with nothing behind each object.

An order placed while its queue is busy joins that queue, regardless of the product's kind. The queue holds up to [`MaximumQueuedObjects`](/keys/maximumqueuedobjects/) waiting entries, plus the object in progress. An order is dropped if the queue is full or the type has reached its build limit, and a player-controlled house then hears [`ScoldSound`](/keys/scoldsound/).

A queue on hold also accepts orders. Ordering its held type resumes the held build instead of queuing a second copy.

When the object in progress leaves its factory, is canceled, or becomes unbuildable, the first queued type starts as if it had just been ordered.

Structures do not join a waiting queue. A second structure order is refused while a structure in that same queue is building, on hold, or waiting to be placed. Structures assigned to different queues can be on order at the same time. The [sidebar](/systems/sidebar/#clicking-a-build-cameo) draws other structure cameos for a busy queue darkened.

## Leaving the factory

After the last step the finished object waits in its queue's production slot. How it leaves depends on whether a player or a computer house built it.

A player's finished vehicle, aircraft or infantryman is announced and immediately tries to leave through a factory. A finished structure is announced and waits; clicking its cameo enters placement mode.

While every factory that produces its queue is switched off, a finished vehicle, aircraft or infantryman keeps waiting. Clicking its cameo then only repeats the exit attempt, which fails the same way.

If no factory of the house could build the object any more, even a switched-off one, clicking its cameo cancels and refunds the order, and EVA reports that there is no factory.

An exit attempt either succeeds, is temporarily blocked, or fails permanently. Success releases the object and clears the slot. For a player's vehicle, aircraft or infantryman, either kind of failure cancels and refunds the order, even a blockage the factory would soon have cleared. A structure is never canceled this way; it stays in the slot until it is placed.

A structure placed by hand appears on the chosen cell, plays the placement sound and clears the placement cursor. A firestorm wall, or a structure that lays a wall overlay, also fills the gap between itself and a nearby wall of the same house. If the chosen cell refuses the structure, it stays in the slot.

How a factory lets an object out depends on the factory:

- An aircraft appears on its pad. If the pad is already in radio contact with another aircraft, the new one appears at the edge of the playable area and flies in instead. During an ion storm, it appears on a nearby cell.
- A [`WeaponsFactory=yes`](/keys/weaponsfactory/) structure opens its door and clears whatever stands on its exit cell. Vehicles follow the factory's drive exit track, tunnel units use that track after switching locomotors, and other non-tunnel `UnitType` objects that do not use drive movement head to the first passable cell in the building's exit list. While it is still unloading the previous object, the factory passes the next one to another idle structure of the same type, which lets it out through its own exit. If no such structure is free, the attempt is temporarily blocked.
- Every other factory moves the object out of its footprint to an exit cell beside it. If it finds no exit cell, the attempt fails permanently. A [`GDIBarracks=yes`](/keys/gdibarracks/) or [`NODBarracks=yes`](/keys/nodbarracks/) structure starts the object at its [`ExitCoord`](/keys/exitcoord/) offset when the exit cell is its door cell.
- A factory lets out one vehicle or infantryman at a time. While it is still in radio contact with the last object to leave, the next attempt is temporarily blocked. [`Hospital=yes`](/keys/hospital/), [`Armory=yes`](/keys/armory/) and `WeaponsFactory=yes` structures are exempt.
- A refinery or weeder cannot release what it produces. A vehicle briefly appears south-west of it and heads off to harvest, but the order is then canceled and refunded, which removes the vehicle again. Infantry fail the same way.

A jumpjet infantryman whose route to the rally point calls for flight skips the exit cell and flies straight to the rally point. It releases the factory's radio contact at once, so the next object can leave. Other infantry, and jumpjets with a nearby rally point or none, walk to the exit cell as usual.

For a computer house, a vehicle or infantryman leaving through an exit cell (from any factory except a `WeaponsFactory=yes` one) moves to that cell first. The base position its house chooses for it is queued next and becomes the area it guards. If the house has no valid position, the object only clears the factory.

A computer house's factories retry exits on their own and treat the outcomes differently:

- A temporary blockage waits [`PlacementDelay`](/keys/placementdelay/) minutes before the next attempt.
- A permanent failure cancels and refunds the order.

A computer factory never keeps a build on hold. If its build is put on hold or suspended, the factory cancels and refunds it.

An idle computer factory-and-queue pair starts the house's next eligible product for that queue once the house has more than 10 credits, counting stored Tiberium.

### Buildup

A newly placed structure plays its construction animation before it opens. It tells the structure that built it when construction begins, which starts a [`ConstructionYard=yes`](/keys/constructionyard/) structure's [`PreProductionAnim`](/keys/preproductionanim/). It tells the builder again when the animation ends, and then opens.

[`BuildupTime`](/keys/builduptime/) sets the animation's length in minutes. It is divided evenly across the animation's steps: half the number of frames in the [buildup art](/keys/buildup/), or [`GateStages`](/keys/gatestages/) plus one for a [`Gate=yes`](/keys/gate/) type. Like the build times above, the minutes are counted at 15 frames a second. A type with no buildup art opens at once.

A structure hands over what comes with it only when it opens. A [`FreeUnit`](/keys/freeunit/) vehicle or infantryman appears beside the structure, and a free aircraft appears on the structure itself. If a free object cannot be placed anywhere, the house gets its price back instead.

A structure present at scenario start hands over no `FreeUnit`.

A [`HoverPad=yes`](/keys/hoverpad/) structure also receives the first [`PadAircraft`](/keys/padaircraft/) entry, unless [`SeparateAircraft=yes`](/keys/separateaircraft/) is set or the structure's [`FreeUnit`](/keys/freeunit/) is an aircraft. A captured structure hands over neither.

## Rally points

A structure that produces vehicles, infantry or aircraft can hold a rally point. Select it and click the ground to set one. Holding the force-move key instead gives the structure a move order, which packs up a structure that names an [`UndeploysInto`](/keys/undeploysinto/) type. [`AltToRally=yes`](/keys/alttorally/) swaps the two.

The rally point is not always the clicked cell. The engine records the nearest cell to it where infantry could stand, or where an aircraft could fly for a factory that produces only aircraft among its mobile products. A factory that also produces vehicles or infantry uses ground movement rules. [`RallyPointOnLand=`](/keys/rallypointonland--buildingtype/) defaults to `yes`; [`RallyPointOnWater=`](/keys/rallypointonwater--buildingtype/) defaults to `no`. Water targets use the building's amphibious movement zone, and the engine records a reachable water cell in that zone. If there is no suitable cell, no rally point is set.

The cursor refuses cells outside the playable area. It also refuses water cells unless `RallyPointOnWater=yes` and the cell is in the building's amphibious movement zone. `RallyPointOnLand=no` refuses non-water cells, including bridge cells. Otherwise, the existing land movement-zone and passability checks still apply to factories that do not produce only aircraft.

What the departing object does with the rally point depends on the factory:

- An aircraft heads for it as soon as it appears, whether on its pad or at the edge of the playable area. During an ion storm the aircraft appears on a nearby cell and ignores the rally point.
- A [`WeaponsFactory=yes`](/keys/weaponsfactory/) structure passes it to the vehicle, which heads for it once it has cleared the building. A harvester or weeder then starts harvesting from there.
- A barracks, [`Hospital=yes`](/keys/hospital/) or [`Armory=yes`](/keys/armory/) structure passes it to the infantryman, who walks to the exit cell first and heads for the rally point the next time he is idle.
- Every other factory ignores it. A vehicle factory with [`WeaponsFactory=no`](/keys/weaponsfactory/) accepts a rally point, but its vehicles stop on the exit cell.

If a departing object belongs to a house a player controls and its type cannot be ordered into the shroud, the object ignores a rally point that is still shrouded for that player and leaves as if the factory had no rally point. That covers a type with [`MoveToShroud=no`](/keys/movetoshroud/), the default for aircraft types, and a subterranean type while [`AllowShroudedSubteranneanMoves=no`](/keys/allowshroudedsubteranneanmoves/). The check is made as each object leaves, so the rally point works again once the player uncovers that ground.

A structure stores its rally point as its move destination, so a structure that undeploys passes the rally point to the vehicle it becomes.

## When a factory is lost

Destroying a computer house's factory cancels and refunds whatever that factory was building. For a player, the build in the matching queue is canceled and refunded only if no remaining structure of the house could build it.

Capturing a factory cancels whatever it was building for a computer house before the transfer. The factory then moves from one house's factory count to the other's.

A house re-examines a production queue when a factory that produces from it is placed, leaves the map, is captured, or is switched on or off. A capture makes both houses re-examine the affected queues. Only a player's production queues are re-examined, and these rules apply in order:

1. Queued types that nothing the house owns could build, even in theory, are dropped from the queue.
2. An object in progress that nothing could build any more is canceled and refunded, and the queue advances.
3. An object that could be built in theory but not now, because every factory that could build it is switched off, is suspended. It keeps its step and the credits already paid.
4. An object that can be built again restarts, unless the player put it on hold.

A queue left with nothing in progress and nothing queued is then discarded.

A computer house's factory production is not re-examined. It ends when the structure is destroyed or captured, or when the factory cancels a build that has stopped making progress.

When a computer house cannot make money, it [sells its base for a replacement refinery or harvester](/systems/ai-base-building/#power-and-money-interventions) and cancels every build it has running. Handing a house over to the computer cancels its builds as well.
