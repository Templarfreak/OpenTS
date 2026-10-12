/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

#include "always.h"

#include "teleport.h"

#include "foot.h"
#include "globals.h"
#include "savestream.h"
#include "dbgprint.h"
#include "_map.h"
#include "cell.h"
#include "building.h"
#include "anim.h"
#include "rules.h"
#include "ccrand.h"
#include "unit.h"
#include "unittype.h"

#include "_rules.h"

#include <algorithm>

bool TeleportLocomotionClass::Should_Act_Normal(void) const
{
	if (LinkedTo->RTTI != RTTI_UNIT) {
		return(false);
	}

	if (GroundLocomotion->Is_Moving() || GroundLocomotion->Is_Moving_Now() || GroundLocomotion->Is_Surfacing()) {
		return(true);
	}

	if (LinkedTo->IsInLimbo) {
		return(true);
	}

	UnitClass const * unit = (UnitClass const *)LinkedTo;

	if (!unit->Class->TeleportDocking) {
		return(false);
	}
	if (LinkedTo->CurrentMission != MISSION_ENTER) {
		return(true);
	}
	TechnoClass * contact = LinkedTo->Contact_With_Whom();
	if (contact == NULL) {
		contact = dynamic_cast<TechnoClass *>(LinkedTo->ArchiveTarget);
	}
	BuildingClass * dock = NULL;
	if (contact != NULL && contact->RTTI == RTTI_BUILDING) {
		dock = (BuildingClass *)contact;
	}
	if (dock != NULL && dock->Class->IsDockUnload) {
		return(false);
	}
	return(true);
}

void TeleportLocomotionClass::Ensure_Ground_Locomotion(ClassID const loco)
{
	if (GroundLocomotion == nullptr) {
		GroundLocomotion = Create_Locomotor(loco);
		GroundLocomotion->Link_To_Object(LinkedTo);
	}
}
void TeleportLocomotionClass::Object_Linked(void)
{
	Ensure_Ground_Locomotion(LinkedTo->TClass->TeleportDockingLocomotor);
}

// forward to inner locomotor because we dont do anything in these functions but the inner locomotor may need to

void TeleportLocomotionClass::Do_Turn(DirType coord)
{
	GroundLocomotion->Do_Turn(coord);
}
void TeleportLocomotionClass::Force_New_Slope(int ramp)
{
	GroundLocomotion->Force_New_Slope(ramp);
}
void TeleportLocomotionClass::Tilt_Pitch_AI(void)
{
	GroundLocomotion->Tilt_Pitch_AI();
}
Matrix3D TeleportLocomotionClass::Draw_Matrix(int *key)
{
	return GroundLocomotion->Draw_Matrix(key);
}
MoveType TeleportLocomotionClass::Can_Enter_Cell(Cell cell)
{
	return GroundLocomotion->Can_Enter_Cell(cell);
}
Coord TeleportLocomotionClass::Head_To_Coord(void)
{
	return GroundLocomotion->Head_To_Coord();
}
bool TeleportLocomotionClass::Is_Surfacing()
{
	return GroundLocomotion->Is_Surfacing();
}
bool TeleportLocomotionClass::Is_Powered(void)
{
	return GroundLocomotion->Is_Powered();
}
bool TeleportLocomotionClass::Is_Ion_Sensitive(void)
{
	return GroundLocomotion->Is_Ion_Sensitive();
}
bool TeleportLocomotionClass::Push(DirType dir)
{
	return GroundLocomotion->Push(dir);
}
bool TeleportLocomotionClass::Shove(DirType dir)
{
	return GroundLocomotion->Shove(dir);
}
bool TeleportLocomotionClass::Is_Moving_Now(void)
{
	return GroundLocomotion->Is_Really_Moving_Now();
}
void TeleportLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
	return GroundLocomotion->Mark_All_Occupation_Bits(mark);
}
bool TeleportLocomotionClass::Is_Moving_Here(Coord to)
{
	return GroundLocomotion->Is_Moving_Here(to);
}

