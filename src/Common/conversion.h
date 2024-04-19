#pragma once

namespace dsry::conversion
{
	void f64_to_f32(float* dest, const double* src, size_t length);
	void f32_to_f64(double* dest, const float* src, size_t length);
};