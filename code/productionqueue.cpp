/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "dbgprint.h"
#include "productionqueue.h"
#include "techtype.h"

#include "_productionqueue.h"
#include "vector.h"

ProductionQueueClass::ProductionQueueClass(char const * name) :
	ID(ProductionQueues.Count()),
	IniName(name)
{
	ProductionQueues.Add(this);
}


static bool QueuesInitialized = false;

static void Register_Retail_Queues(void)
{
	assert(ProductionQueues.Count() == PRODUCTION_QUEUE_AIRCRAFT);
	new ProductionQueueClass("AircraftType");
	assert(ProductionQueues.Count() == PRODUCTION_QUEUE_INFANTRY);
	new ProductionQueueClass("InfantryType");
	assert(ProductionQueues.Count() == PRODUCTION_QUEUE_UNIT);
	new ProductionQueueClass("UnitType");
	assert(ProductionQueues.Count() == PRODUCTION_QUEUE_BUILDING);
	new ProductionQueueClass("BuildingType");
}


void ProductionQueueClass::One_Time(void)
{
	if (QueuesInitialized) {
		return;
	}

	QueuesInitialized = true;
	Register_Retail_Queues();
}


// Drops every custom queue and every registered product so rules can be read again from
// scratch. Call it before the type heaps are deleted, because the product lists point into them.
void ProductionQueueClass::Reset(void)
{
	for (int index = 0; index < ProductionQueues.Count(); index++) {
		delete ProductionQueues[index];
	}
	ProductionQueues.Clear();
	QueuesInitialized = true;
	Register_Retail_Queues();
}


ProductionQueueClass * ProductionQueueClass::Find_Or_Make(char const * name)
{
	One_Time();

	if (name == nullptr || name[0] == '\0' || strcmpi(name, "none") == 0 || strcmpi(name, "<none>") == 0) {
		return(nullptr);
	}

	for (int index = 0; index < ProductionQueues.Count(); index++) {
		if (strcmpi(ProductionQueues[index]->IniName.c_str(), name) == 0) {
			return(ProductionQueues[index]);
		}
	}

	if (ProductionQueues.Count() >= PRODUCTION_QUEUE_COUNT) {
		DebugString("Unable to register production queue '%s': the limit of %d custom queues has been reached.\n",
			name, PRODUCTION_QUEUE_CUSTOM_LIMIT);
		return(nullptr);
	}

	return(new ProductionQueueClass(name));
}


ProductionQueueClass * ProductionQueueClass::From_ID(ProductionQueueID id)
{
	One_Time();

	if (id < 0 || id >= ProductionQueues.Count()) {
		return(nullptr);
	}

	return(ProductionQueues[id]);
}


int ProductionQueueClass::Count(void)
{
	One_Time();
	return(ProductionQueues.Count());
}


char const * ProductionQueueClass::Name_From_ID(ProductionQueueID id)
{
	if (id == PRODUCTION_QUEUE_NONE) {
		return("<none>");
	}

	ProductionQueueClass const * queue = From_ID(id);
	assert(queue != nullptr);
	return(queue != nullptr ? queue->IniName.c_str() : "<invalid>");
}


ProductionQueueID ProductionQueueClass::From_RTTI(RTTIType rtti)
{
	One_Time();

	switch (rtti) {
		case RTTI_AIRCRAFT:
		case RTTI_AIRCRAFTTYPE:
			return(PRODUCTION_QUEUE_AIRCRAFT);

		case RTTI_INFANTRY:
		case RTTI_INFANTRYTYPE:
			return(PRODUCTION_QUEUE_INFANTRY);

		case RTTI_UNIT:
		case RTTI_UNITTYPE:
			return(PRODUCTION_QUEUE_UNIT);

		case RTTI_BUILDING:
		case RTTI_BUILDINGTYPE:
			return(PRODUCTION_QUEUE_BUILDING);

		default:
			return(PRODUCTION_QUEUE_NONE);
	}
}


void ProductionQueueClass::Register_Product(ProductionQueueID queue, TechnoTypeClass const * type)
{
	ProductionQueueClass * entry = From_ID(queue);
	if (entry != nullptr && type != nullptr) {
		if (entry->ProductTypes.ID(type) == -1) {
			entry->ProductTypes.Add(type);
		}
	}
}


void ProductionQueueClass::Unregister_Product(ProductionQueueID queue, TechnoTypeClass const * type)
{
	ProductionQueueClass * entry = From_ID(queue);
	if (entry != nullptr && type != nullptr) {
		const_cast<ProductionQueueClass *>(entry)->ProductTypes.Delete(type);
	}
}


bool ProductionQueueClass::Queue_Supports_RTTI(ProductionQueueID queue, RTTIType rtti)
{
	ProductionQueueClass const * entry = From_ID(queue);
	if (entry == nullptr) {
		return(false);
	}

	ProductionQueueID const category = From_RTTI(rtti);
	if (category == PRODUCTION_QUEUE_NONE) {
		return(false);
	}

	for (int index = 0; index < entry->ProductTypes.Count(); index++) {
		if (From_RTTI(entry->ProductTypes[index]->RTTI) == category) {
			return(true);
		}
	}

	return(false);
}
