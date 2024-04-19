#pragma once

#include <stdint.h>

namespace dsry::memory
{
	enum class chunk_type : uint32_t
	{
		f32,
		f64
	};

	struct chunk_info
	{
		void* data;
		size_t length; // in units of type
		chunk_type type;
		chunk_type stream_type;

		size_t stream_offset;
	};


	size_t make_layout(chunk_info* info, size_t count, size_t align);

	void pack_chunk(void* stream, const chunk_info* info);
	void pack_all_chunks(void* stream, const chunk_info* info, size_t count);
	void unpack_chunk(const chunk_info* info, const void* stream);
	void unpack_all_chunks(const chunk_info* info, size_t count, const void* stream);
};