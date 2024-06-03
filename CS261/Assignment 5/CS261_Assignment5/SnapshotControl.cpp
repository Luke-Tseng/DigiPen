//---------------------------------------------------------
// file:	DeadReckoningControl.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates the position as *interpolated* from the last two known positions
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "SnapshotControl.h"
#include <cmath>


void SnapshotControl::AddSnapshot(const State& new_state, const u_long remote_frame)
{
	sync_ratio_.base_frame = sync_ratio_.target_frame;
	sync_ratio_.target_frame = remote_frame;
	sync_ratio_.t = 0.0f;

	previous_state_ = latest_state_;
	latest_state_ = new_state;

	if (is_initialized_ == false)
	{
		current_x_ = previous_state_.x = latest_state_.x;
		current_y_ = previous_state_.y = latest_state_.y;
		is_initialized_ = true;
	}

	// intialize the angle when more than one state
	if (latest_state_.x == previous_state_.x && latest_state_.y == previous_state_.y)
	{
		current_angle_ = atan2(previous_state_.y - latest_state_.y, previous_state_.x - latest_state_.x) + M_PI;
	}

	if (new_state.time_since_last_update_secs <= 0.0f)
	{
		current_x_ = previous_state_.x;
		current_y_ = previous_state_.y;
	}

	current_x_ = previous_state_.x;
	current_y_ = previous_state_.y;
	previous_state_.angle = current_angle_;
}


float SnapshotControl::CalculateX(const float t, const float ratio) const
{
	const auto clamped_t = CP_Math_ClampFloat(t, 0.0f, 1.0f);
	if (ratio == -1.0f)
	{
		return previous_state_.x + clamped_t * (latest_state_.x - previous_state_.x);
	}
	// distance between current and latest state
	float distance = sqrt(pow(latest_state_.x - previous_state_.x, 2) + pow(latest_state_.y - previous_state_.y, 2));

	return current_x_ + ((distance * ratio) * cos(current_angle_));
}


float SnapshotControl::CalculateY(const float t, const float ratio) const
{
	const auto clamped_t = CP_Math_ClampFloat(t, 0.0f, 1.0f);
	if (ratio == -1.0f)
	{
		return previous_state_.y + clamped_t * (latest_state_.y - previous_state_.y);
	}

	// distance between current and latest state
	float distance = sqrt(pow(latest_state_.x - previous_state_.x, 2) + pow(latest_state_.y - previous_state_.y, 2));
	
	return current_y_ + ((distance * ratio) * sin(current_angle_));

}


void SnapshotControl::Update(const float dt)
{

	if (is_initialized_ == false)
	{
		return;
	}

	if (latest_state_.time_since_last_update_secs <= 0.0f)
	{
		return;
	}

	// simple idea: we should move from 0 - 1 in bias in the same amount of time it took for the object to arrive
	sync_ratio_.t += dt / latest_state_.time_since_last_update_secs;

	
	float angle = atan2(current_y_ - latest_state_.y, current_x_ - latest_state_.x) + M_PI;
	
	// shift angle based on current angle and new angle
	current_angle_ = current_angle_ + (angle - previous_state_.angle) * dt / latest_state_.time_since_last_update_secs;
	
	current_x_ = CalculateX(sync_ratio_.t, (dt / latest_state_.time_since_last_update_secs));
	current_y_ = CalculateY(sync_ratio_.t, (dt / latest_state_.time_since_last_update_secs));
}


void SnapshotControl::Draw()
{
	CP_Settings_Stroke(CP_Color_Create(0, 255, 255, 255));
	CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
	CP_Graphics_DrawCircle(latest_state_.x, latest_state_.y, 15);
	CP_Graphics_DrawCircle(previous_state_.x, previous_state_.y, 15);
	CP_Graphics_DrawLine(latest_state_.x, latest_state_.y, previous_state_.x, previous_state_.y);
	CP_Graphics_DrawLine(current_x_, current_y_, previous_state_.x, previous_state_.y);
	// Draw line in direction of angle
	CP_Graphics_DrawLine(current_x_, current_y_, current_x_ + 50 * cos(current_angle_), current_y_ + 50 * sin(current_angle_));
}