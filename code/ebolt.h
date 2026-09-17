/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Vinifera (src/new/ebolt.h,
 * https://github.com/Vinifera-Developers/Vinifera), SPDX-License-Identifier:
 * GPL-3.0-or-later, Copyright (c) 2020-2026 Vinifera contributors.
 * Ported to OpenTS and modified by OpenTS contributors, 2026.
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

#include "coord.h"
#include "rgb.h"
#include "vector.h"

class TechnoClass;
class WeaponTypeClass;


/*
 * Default values for the drawing of electric bolts. A weapon may override each of these
 * through its EBolt* INI keys; see WeaponTypeClass.
 */
#define EBOLT_DEFAULT_DEVIATION         1.0f
#define EBOLT_DEFAULT_ITERATIONS        1
#define EBOLT_DEFAULT_LINE_SEGMENTS     8
#define EBOLT_SEGMENT_LINE_COUNT        3
#define EBOLT_DEFAULT_LIFETIME          17
#define EBOLT_DEFAULT_COLOR_1           RGBClass(255, 255, 255)   // White
#define EBOLT_DEFAULT_COLOR_2           RGBClass(82, 81, 255)     // Dark blue
#define EBOLT_DEFAULT_COLOR_3           RGBClass(82, 81, 255)     // Dark blue


/*
 * Draws a jagged, animated electric bolt between two points in the world, such as the beam a
 * Tesla Coil or Tesla Trooper zaps its target with.
 *
 * Unlike LaserDrawClass, a bolt tracks the object that fired it: its start point is re-anchored
 * to that object's current fire coordinate every frame for as long as the object stays alive,
 * so the beam stays attached even while the source moves or its turret rotates. Once its
 * source is gone, or once its lifetime runs out, the bolt retires itself.
 */
class EBoltClass
{
	public:
		EBoltClass(void);
		~EBoltClass(void);

		void Draw_It(void);
		void Create(Coord const & start, Coord const & end, int zadjust);

		Coord Source_Coord(void) const;
		void Set_Properties(TechnoClass * source, WeaponTypeClass const * weapon, int which);

		void Flag_To_Delete(void) { Lifetime = 0; }

		static void Draw_All(void);
		static void Clear_All(void);

	private:
		void Clear(void);

		void Add_Plot_Line(Coord const & start, Coord const & end, RGBClass const & color, int start_z, int end_z)
		{
			LineDrawList.Add(LineDrawDataStruct { start, end, color, start_z, end_z });
		}

		void Plot_Bolt(Coord const & start, Coord const & end);
		void Draw_Bolts(void);

	private:
		/*
		 * The world-space start and end coordinates of this bolt. Start is re-anchored to
		 * Source's current fire coordinate every frame that Source is still alive.
		 */
		Coord StartCoord;
		Coord EndCoord;

		/*
		 * The depth bias applied to the bolt's starting end, for the same reason
		 * LaserDrawClass::ZAdjust exists on a laser beam.
		 */
		int ZAdjust;

		/*
		 * How far a plotted point may stray from the straight line between its neighbors. The
		 * higher this is, the wilder and more "electric" the bolt looks.
		 */
		float Deviation;

		/*
		 * The object that fired (or otherwise created) this bolt. While Source is alive, the
		 * bolt's start point follows its current fire coordinate; once Source is gone, or has
		 * gone into limbo, the bolt is retired.
		 */
		TechnoClass * Source;

		/*
		 * The weapon this bolt was fired from, if any, and which weapon slot fired it
		 * (0=primary, 1=secondary). Used to keep re-fetching the correct fire coordinate from
		 * Source each frame.
		 */
		WeaponTypeClass const * Weapon;
		int WeaponSlot;

		/*
		 * How many more times this bolt should be replotted before it expires. This is
		 * decremented once per unique game frame that the bolt is replotted on, not once per
		 * Draw_It call -- Draw_It may be called several times in a single frame by multiple
		 * render passes, and only the first such call should count against the bolt's life.
		 */
		int Lifetime;

		/*
		 * How many independently-jittered bolts should be plotted and drawn between the two
		 * endpoints at once.
		 */
		int IterationCount;

		/*
		 * The colors of the bolt's three parallel line segments, copied from the firing
		 * weapon on creation.
		 */
		RGBClass LineColor1;
		RGBClass LineColor2;
		RGBClass LineColor3;

		/*
		 * How many times the bolt is recursively subdivided when plotted -- more segments make
		 * for a longer, more detailed bolt.
		 */
		int LineSegmentCount;

		/*
		 * One pending line segment to draw, plotted fresh each time the bolt is replotted.
		 */
		struct LineDrawDataStruct
		{
			Coord Start;
			Coord End;
			RGBClass Color;
			int StartZ;
			int EndZ;
		};

		/*
		 * The line segments plotted this frame. Draw_Bolts redraws this same list on every
		 * Draw_It call until the bolt is next replotted.
		 */
		DynamicVectorClass<LineDrawDataStruct> LineDrawList;

		/*
		 * The game frame this bolt was last (re)plotted on. Draw_It replots the bolt only once
		 * per frame and simply redraws the cached line segments on any further calls made that
		 * same frame.
		 */
		int DrawFrame;

		/*
		 * The master list of every electric bolt that is currently active. A bolt adds itself
		 * here when it is created and removes itself when it is deleted.
		 */
		static DynamicVectorClass<EBoltClass *> EBolts;
};
