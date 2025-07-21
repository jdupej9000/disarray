#include "console.h"
#include "color.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace dsry::console
{
	table_writer::table_writer(void) :
		m_staticWidth(-1),
		m_padding0(0),
		m_padding1(0),
		m_rowNameWidth(8)
	{
		m_celltext = [](ostream& os, int c, int r) {};
		m_cellformat = [](ostream& os, int c, int r) {};
		m_rowname = [](int x) { stringstream ss; ss << x; return ss.str(); };
		m_colname = m_rowname;
	}

	void table_writer::write(std::ostream& os)
	{
		os << reset();
		write_padding(os, m_padding0 + m_padding1 + m_rowNameWidth);
		for (int i = 0; i < m_cols; i++) {
			write_padding(os, m_padding0);
			write_fixed(os, m_colname(i), get_width(i));
			write_padding(os, m_padding1);
		}
		os << endl;

		for (int r = 0; r < m_rows; r++) {
			os << reset();
			write_fixed(os, m_rowname(r), m_rowNameWidth);

			for (int c = 0; c < m_cols; c++) {
				m_cellformat(os, c, r);
				write_padding(os, m_padding0);
				m_celltext(os, c, r);
				write_padding(os, m_padding1);
			}

			os << endl;
		}

		os << reset();
	}

	void table_writer::set_data_matrix(const float* data, int row_stride, int width, int precision)
	{
		m_celltext = [data, row_stride, width, precision](ostream& os, int c, int r)
			{
				os << fixed << setprecision(precision) << setw(width) << data[row_stride * r + c];
			};
	}

	void table_writer::set_colormap(const float* data, int row_stride, const dsry::color::lut* lut, float v0, float v1)
	{
		m_cellformat = [data, row_stride, lut, v0, v1](ostream& os, int c, int r)
			{
				uint32_t col = lut->sample((data[row_stride * r + c] - v0) / (v1 - v0));
				os << foreground(COLOR_TO_RGB(col));
			};
	}

	void table_writer::set_column_width(int w)
	{
		m_staticWidth = w;
	}

	void table_writer::set_padding(int before, int after)
	{
		m_padding0 = before;
		m_padding1 = after;
	}

	int table_writer::get_width(int idx)
	{
		if (m_staticWidth >= 0)
			return m_staticWidth;

		return 1;
	}

	void table_writer::write_fixed(ostream& os, string_view s, int fixed_width)
	{
		string_view clamped = s.substr(0, fixed_width);
		os << clamped;
		write_padding(os, fixed_width - clamped.length());
	}

	void table_writer::write_padding(ostream& os, int n)
	{
		if (n < 1)
			return;

		for (int i = 0; i < n; i++)
			os << " ";
	}
	
};