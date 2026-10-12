/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "loco.h"


class TeleportLocomotionClass : public LocomotionClass
{
		typedef LocomotionClass BASECLASS;

	public:
		std::unique_ptr<ILocomotion> GroundLocomotion;

		TeleportLocomotionClass(void);

		void Ensure_Ground_Locomotion(ClassID const);
		bool Should_Act_Normal(void) const;

		virtual ClassID Class_ID(void) const override;
		virtual void Serialize(SaveStreamClass & stream) override;
		virtual bool Is_Moving(void) override;
		virtual Coord Destination(void) override;
		virtual void Move_To(Coord to) override;
		virtual void Stop_Moving(void) override;
		virtual bool Process(void) override;
		virtual LayerType In_Which_Layer(void) override;
		virtual VisualType Visual_Character(bool) override;
		virtual void Object_Linked(void) override;
		virtual void Do_Turn(DirType coord) override;

		// forwarded calls
		virtual Coord Head_To_Coord(void) override;
		virtual Matrix3D Draw_Matrix(int *key) override;
		virtual bool Power_Off(void) override;
		virtual bool Is_Powered(void) override;
		virtual bool Is_Ion_Sensitive(void) override;
		virtual bool Push(DirType dir) override;
		virtual bool Shove(DirType dir) override;
		virtual bool Is_Moving_Now(void) override;
		virtual void Mark_All_Occupation_Bits(int mark) override;
		virtual bool Is_Moving_Here(Coord to) override;
		virtual bool Is_Surfacing() override;
		virtual MoveType Can_Enter_Cell(Cell cell) override;
		virtual void Tilt_Pitch_AI(void) override;
		virtual void Force_New_Slope(int ramp) override;
		virtual bool Is_To_Have_Shadow(void) override;
		virtual Matrix3D Shadow_Matrix(int *key) override;
		virtual Point2D Draw_Point(void) override;
		virtual Point2D Shadow_Point(void) override;
		virtual int Z_Adjust(void) override;
		virtual ZGradientType Z_Gradient(void) override;
		virtual void Unlimbo(void) override;
		virtual bool Power_On(void) override;
		virtual void Force_Track(int track, Coord coord) override;
		virtual void Force_Immediate_Destination(Coord coord) override;
		virtual int Apparent_Speed(void) override;
		virtual int Drawing_Code(void) override;
		virtual FireErrorType Can_Fire(void) override;
		virtual int Get_Status() override;
		virtual void Acquire_Hunter_Seeker_Target(void) override;
		virtual bool Will_Jump_Tracks(void) override;
		virtual bool Is_Really_Moving_Now(void) override;
		virtual void Stop_Movement_Animation(void) override;
		virtual void Lock(void) override;
		virtual void Unlock(void) override;
		virtual int Get_Track_Number(void) override;
		virtual int Get_Track_Index(void) override;
		virtual int Get_Speed_Accum(void) override;

		virtual bool Is_Stationary(void);

	private:
		/*
		 * This is the coordinate that the object will be set down at the next time this
		 * locomotor is processed. If COORD_NONE, then no teleport is pending -- there is
		 * no separate moving flag, so this doubles as one.
		 */
		Coord DestinationCoord;
		bool teleported;
};
