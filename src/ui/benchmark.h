#pragma once
#include <chrono> 
#include <iostream>

long long speedmeter = 0;

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
