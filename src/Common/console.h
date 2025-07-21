#pragma once

#include <iostream>
#include <functional>
#include "lut.h"

namespace dsry::console
{
    #define CH6(x) ((x) * 6 / 255)
    #define CH24(x) ((x) * 24 / 255)
    #define MOD(name,...) class name : public raw_mod { public: name () : raw_mod( __VA_ARGS__ ) {} }

    constexpr size_t MaxCodes = 4;

    class raw_mod 
    {
    public:
        raw_mod(int code) : n_codes(1) 
        {
            m_code[0] = code;
        }

        raw_mod(int code0, int code1) : n_codes(2)
        {
            m_code[0] = code0;
            m_code[1] = code1;
        }

        raw_mod(int code0, int code1, int code2) : n_codes(3)
        {
            m_code[0] = code0;
            m_code[1] = code1;
            m_code[2] = code2;
        }

        raw_mod(int code0, int code1, int code2, int code3) : n_codes(4)
        {
            m_code[0] = code0;
            m_code[1] = code1;
            m_code[2] = code2;
            m_code[3] = code3;
        }

        int m_code[MaxCodes];
        size_t n_codes;
       
        friend std::ostream& operator<<(std::ostream& os, const raw_mod& mod) 
        {
            os << "\033[";

            for (size_t i = 0; i < mod.n_codes; i++) {
                os << mod.m_code[i];
                if(i != mod.n_codes - 1)
                    os << ";";
            }

            return os << "m";
        }
    };

    class foreground : public raw_mod
    {
    public:
        foreground(int r, int g, int b) :
            raw_mod(38, 5, 16 + 36 * CH6(r) + 6 * CH6(g) + CH6(b))
        { }

        foreground(int gray) :
            raw_mod(38, 5, 232 + CH24(gray))
        {
        }
    };

    class background : public raw_mod
    {
    public:
        background(int r, int g, int b) :
            raw_mod(48, 5, 16 + 36 * CH6(r) + 6 * CH6(g) + CH6(b))
        {
        }

        background(int gray) :
            raw_mod(48, 5, 232 + CH24(gray))
        {
        }
    };

    MOD(reset, 0);
    MOD(green, 32);
    MOD(red, 31);
    MOD(yellow, 33);

    class table_writer
    {
    public:
        table_writer(void);

        table_writer(int cols, int rows) 
            : table_writer() 
        {
            m_cols = cols;
            m_rows = rows;
        }

    protected:
        int m_cols, m_rows;
        int m_staticWidth, m_rowNameWidth;
        int m_padding0, m_padding1;

        std::function<void(std::ostream& os, int, int)> m_celltext;
        std::function<void(std::ostream& os, int, int)> m_cellformat;
        std::function<std::string(int)> m_colname;
        std::function<std::string(int)> m_rowname;

        int get_width(int idx);
        void write_fixed(std::ostream& os, std::string_view s, int fixed_width);
        void write_padding(std::ostream& os, int n);

    public:
        void write(std::ostream& os);

        void set_data_matrix(const float* data, int row_stride, int width, int precision);
        void set_colormap(const float* data, int row_stride, const dsry::color::lut* lut, float v0, float v1);
        void set_column_width(int w);
        void set_padding(int before, int after);

    };
};