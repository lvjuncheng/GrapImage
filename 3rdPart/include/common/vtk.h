/***************************************************
 * 定义于vtk输出相关的函数
 * \author 黄劲 
 ***************************************************/

#ifndef HJ_VTK_H_
#define HJ_VTK_H_ 



template <typename OS, typename FLOAT, typename INT>
void line2vtk(
    OS &os,
    const FLOAT *node, size_t node_num,
    const INT *line, size_t line_num)
{
  os << "# vtk DataFile Version 2.0\nTRI\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";

  os<< "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";

  os << "CELLS " << line_num << " " << line_num*3 << "\n";
  for(size_t i = 0; i < line_num; ++i)
    os << 2 << " " << line[i*2+0] << " " << line[i*2+1] << "\n";

  os << "CELL_TYPES " << line_num << "\n";
  for(size_t i = 0; i < line_num; ++i)
    os << 3 << "\n";
}

template <typename OS,typename FLOAT, typename INT>
void point2vtk(OS &os,
               const FLOAT *node, size_t node_num,
               const INT *points, size_t points_num)
{
  os << "# vtk DataFile Version 2.0\nTRI\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";

  os<< "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";

  os << "CELLS " << points_num << " " << points_num*2 << "\n";
  for(size_t i = 0; i < points_num; ++i)
    os << 1 << " " << points[i] << "\n";

  os << "CELL_TYPES " << points_num << "\n";
  for(size_t i = 0; i < points_num; ++i)
    os << 1 << "\n";
}

template <typename OS, typename FLOAT, typename INT>
void tri2vtk(
    OS &os,
    const FLOAT *node, size_t node_num,
    const INT *tri, size_t tri_num)
{
  os << "# vtk DataFile Version 2.0\nTRI\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";

  os<< "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";

  os << "CELLS " << tri_num << " " << tri_num*4 << "\n";
  for(size_t i = 0; i < tri_num; ++i)
    os << 3 << "  " << tri[i*3+0] << " " << tri[i*3+1] << " " << tri[i*3+2] << "\n";
  os << "CELL_TYPES " << tri_num << "\n";
  for(size_t i = 0; i < tri_num; ++i)
    os << 5 << "\n";
}

template <typename OS, typename FLOAT, typename INT>
void vox2vtk(
    OS &os,
    const FLOAT *node, size_t node_num,
    const INT *vox, size_t vox_num)
{
  os << "# vtk DataFile Version 2.0\nTRI\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";

  os<< "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";

  os << "CELLS " << vox_num << " " << vox_num*9 << "\n";
  for(size_t i = 0; i < vox_num; ++i)
  {
    os << 8;
    for(size_t j = 0; j < 8; ++j)
    {
      os << "  " << vox[i*8+j];
    }
    os << "\n";
  }
  os << "CELL_TYPES " << vox_num << "\n";
  for(size_t i = 0; i < vox_num; ++i)
    os << 11 << "\n";
}

template <typename OS, typename FLOAT, typename INT>
void tet2vtk(
    OS &os,
    const FLOAT *node, size_t node_num,
    const INT *tet, size_t tet_num)
{
  os << "# vtk DataFile Version 2.0\nTET\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";
  os << "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";

  os << "CELLS " << tet_num << " " << tet_num*5 << "\n";
  for(size_t i = 0; i < tet_num; ++i)
    os << 4 << "  "
       << tet[i*4+0] << " " << tet[i*4+1] << " "
       << tet[i*4+2] << " " << tet[i*4+3] << "\n";
  os << "CELL_TYPES " << tet_num << "\n";
  for(size_t i = 0; i < tet_num; ++i)
    os << 10 << "\n";
}

template <typename OS, typename Iterator, typename INT>
void vtk_data(OS &os, Iterator first, INT size, char *value_name, char *table_name = "my_table")
{
  os << "SCALARS " << value_name << " floata\nLOOKUP_TABLE " << table_name << "\n";
  for(size_t i = 0; i < size; ++i, ++first)
    os << *first << "\n";
}

template <typename OS, typename Iterator, typename INT>
void vtk_data_rgba(OS &os, Iterator first, INT size, char *value_name, char *table_name = "my_table")
{
  os << "COLOR_SCALARS " << value_name << " 4\n";//\nLOOKUP_TABLE " << table_name << "\n";
  for(size_t i = 0; i < size; ++i)
  {
    for(size_t j = 0; j < 4; ++j,++first)
    {
      if(j != 3)
        os << *first << " ";
      else
        os << *first;
    }
    os << "\n";
  }
}
template <typename OS, typename Iterator, typename INT>
void point_data(OS &os, Iterator first, INT size, char *value_name, char *table_name = "my_table")
{
  os << "POINT_DATA " << size << "\n";
  vtk_data(os, first, size, value_name, table_name);
}

template <typename OS, typename Iterator, typename INT>
void cell_data(OS &os, Iterator first, INT size, char *value_name, char *table_name = "my_table")
{
  os << "CELL_DATA " << size << "\n";
  vtk_data(os, first, size, value_name, table_name);
}

template <typename OS, typename Iterator, typename INT>
void cell_data_rgba(OS &os, Iterator first, INT size, char *value_name, char *table_name = "my_table")
{
  os << "CELL_DATA " << size << "\n";
  vtk_data_rgba(os, first, size, value_name, table_name);
}

template <typename OS, typename Iterator, typename INT>
void cell_data_rgba_and_scalar(OS &os, Iterator rgba_first, Iterator scalar_first, INT size,
                               char *rgba_value_name, char *scalar_value_name,
                               char *table_name = "my_table")
{
  os << "CELL_DATA " << size << "\n";
  vtk_data_rgba(os, rgba_first, size, rgba_value_name, table_name);
  vtk_data(os, scalar_first, size, scalar_value_name, table_name);

}
#endif
