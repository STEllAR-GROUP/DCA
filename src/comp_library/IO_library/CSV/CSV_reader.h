//-*-C++-*-

#ifndef CSV_READER_HEADER_H
#define CSV_READER_HEADER_H

namespace IO
{
  
  /*!
   * \author Peter Staar
   */
  template<>
  class reader<IO::CSV>
  {
  public:

    template<typename scalartype>
    static void execute(std::string file_name,  std::vector<std::vector<scalartype> >& data);
  };
  
  template<typename scalartype>
  void reader<IO::CSV>::execute(std::string file_name,  std::vector<std::vector<scalartype> >& data)
  {
    std::filebuf fb;
    
    if(fb.open(file_name.c_str(),std::ios::in))
      {
	std::istream myfile(&fb);
    
	std::string row;
	
	data.resize(0);
	while(getline(myfile, row))
	  {
	    data.push_back(vector<scalartype>());
	    
	    istringstream tokenS(row);
	    string token;
	    
	    while(getline(tokenS, token, ','))
	      {
		std::istringstream valueS(token);
		
		valueS.imbue(myfile.getloc());
		
		scalartype value;
		
		if(valueS >> value)
		  data.back().push_back(value);
	      }
	  }

	fb.close();
      }
    else
      {
	std::cout << "\n\n\t " << file_name << " can not be read !!! \n\n\t";
	throw std::logic_error(__FUNCTION__);
      }
  }

}

#endif