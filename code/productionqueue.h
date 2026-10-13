/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "rtti.hh"
#include "vector.h"

#include <string>

class TechnoTypeClass;
class SaveStreamClass;

using ProductionQueueID = int;

inline constexpr ProductionQueueID PRODUCTION_QUEUE_NONE = -1;
inline constexpr ProductionQueueID PRODUCTION_QUEUE_AIRCRAFT = 0;
inline constexpr ProductionQueueID PRODUCTION_QUEUE_INFANTRY = 1;
inline constexpr ProductionQueueID PRODUCTION_QUEUE_UNIT = 2;
inline constexpr ProductionQueueID PRODUCTION_QUEUE_BUILDING = 3;
inline constexpr int PRODUCTION_QUEUE_CUSTOM_LIMIT = 64;
inline constexpr int PRODUCTION_QUEUE_COUNT = PRODUCTION_QUEUE_BUILDING + 1 + PRODUCTION_QUEUE_CUSTOM_LIMIT;

class ProductionQueueClass
{
	public:
		explicit ProductionQueueClass(char const * name);

		ProductionQueueID ID;
		std::string IniName;
		DynamicVectorClass<TechnoTypeClass const *> ProductTypes;

		static void One_Time(void);
		static void Reset(void);
		static bool Save(SaveStreamClass & stream);
		static bool Load(SaveStreamClass & stream);
		static void Rebuild_Products(void);
		static ProductionQueueClass * Find_Or_Make(char const * name);
		static int Count(void);
		static ProductionQueueClass * From_ID(ProductionQueueID id);
		static char const * Name_From_ID(ProductionQueueID id);
		static ProductionQueueID From_RTTI(RTTIType rtti);
		static void Register_Product(ProductionQueueID queue, TechnoTypeClass const * type);
		static void Unregister_Product(ProductionQueueID queue, TechnoTypeClass const * type);
		static bool Queue_Supports_RTTI(ProductionQueueID queue, RTTIType rtti);
};