bool TeleportLocomotionClass::Is_To_Have_Shadow(void)
{
	return GroundLocomotion->Is_To_Have_Shadow();
}
Matrix3D TeleportLocomotionClass::Shadow_Matrix(int *key)
{
	return GroundLocomotion->Shadow_Matrix(key);
}
Point2D TeleportLocomotionClass::Draw_Point(void)
{
	return GroundLocomotion->Draw_Point();
}
Point2D TeleportLocomotionClass::Shadow_Point(void)
{
	return GroundLocomotion->Shadow_Point();
}
int TeleportLocomotionClass::Z_Adjust(void)
{
	return GroundLocomotion->Z_Adjust();
}
ZGradientType TeleportLocomotionClass::Z_Gradient(void)
{
	return GroundLocomotion->Z_Gradient();
}
void TeleportLocomotionClass::Unlimbo(void)
{
	return GroundLocomotion->Unlimbo();
}
bool TeleportLocomotionClass::Power_On(void)
{
	return GroundLocomotion->Power_On();
}
void TeleportLocomotionClass::Force_Track(int track, Coord coord)
{
	return GroundLocomotion->Force_Track(track, coord);
}
void TeleportLocomotionClass::Force_Immediate_Destination(Coord coord)
{
	return GroundLocomotion->Force_Immediate_Destination(coord);
}
int TeleportLocomotionClass::Apparent_Speed(void)
{
	return GroundLocomotion->Apparent_Speed();
}
int TeleportLocomotionClass::Drawing_Code(void)
{
	return GroundLocomotion->Drawing_Code();
}
FireErrorType TeleportLocomotionClass::Can_Fire(void)
{
	return GroundLocomotion->Can_Fire();
}
int TeleportLocomotionClass::Get_Status()
{
	return GroundLocomotion->Get_Status();
}
void TeleportLocomotionClass::Acquire_Hunter_Seeker_Target(void)
{
	return GroundLocomotion->Acquire_Hunter_Seeker_Target();
}
bool TeleportLocomotionClass::Will_Jump_Tracks(void)
{
	return GroundLocomotion->Will_Jump_Tracks();
}
bool TeleportLocomotionClass::Is_Really_Moving_Now(void)
{
	return GroundLocomotion->Is_Really_Moving_Now();
}
void TeleportLocomotionClass::Stop_Movement_Animation(void)
{
	return GroundLocomotion->Stop_Movement_Animation();
}
void TeleportLocomotionClass::Lock(void)
{
	return GroundLocomotion->Lock();
}
void TeleportLocomotionClass::Unlock(void)
{
	return GroundLocomotion->Unlock();
}
int TeleportLocomotionClass::Get_Track_Number(void)
{
	return GroundLocomotion->Get_Track_Number();
}
int TeleportLocomotionClass::Get_Track_Index(void)
{
	return GroundLocomotion->Get_Track_Index();
}
int TeleportLocomotionClass::Get_Speed_Accum(void)
{
	return GroundLocomotion->Get_Speed_Accum();
}
bool TeleportLocomotionClass::Power_Off(void)
{
	return GroundLocomotion->Power_Off();
}

/// <summary>
/// Creates a teleport locomotor.
/// The locomotor starts out idle, with no destination waiting to be jumped to.
/// </summary>
TeleportLocomotionClass::TeleportLocomotionClass(void) :
	BASECLASS(),
	DestinationCoord(COORD_NONE)
{
}


/// <summary>
/// Is a teleport pending?
/// The object counts as moving from the moment a destination is handed to this
/// locomotor until the jump has actually been made.
/// </summary>
bool TeleportLocomotionClass::Is_Moving(void)
{
	if (Should_Act_Normal()) {
		return GroundLocomotion->Is_Moving();
	} else {
		if (DestinationCoord != COORD_NONE && !teleported) {
			return(true);
		}
		return(false);
	}
}


/// <summary>
/// Is the object standing still?
/// This is the plain opposite of Is_Moving. An object with a teleport ordered counts as
/// being on the move even though it has not gone anywhere yet.
/// </summary>
bool TeleportLocomotionClass::Is_Stationary(void)
{
	if (Is_Moving() == false) {
		return(true);
	}
	return(false);
}


/// <summary>
/// Fetches the location this locomotor is bound for.
/// </summary>
/// <returns>Returns with the pending teleport destination, or with the object's current
/// position if no teleport has been ordered.</returns>
Coord TeleportLocomotionClass::Destination(void)
{
	if (Should_Act_Normal()) {
		return GroundLocomotion->Destination();
	} else {
		if (Is_Moving()) {
			return(DestinationCoord);
		}
		return(LinkedTo->PositionCoord);
	}

}


/// <summary>
/// Orders the object to teleport to the location specified.
/// The jump is not made here. It happens the next time this locomotor is processed.
/// </summary>
/// <param name="to">The coordinate to teleport the object to.</param>
void TeleportLocomotionClass::Move_To(Coord to)
{
	if (Should_Act_Normal()) {
		GroundLocomotion->Move_To(to);
	} else {
		DestinationCoord = to;
	}
}


/// <summary>
/// Cancels any teleport that has been ordered.
/// The pending destination is forgotten, so the object stays where it is rather than
/// making the jump.
/// </summary>
void TeleportLocomotionClass::Stop_Moving(void)
{
	if (Should_Act_Normal()) {
		GroundLocomotion->Stop_Moving();
	} else {
		DestinationCoord = COORD_NONE;
	}
}

