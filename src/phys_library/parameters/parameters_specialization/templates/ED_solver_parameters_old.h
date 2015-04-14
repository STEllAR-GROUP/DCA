//-*-C++-*-

#ifndef ED_SOLVER_PARAMETERS_H
#define ED_SOLVER_PARAMETERS_H

/*!
 *   \author Peter Staar
 */
class ED_solver_parameters
{
public:

  ED_solver_parameters();
  ~ED_solver_parameters();

  /******************************************
   ***        CONCURRENCY                 ***
   ******************************************/

  template<class concurrency_type>
  int  get_buffer_size( concurrency_type& concurrency);

  template<class concurrency_type>
  void pack           ( concurrency_type& concurrency, int* buffer, int buffer_size, int& position);

  template<class concurrency_type>
  void unpack         ( concurrency_type& concurrency, int* buffer, int buffer_size, int& position);

  /******************************************
   ***        READ/WRITE                  ***
   ******************************************/

  template<class read_write_type>
  void read_write(read_write_type& read_write_obj);

  /******************************************
   ***        DATA                        ***
   ******************************************/

  double get_eigenvalue_cut_off();

private:

  double eigenvalue_cut_off;
};

ED_solver_parameters::ED_solver_parameters():
  eigenvalue_cut_off(1.e-6)
{}

ED_solver_parameters::~ED_solver_parameters()
{}

/******************************************
 ***        CONCURRENCY                 ***
 ******************************************/

template<class concurrency_type>
int ED_solver_parameters::get_buffer_size(concurrency_type& concurrency)
{
  int buffer_size = 0;

  buffer_size += concurrency.get_buffer_size(eigenvalue_cut_off);

  return buffer_size;
}

template<class concurrency_type>
void ED_solver_parameters::pack(concurrency_type& concurrency, int* buffer, int buffer_size, int& position)
{
  concurrency.pack(buffer, buffer_size, position, eigenvalue_cut_off);
}

template<class concurrency_type>
void ED_solver_parameters::unpack(concurrency_type& concurrency, int* buffer, int buffer_size, int& position)
{
  concurrency.unpack(buffer, buffer_size, position, eigenvalue_cut_off);
}

/******************************************
 ***        READ/WRITE                  ***
 ******************************************/

template<class read_write_type>
void ED_solver_parameters::read_write(read_write_type& read_write_obj)
{
  try
    {
      read_write_obj.open_group("ED-solver-parameters");

      try { read_write_obj.execute("eigenvalue-cut-off", eigenvalue_cut_off); } catch(const std::exception& r_e) {}

      read_write_obj.close_group();
    }
  catch(const std::exception& r_e)
    {}
}

double ED_solver_parameters::get_eigenvalue_cut_off()
{
  return eigenvalue_cut_off;
}

#endif