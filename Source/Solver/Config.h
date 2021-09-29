#pragma once

enum class RunType
{
    FLUID, RIGIDBODY, UNIVERSITY_CUDA
};

const RunType G_RUN_TYPE = RunType::FLUID;

const int G_WINDOW_WIDTH = 1024;
const int G_WINDOW_HEIGHT = 768;

const float G_CAMERA_X = 8;
const float G_CAMERA_Y = 5;
const float G_CAMERA_Z = 8;

const float G_TARGET_X = 0;
const float G_TARGET_Y = 0;
const float G_TARGET_Z = 0;