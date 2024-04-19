#include "packing.h"
#include "config.h"
#include "conversion.h"
#include "miscmath.h"
#include <memory>

namespace dsry::memory
{
	size_t chitem_size(chunk_type cht);
	void convert(void* dest, const void* src, size_t length, chunk_type dest_type, chunk_type src_type);

	size_t make_layout(chunk_info* info, size_t count, size_t align)
	{
		dsry_assert(dsry::math::is_power_of_two(align));

		size_t offset = 0;
		for (size_t i = 0; i < count; i++)
		{
			size_t chunk_stream_size = dsry::math::round_up(info->length * chitem_size(info->stream_type), align);
			info->stream_offset = offset;
			offset += chunk_stream_size;
		}

		return offset;
	}

	void pack_chunk(void* stream, const chunk_info* info)
	{
		if(info->data != nullptr)
			convert((uint8_t*)stream + info->stream_offset, info->data, info->length, info->stream_type, info->type);
	}

	void pack_all_chunks(void* stream, const chunk_info* info, size_t count)
	{
		for (size_t i = 0; i < count; i++)
			pack_chunk(stream, info + i);
	}

	void unpack_chunk(const chunk_info* info, const void* stream)
	{
		if(info->data != nullptr)
			convert(info->data, (const uint8_t*)stream + info->stream_offset, info->length, info->type, info->stream_type);
	}

	void unpack_all_chunks(const chunk_info* info, size_t count, const void* stream)
	{
		for (size_t i = 0; i < count; i++)
			unpack_chunk(info + i, stream);
	}


	size_t chitem_size(chunk_type cht)
	{
		switch (cht)
		{
		case chunk_type::f32: return sizeof(float);
		case chunk_type::f64: return sizeof(double);
		}

		return 0;
	}

	void convert(void* dest, const void* src, size_t length, chunk_type dest_type, chunk_type src_type)
	{
		if (src_type == dest_type)
		{
			memcpy(dest, src, length * chitem_size(dest_type));
		}
		else if (src_type == chunk_type::f32)
		{
			switch (dest_type)
			{
			case chunk_type::f64:
				dsry::conversion::f32_to_f64((double*)dest, (const float*)src, length);
				break;
			default:
				dsry_assert_notreached;
				break;
			}
		}
		else if (src_type == chunk_type::f64)
		{
			switch (dest_type)
			{
			case chunk_type::f32:
				dsry::conversion::f64_to_f32((float*)dest, (const double*)src, length);
				break;
			default:
				dsry_assert_notreached;
				break;
			}
		}
	}
};