/// <summary>
/// Performs any pending teleport.
/// This routine is called by the owning object's movement processing. The object is
/// lifted off the map, set down at its destination, and made to look around from where
/// it now stands. The whole journey is over by the time this routine returns.
/// </summary>
/// <returns>bool; Is there more movement still to process? A teleport never leaves any.</returns>
bool TeleportLocomotionClass::Process(void)
{
	if (Should_Act_Normal()) {
		return GroundLocomotion->Process();
	} else {
		if (Is_Moving() && !LinkedTo->Is_Immobilized()) {
			Coord spot = COORD_NONE;

			if (LinkedTo->What_Am_I() == RTTI_INFANTRY) {
				spot = Map[DestinationCoord].Closest_Free_Spot(DestinationCoord, false, Map[DestinationCoord].IsUnderBridge);
				if (spot == COORD_NONE) {
					Cell fallback = Map.Nearby_Location((Cell)DestinationCoord, LinkedTo->TClass->Speed);
					if (fallback != CELL_NONE) {
						spot = Map[fallback].Closest_Free_Spot(fallback.As_Coord(), false, Map[fallback].IsUnderBridge);
					}
				}
			}
			else {
				bool blocked = Map[DestinationCoord].Flag.Occupy.Vehicle
				|| (Map[DestinationCoord].Flag.Occupy.Monolith
					&& (!Map[DestinationCoord].Get_Gate() || !Map[DestinationCoord].Get_Gate()->Is_Gate_Open()));
				if (blocked) {
					Cell fallback = Map.Nearby_Location((Cell)DestinationCoord, LinkedTo->TClass->Speed);
					if (fallback != CELL_NONE) {
						spot = fallback.As_Coord();
					}
				} else {
					spot = DestinationCoord;
				}
			}
			if (spot == COORD_NONE) {
				return(false);
			}
			if (LinkedTo->TClass->ChronoDistanceFactor > 0) {
				int dist = Point2D(LinkedTo->PositionCoord).Distance_To(Point2D(spot));
				int stun_duration = dist / LinkedTo->TClass->ChronoDistanceFactor;
				stun_duration = std::max(LinkedTo->TClass->ChronoMinimumDelay, stun_duration);

				if (LinkedTo->StunDuration < stun_duration) {
					if (LinkedTo->RTTI == RTTI_UNIT) {
						UnitClass const * unit = (UnitClass const *)LinkedTo;
						if (!unit->Class->TeleportDocking) {
							LinkedTo->StunDuration = stun_duration;
						}
					} else {
						LinkedTo->StunDuration = stun_duration;
					}
				}
			}
			if (LinkedTo->TClass->TeleportInEffect != NULL) {
				new AnimClass(LinkedTo->TClass->TeleportInEffect, LinkedTo->PositionCoord);
			}
			if (LinkedTo->TClass->TeleportOutEffect != NULL) {
				new AnimClass(LinkedTo->TClass->TeleportInEffect, spot);
			}
			if (LinkedTo->TClass->TeleportInSound != VOC_NONE) {
				Sound_Effect(LinkedTo->TClass->TeleportInSound, LinkedTo->PositionCoord);
			}
			if (LinkedTo->TClass->TeleportOutSound != VOC_NONE) {
				Sound_Effect(LinkedTo->TClass->TeleportOutSound, spot);
			}
			if (LinkedTo->TClass->TeleportSparks != NULL && LinkedTo->StunDuration > 0) {
				AnimClass * sparks = new AnimClass(LinkedTo->TClass->TeleportSparks, LinkedTo->Center_Coord(), LinkedTo->StunDuration);
				if (sparks != NULL) {
					sparks->Attach_To(LinkedTo);
				}
			}
			LinkedTo->Clear_Occupy_Bit(LinkedTo->PositionCoord);
			LinkedTo->Mark(MARK_UP);
			LinkedTo->PositionCoord = spot;
			LinkedTo->Set_Occupy_Bit(spot);
			LinkedTo->Mark(MARK_DOWN);
			Stop_Moving();
			LinkedTo->Assign_Destination(NULL);
			LinkedTo->Per_Cell_Process(PCP_END);
			LinkedTo->Look();
			teleported = true;
		}
		else {
			if (teleported && LinkedTo->StunDuration <= 0) {
				teleported = false;
			}
		}
		return(false);
	}
}

VisualType TeleportLocomotionClass::Visual_Character(bool raw)
{
	if (Should_Act_Normal()) {
		GroundLocomotion->Visual_Character(raw);
	} else {
		if (LinkedTo->StunDuration > 0) {
			return(VISUAL_INDISTINCT);
		}
	}

	return(VISUAL_NORMAL);
}


ClassID TeleportLocomotionClass::Class_ID(void) const
{
	return(ClassID_TeleportLocomotion);
}


/// <summary>
/// Lists the members this teleport locomotor carries.
/// </summary>
/// <param name="stream">The stream carrying the members.</param>
void TeleportLocomotionClass::Serialize(SaveStreamClass & stream)
{
	BASECLASS::Serialize(stream);
	GroundLocomotion->Serialize(stream);

	stream.Serialize(DestinationCoord);
}


/// <summary>
/// Fetches the display layer that the owning object belongs in.
/// A teleporting object is always on the ground. It never travels through the air on
/// the way to its destination, so it never rises out of the ground layer.
/// </summary>
/// <returns>Returns with the layer the object should be rendered in.</returns>
LayerType TeleportLocomotionClass::In_Which_Layer(void)
{
	if (Should_Act_Normal()) {
		return GroundLocomotion->In_Which_Layer();
	} else {
		return(LAYER_GROUND);
	}
}
