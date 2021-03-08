#pragma once
const int ENVIRONMENT_SIZE_X = 80;
const int ENVIRONMENT_SIZE_Y = 60;

const short OUTLINE = 1;
const short CELL_SIZE = 10;
const short CELL_OUTLINE = CELL_SIZE + OUTLINE;

const unsigned short LIGHT_POWER = 250;
const double LIGHT_COEF = 0.6;

const unsigned short MAX_MINERALS_TO_EAT = 500;
const unsigned short MAX_MEAT_TO_EAT = 1000;
const unsigned int MAX_ENERGY = 20000;
const unsigned int MAX_HP = 100;

const int CELL_START_COUNT = (ENVIRONMENT_SIZE_X * ENVIRONMENT_SIZE_Y) * 0.01;
