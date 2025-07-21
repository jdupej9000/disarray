#include "lut.h"
#include <algorithm>
#include "color.h"

using namespace std;

namespace dsry::color
{
    const lut_stop LUTSTOPS_FAST[] = {
        { 0.00f, make_rgba(14, 14, 20) },
        { 0.17f, make_rgba(62, 117, 207) },
        { 0.30f, make_rgba(91, 190, 243) },
        { 0.43f, make_rgba(175, 237, 234) },
        { 0.50f, make_rgba(229, 241, 196) },
        { 0.59f, make_rgba(224, 213, 130) },
        { 0.71f, make_rgba(137, 158, 80) },
        { 0.85f, make_rgba(204, 90, 41) },
        { 1.00f, make_rgba(150, 20, 30) }
    };

    const lut_stop LUTSTOPS_JET[] = {
        { 0.00f, make_rgba(0,0,127) },
        { 0.10f, make_rgba(0,0,229) },
        { 0.20f, make_rgba(0,76,255) },
        { 0.30f, make_rgba(0,178,255) },
        { 0.40f, make_rgba(25,255,229) },
        { 0.50f, make_rgba(127,255,127) },
        { 0.60f, make_rgba(229,255,25) },
        { 0.70f, make_rgba(255,178,0) },
        { 0.80f, make_rgba(255,76,0) },
        { 0.90f, make_rgba(229,0,0) },
        { 1.00f, make_rgba(127,0,0)}
    };

    const lut_stop LUTSTOPS_BLACKBODY[] = {
        { 0.00f, make_rgba(0, 0, 0) },
        { 0.39f, make_rgba(178, 34, 34) },
        { 0.58f, make_rgba(227, 105, 5) },
        { 0.89f, make_rgba(230, 230, 53) },
        { 1.00f, make_rgba(255, 255, 255)}
    };


    lut::lut(void) :
        m_data(nullptr), m_width(0)
    {
    }

    lut::lut(size_t width) :
        m_width(width)
    {
        m_data = new uint32_t[width];
    }

    lut::lut(lut&& other) :
        m_data(other.m_data),
        m_width(other.m_width)
    {
        other.m_data = nullptr;
    }

    lut::~lut(void)
    {
        if (m_data != nullptr)
            delete[] m_data;
    }

    uint32_t lut::sample(float t) const noexcept
    {
        if (t <= 0.0f)
            return m_data[0];

        if (t >= 1.0f)
            return m_data[m_width - 1];

        size_t idx = (m_width - 1) * t;
        return m_data[idx];
    }

    lut create_color_lut(size_t width, size_t num_stops, const lut_stop* stops)
    {
        lut ret{ width };
        uint32_t* data = ret.ptr();

        for (size_t s = 0; s < num_stops - 1; s++) {
            const lut_stop* s0 = stops + s;
            const lut_stop* s1 = stops + s + 1;
            size_t i0 = min(width, (size_t)(width * s0->pos));
            size_t i1 = min(width, (size_t)(width * s1->pos));

            for (size_t i = i0; i < i1; i++) {
                data[i] = lerp_rgba8(s0->color, s1->color,
                    255 * (i - i0) / (i1 - i0));
            }
        }

        if (num_stops > 0) {
            const lut_stop* last = stops + num_stops - 1;
            size_t ilast = min(width, (size_t)(width * last->pos));

            for (size_t i = ilast; i < width; i++)
                data[i] = last->color;
        }

        return ret;
    }
};