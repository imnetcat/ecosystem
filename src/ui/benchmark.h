#pragma once
#include <chrono> 
#include <iostream>
#include <fstream>

static long long speedmeter = 0;
static long long speedmeter2 = 0;

/*
#define MYBENCHMARK(func, label) { \
	auto start = std::chrono::high_resolution_clock::now(); \
	func; \
	auto stop = std::chrono::high_resolution_clock::now(); \
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
	if (duration.count() > speedmeter) \
	{ \
		speedmeter = duration.count(); \
		std::cout << label << ": \t" << speedmeter << " ms\r"; \
	} \
}
*/

#define MYBENCHMARK(func, label) { \
	auto start = std::chrono::high_resolution_clock::now(); \
		func; \
		auto stop = std::chrono::high_resolution_clock::now(); \
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
		if (duration.count() > speedmeter) \
		{ \
			speedmeter = duration.count(); \
			std::ofstream myfile; \
			myfile.open("log.txt", ios::app); \
			myfile << label << ": " << speedmeter << " ms\r"; \
		} \
}

#define MYBENCHMARK2(func, label) { \
	auto start = std::chrono::high_resolution_clock::now(); \
		func; \
		auto stop = std::chrono::high_resolution_clock::now(); \
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
		if (duration.count() > speedmeter2) \
		{ \
			speedmeter2 = duration.count(); \
			std::ofstream myfile; \
			myfile.open("log.txt", ios::app); \
			myfile << label << ": " << speedmeter2 << " ms\r"; \
		} \
}
