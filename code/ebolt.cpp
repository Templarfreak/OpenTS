/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Vinifera (src/new/ebolt.cpp,
 * https://github.com/Vinifera-Developers/Vinifera), SPDX-License-Identifier:
 * GPL-3.0-or-later, Copyright (c) 2020-2026 Vinifera contributors.
 * Ported to OpenTS and modified by OpenTS contributors, 2026.
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "ebolt.h"

#include "_rect.h"
#include "_rules.h"
#include "_surface.h"
#include "_tactica.h"
#include "ccrand.h"
#include "dbgprint.h"
#include "dsurface.h"
#include "globals.h"
#include "inline.h"
#include "partsys.h"
#include "point.h"
#include "rules.h"
#include "sun.h"
#include "tactical.h"
#include "techno.h"
#include "weapon.h"
#include "xsurface.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iterator>


DynamicVectorClass<EBoltClass *> EBoltClass::EBolts;


/// <summary>
/// Class constructor.
/// </summary>
EBoltClass::EBoltClass(void) :
	StartCoord(),
	EndCoord(),
	ZAdjust(0),
	Deviation(EBOLT_DEFAULT_DEVIATION),
	Source(NULL),
	Weapon(NULL),
	WeaponSlot(0),
	Lifetime(EBOLT_DEFAULT_LIFETIME),
	IterationCount(EBOLT_DEFAULT_ITERATIONS),
	LineColor1(EBOLT_DEFAULT_COLOR_1),
	LineColor2(EBOLT_DEFAULT_COLOR_2),
	LineColor3(EBOLT_DEFAULT_COLOR_3),
	LineSegmentCount(EBOLT_DEFAULT_LINE_SEGMENTS),
	LineDrawList(),
	DrawFrame(-1)
{
}


/// <summary>
/// Class destructor. Removes the bolt from the master list and detaches it from its source.
/// </summary>
EBoltClass::~EBoltClass(void)
{
	EBolts.Delete(this);
	Clear();
}


/// <summary>
/// Detaches the electric bolt from its source object.
/// </summary>
void EBoltClass::Clear(void)
{
	Source = NULL;
	LineDrawList.Clear();
}


/// <summary>
/// Draws the electric bolt to the screen, replotting it once per game frame.
/// This routine may be called several times within the same frame by different render
/// passes; only the first such call replots the bolt's line segments (and counts against its
/// lifetime), every further call that frame just redraws the cached segments.
/// </summary>
void EBoltClass::Draw_It(void)
{
	if (DrawFrame == Frame) {

		/*
		 * This is still our current draw frame, so just redraw what was already plotted.
		 */
		if (LineDrawList.Count()) {
			Draw_Bolts();
		}

	} else {

		/*
		 * Starting a new frame's worth of lines -- clear the previous set before plotting.
		 */
		LineDrawList.Clear();

		for (int i = 0; i < IterationCount; ++i) {
			if (Lifetime) {

				Point2D pixel_start;
				Point2D pixel_end;

				TacticalMap->Coord_To_Pixel(StartCoord, pixel_start);
				TacticalMap->Coord_To_Pixel(EndCoord, pixel_end);

				if (Clip_Line_To_Rect(pixel_start, pixel_end, TacticalRect)) {
					Plot_Bolt(StartCoord, EndCoord);
				}
			}
		}

		/*
		 * Draw the freshly plotted set of lines.
		 */
		if (LineDrawList.Count()) {
			Draw_Bolts();
		}

		--Lifetime;

		DrawFrame = Frame;
	}
}


/// <summary>
/// Creates the electric bolt between the two coordinates specified and adds it to the master
/// list. A spark particle is spawned at the destination coordinate to mark the impact.
/// </summary>
/// <param name="start">The world coordinate the bolt originates from.</param>
/// <param name="end">The world coordinate the bolt strikes.</param>
/// <param name="zadjust">The depth bias applied to the bolt's starting end.</param>
void EBoltClass::Create(Coord const & start, Coord const & end, int zadjust)
{
	StartCoord = start;
	EndCoord = end;

	ZAdjust = zadjust;

	EBolts.Add(this);

	/*
	 * Spawn a spark particle at the destination of the electric bolt.
	 */
	if (Rule->DefaultSparkSystem != NULL) {
		ParticleSystemClass * particlesys = new ParticleSystemClass(Rule->DefaultSparkSystem, end);
		assert(particlesys != NULL);
	}
}


/// <summary>
/// Fetches the current fire coordinate of the object that fired this bolt.
/// </summary>
/// <returns>Returns with the fire coordinate of Source, or COORD_NONE if this bolt has no
/// source object (or it has since gone away).</returns>
Coord EBoltClass::Source_Coord(void) const
{
	Coord coord = COORD_NONE;
	if (Source != NULL) {
		coord = Source->Fire_Coord(WeaponSlot);
	}
	return(coord);
}


/// <summary>
/// Assigns the firing source object and weapon for this electric bolt, and copies the visual
/// properties (colors, lifetime, shape) from that weapon's INI-defined overrides.
/// </summary>
/// <param name="source">The object that fired this bolt.</param>
/// <param name="weapon">The weapon type that fired this bolt.</param>
/// <param name="which">Which weapon slot fired this bolt (0=primary, 1=secondary).</param>
void EBoltClass::Set_Properties(TechnoClass * source, WeaponTypeClass const * weapon, int which)
{
	if (source != NULL && weapon != NULL) {
		if (source->IsActive && !source->IsInLimbo) {
			Source = source;
			Weapon = weapon;
			WeaponSlot = which;

			/*
			 * Copy the color and shape overrides from the firing object's weapon.
			 */
			LineColor1 = weapon->ElectricBoltColor1;
			LineColor2 = weapon->ElectricBoltColor2;
			LineColor3 = weapon->ElectricBoltColor3;
			IterationCount = weapon->ElectricBoltIterationCount;
			LineSegmentCount = weapon->ElectricBoltSegmentCount;
			Lifetime = Sim_Random_Pick(weapon->ElectricBoltLifetime, weapon->ElectricBoltMaxLifetime);
			Deviation = weapon->ElectricBoltDeviation;
		}
	}
}


/// <summary>
/// Processes every active electric bolt for this frame: re-anchors each bolt's start point to
/// its (still living) source, draws it, and retires any bolt whose source has left the game
/// world or whose lifetime has run out.
/// </summary>
/// <remarks>This is the only per-frame entry point a bolt needs; unlike LaserDrawClass, there
/// is no separate AI/Update_All pass -- Draw_It's own frame-tracking handles that.</remarks>
void EBoltClass::Draw_All(void)
{
	for (int i = EBolts.Count() - 1; i >= 0; --i) {
		EBoltClass * ebolt = EBolts[i];
		if (ebolt == NULL) {
			DebugString("Invalid EBolt in EBolts list!\n");
			continue;
		}

		/*
		 * If the source object has left the game world, retire this bolt.
		 */
		if (ebolt->Source != NULL && (!ebolt->Source->IsActive || ebolt->Source->IsInLimbo)) {
			delete ebolt;
			continue;
		}

		/*
		 * Re-anchor the start coordinate to the source's current fire coordinate.
		 */
		Coord coord = ebolt->Source_Coord();
		if (coord != COORD_NONE) {
			ebolt->StartCoord = coord;
		}

		ebolt->Draw_It();

		/*
		 * The electric bolt has expired, retire it.
		 */
		if (ebolt->Lifetime <= 0) {
			delete ebolt;
		}
	}
}


/// <summary>
/// Removes every electric bolt from the game world. Used when tearing down a scenario so that
/// no bolt survives into the next game.
/// </summary>
void EBoltClass::Clear_All(void)
{
	while (EBolts.Count()) {
		delete EBolts[0];
	}
}


/// <summary>
/// Plots the complete electric bolt from source to target, recursively subdividing the line
/// between them and jittering each new midpoint to give the bolt its jagged, "electric" look.
/// </summary>
void EBoltClass::Plot_Bolt(Coord const & start, Coord const & end)
{
	struct EBoltPlotStruct
	{
		Coord StartCoords[EBOLT_SEGMENT_LINE_COUNT];
		Coord EndCoords[EBOLT_SEGMENT_LINE_COUNT];
		int Distance;
		int Deviation;
		int StartZ;
		int EndZ;
	};

	int const SEGMENT_COORDS_SIZE = sizeof(Coord) * EBOLT_SEGMENT_LINE_COUNT;

	VectorClass<EBoltPlotStruct> ebolt_plots(LineSegmentCount);

	Coord start_coords[EBOLT_SEGMENT_LINE_COUNT];
	Coord end_coords[EBOLT_SEGMENT_LINE_COUNT];
	Coord working_coords[EBOLT_SEGMENT_LINE_COUNT];

	int deviation_values[6];

	bool init_deviation_values = true;
	int plot_index = 0;

	/*
	 * Make sure there is actual distance between the two coordinates before plotting.
	 */
	int distance = Distance(start, end);
	if (distance) {

		for (int i = 0; i < EBOLT_SEGMENT_LINE_COUNT; ++i) {
			end_coords[i] = end;
			start_coords[i] = start;
		}

		int line_start_z = ZAdjust;
		int line_end_z = 0;

		int dist_a = (102 * distance / CELL_LEPTON_W);

		/*
		 * Max distance from the line center, with Deviation as the delta.
		 */
		int desired_deviation = 23;
		int line_deviation = (int)((desired_deviation * Deviation) * distance / CELL_LEPTON_W);

		while (true) {

			while (distance > (CELL_LEPTON_W / 4) && plot_index < ebolt_plots.Length()) {

				for (int i = 0; i < EBOLT_SEGMENT_LINE_COUNT; ++i) {
					working_coords[i].X = (end_coords[i].X + start_coords[i].X) / 2;
					working_coords[i].Y = (end_coords[i].Y + start_coords[i].Y) / 2;
					working_coords[i].Z = (end_coords[i].Z + start_coords[i].Z) / 2;
				}

				/*
				 * Initialize the line deviation values.
				 */
				if (init_deviation_values) {

					for (int i = 0; i < (int)std::size(deviation_values); ++i) {
						deviation_values[i] = (int)(std::sin((double)Sim_Random_Pick(0, 256) * M_PI / (double)(i + 7)) * (double)line_deviation);
					}

					for (int i = 0; i < EBOLT_SEGMENT_LINE_COUNT; ++i) {
						working_coords[i].X += deviation_values[0] + deviation_values[3];
						working_coords[i].Y += deviation_values[1] + deviation_values[5];
						working_coords[i].Z += (deviation_values[2] + deviation_values[4] + 2 * line_deviation) / 2;
					}

					init_deviation_values = false;
				}

				if (distance <= (CELL_LEPTON_W / 2)) {
					working_coords[0].X += 2 * line_deviation * Sim_Random_Pick(-1, 1);
					working_coords[0].Y += 2 * line_deviation * Sim_Random_Pick(-1, 1);
					working_coords[0].Z += 2 * line_deviation * Sim_Random_Pick(-1, 1);
				} else {
					working_coords[0].X += Sim_Random_Pick(-line_deviation, line_deviation);
					working_coords[0].Y += Sim_Random_Pick(-line_deviation, line_deviation);
					working_coords[0].Z += Sim_Random_Pick(-line_deviation, line_deviation);
				}

				if (distance > dist_a) {
					for (int i = 1; i < EBOLT_SEGMENT_LINE_COUNT; ++i) {
						working_coords[i].X = working_coords[0].X + (Sim_Random_Pick(-line_deviation, line_deviation) / 2);
						working_coords[i].Y = working_coords[0].Y + (Sim_Random_Pick(-line_deviation, line_deviation) / 2);
						working_coords[i].Z = working_coords[0].Z + (Sim_Random_Pick(-line_deviation, line_deviation) / 2);
					}

				} else {
					for (int i = 1; i < EBOLT_SEGMENT_LINE_COUNT; ++i) {
						working_coords[i].X += Sim_Random_Pick(-line_deviation, line_deviation);
						working_coords[i].Y += Sim_Random_Pick(-line_deviation, line_deviation);
						working_coords[i].Z += Sim_Random_Pick(-line_deviation, line_deviation);
					}
				}

				line_deviation /= 2;
				distance /= 2;

				EBoltPlotStruct & plot = ebolt_plots[plot_index];

				std::memcpy(plot.StartCoords, working_coords, SEGMENT_COORDS_SIZE);
				std::memcpy(plot.EndCoords, end_coords, SEGMENT_COORDS_SIZE);
				std::memcpy(end_coords, working_coords, SEGMENT_COORDS_SIZE);

				plot.Distance = distance;
				plot.Deviation = line_deviation;
				plot.StartZ = (line_end_z + line_start_z) / 2;
				plot.EndZ = line_end_z;

				line_end_z = (line_end_z + line_start_z) / 2;

				++plot_index;
			}

			/*
			 * Add these line segments to the draw list.
			 */
			Add_Plot_Line(start_coords[1], end_coords[1], LineColor2, line_start_z, line_end_z);
			Add_Plot_Line(start_coords[2], end_coords[2], LineColor3, line_start_z, line_end_z);
			Add_Plot_Line(start_coords[0], end_coords[0], LineColor1, line_start_z, line_end_z);

			if (--plot_index < 0) {
				break;
			}

			EBoltPlotStruct & plot = ebolt_plots[plot_index];

			distance = plot.Distance;
			line_deviation = plot.Deviation;
			line_start_z = plot.StartZ;
			line_end_z = plot.EndZ;

			std::memcpy(start_coords, plot.StartCoords, SEGMENT_COORDS_SIZE);
			std::memcpy(end_coords, plot.EndCoords, SEGMENT_COORDS_SIZE);
		}
	}
}


/// <summary>
/// Draws every pending line segment plotted for this bolt onto the game surface.
/// </summary>
void EBoltClass::Draw_Bolts(void)
{
	for (int i = 0; i < LineDrawList.Count(); ++i) {
		LineDrawDataStruct & data = LineDrawList[i];

		Point2D start_pixel;
		Point2D end_pixel;

		TacticalMap->Coord_To_Pixel(data.Start, start_pixel);
		TacticalMap->Coord_To_Pixel(data.End, end_pixel);

		int start_z = data.StartZ - TacticalMap->Z_Lepton_To_Pixel(data.Start.Z) - 2;
		int end_z = data.EndZ - TacticalMap->Z_Lepton_To_Pixel(data.End.Z) - 2;

		unsigned color = DSurface::Build_Hicolor_Pixel(data.Color);

		LogicalSurface->Draw_Depth_Shaded_Line(TacticalRect, start_pixel, end_pixel, color, start_z, end_z);
	}
}
