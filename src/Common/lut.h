#pragma once
#include <stdint.h>

namespace dsry::color
{
    struct lut_stop
    {
        lut_stop(float p, uint32_t c) : color(c), pos(p)
        {
        }

        uint32_t color;
        float pos;
    };

    class lut
    {
    public:
        lut(void);
        lut(size_t width);
        lut(lut&& other);
        ~lut(void);

    private:
        uint32_t* m_data;
        size_t m_width;

    public:
        uint32_t* ptr(void) noexcept { return m_data; }
        bool is_valid(void) const noexcept { return m_data != nullptr; }
        size_t width(void) const noexcept { return m_width; }
        uint32_t sample(float t) const noexcept;
    };

    lut create_color_lut(size_t width, size_t num_stops, const lut_stop* stops);

    extern const lut_stop LUTSTOPS_FAST[];
    constexpr size_t NUMLUTSTOPS_FAST = 9;

    extern const lut_stop LUTSTOPS_JET[];
    constexpr size_t NUMLUTSTOPS_JET = 11;

    extern const lut_stop LUTSTOPS_BLACKBODY[];
    constexpr size_t NUMLUTSTOPS_BLACKBODY = 5;
};