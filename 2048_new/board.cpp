#include "board.h"
board::board() : m_fields{ }
{
	for (LONG row = 0; row < rows; ++row)
		for (LONG column = 0; column < columns; ++column)
		{
			auto& f = m_fields[row * columns + column];
			f.position.top =
				row * (field_size + margin) + margin;
			f.position.left =
				column * (field_size + margin) + margin;
			f.position.bottom = f.position.top + field_size;
			f.position.right = f.position.left + field_size;
		}
}