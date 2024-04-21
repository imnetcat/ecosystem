#pragma once

static int SPEED = 3;
static int MIN_SPEED = 1;
static int MAX_SPEED = 3;

static int ZOOM = 1;
static int MIN_ZOOM = 1;
static int MAX_ZOOM = 5;

static bool do_tic = false;
static bool pause = false;
static bool hibernate = false;

static unsigned int CELL_SIZE = 4;

static unsigned int WORLD_WIDTH = 100;
static unsigned int WORLD_HEIGHT = 51;
static unsigned short LIGHT_POWER = 2000;
static unsigned short MAX_ORGANIC_TO_EAT = 3000;
static unsigned short MAX_ENTITIES_TO_EAT = 6000;
static unsigned short MAX_ENERGY = 20000;
static unsigned short MAX_AGE = 100;

static unsigned int ticks = 0;
