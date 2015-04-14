//-*-C++-*-

#ifndef ANALYSIS_INTERPOLATION_H
#define ANALYSIS_INTERPOLATION_H

namespace dca {

  /*! 
   * \author peter staar
   */
  template<class parameter_type, class MOMS_type>
  class analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>
  {
#include "type_definitions.h"

    typedef double scalartype;

    const static int N_LAMBDAS = 10;
    typedef dmn_0<dmn<N_LAMBDAS, int> > lambda_dmn_type;

    const static int N_HARMONICS = 3;
    typedef dmn_0<dmn<N_HARMONICS, int> > harmonics_dmn_type;

    typedef dmn_0<brillouin_zone_path_domain<SQUARE_2D_LATTICE> > k_dmn_cut_type;
    //typedef dmn_0<brillouin_zone_path_domain<FERMI_SURFACE_SQUARE_2D_LATTICE> > k_dmn_cut_type;

    typedef typename parameter_type::profiler_type    profiler_t;
    typedef typename parameter_type::Concurrency_Type concurrency_t;

    typedef dmn_4<b,b,k_DCA ,w_VERTEX> cluster_eigenvector_dmn_t;
    typedef dmn_4<b,b,k_HOST,w_VERTEX> lattice_eigenvector_dmn_t;

    typedef dmn_2<dmn_4<b,b,k_DCA,w_VERTEX>,dmn_4<b,b,k_DCA,w_VERTEX> >   DCA_DCA_matrix_dmn_t;
    typedef dmn_2<dmn_4<b,b,k_HOST,w_VERTEX>,dmn_4<b,b,k_DCA,w_VERTEX> >  HOST_DCA_matrix_dmn_t;
    typedef dmn_2<dmn_4<b,b,k_DCA,w_VERTEX>,dmn_4<b,b,k_HOST,w_VERTEX> >  DCA_HOST_matrix_dmn_t;
    typedef dmn_2<dmn_4<b,b,k_HOST,w_VERTEX>,dmn_4<b,b,k_HOST,w_VERTEX> > HOST_HOST_matrix_dmn_t;

    typedef dmn_2<dmn_4<b,b,k_DCA ,w_VERTEX>,dmn_4<b,b,k_DCA ,w_VERTEX> > DCA_matrix_dmn_t;
    typedef dmn_2<dmn_4<b,b,k_HOST,w_VERTEX>,dmn_4<b,b,k_HOST,w_VERTEX> > HOST_matrix_dmn_t;

  public:
 
    analysis(parameter_type& parameters, MOMS_type& MOMS);
    ~analysis();

    template<class stream_type>
    void to_JSON(stream_type& ss);

    void calculate_cuts();

    void calculate_susceptibilities();

  private:

    void initialize_wave_functions();

    void apply_symmetries();
    
    void load_the_matrices();

    void compute_Gamma_cluster();

    void compute_chi_0();

    void compute_Gamma_lattice();

    void diagonolize_Gamma_times_chi_0();

    void diagonolize_Gamma_times_chi_0_general();
    void diagonolize_Gamma_times_chi_0_symmetric();

    void compute_chi(eigensystem_plan<std::complex<scalartype>, GENERAL>& eigensystem_pln);

    void find_harmonic_expansion();

    void write_on_shell();

  private:

    parameter_type&  parameters;
    concurrency_t&   concurrency;
    MOMS_type&       MOMS;

    cluster_eigenvector_dmn_t cluster_eigenvector_dmn;
    lattice_eigenvector_dmn_t lattice_eigenvector_dmn;

    diagrammatic_symmetries<parameter_type> diagrammatic_symmetries_obj;

    FUNC_LIB::function<std::string             ,               harmonics_dmn_type>   wave_functions_names;
    FUNC_LIB::function<std::complex<scalartype>, dmn_2<k_HOST, harmonics_dmn_type> > harmonics;

    FUNC_LIB::function<std::complex<double>, DCA_matrix_dmn_t> G4;
    FUNC_LIB::function<std::complex<double>, DCA_matrix_dmn_t> G4_0;

    FUNC_LIB::function<std::complex<scalartype>, DCA_matrix_dmn_t > Gamma_cluster;
    FUNC_LIB::function<std::complex<scalartype>, HOST_matrix_dmn_t> Gamma_lattice;
    
    FUNC_LIB::function<std::complex<scalartype>, HOST_matrix_dmn_t> chi;
    FUNC_LIB::function<std::complex<scalartype>, HOST_matrix_dmn_t> chi_0;

    FUNC_LIB::function<std::complex<scalartype>, dmn_0<dmn<1, int> > > chi_q;

    FUNC_LIB::function<std::complex<scalartype>,       lambda_dmn_type>                              leading_eigenvalues;
    FUNC_LIB::function<std::complex<scalartype>, dmn_2<lambda_dmn_type, harmonics_dmn_type> >        leading_symmetries;
    FUNC_LIB::function<std::complex<scalartype>, dmn_2<lambda_dmn_type, lattice_eigenvector_dmn_t> > leading_eigenvectors;

    FUNC_LIB::function<std::complex<scalartype>, k_dmn_cut_type> S_k_cut;
    FUNC_LIB::function<std::complex<scalartype>, k_dmn_cut_type> a_k_cut;
  };

  template<class parameter_type, class MOMS_type>
  analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::analysis(parameter_type& parameters_in, MOMS_type& MOMS_in):
    parameters(parameters_in),
    concurrency(parameters.get_concurrency()),
    MOMS(MOMS_in),

    cluster_eigenvector_dmn(),
    lattice_eigenvector_dmn(),
    
    //eigensystem_pln(lattice_eigenvector_dmn.get_size()),
    diagrammatic_symmetries_obj(parameters),

    harmonics("harmonics"),

    G4("G4"),
    G4_0("G4_0"),

    Gamma_cluster("Gamma_cluster"),
    Gamma_lattice("Gamma_lattice"),

    chi("chi"),
    chi_0("chi_0"),

    chi_q("chi_q"),

    leading_eigenvalues("leading_eigenvalues"),
    leading_symmetries("leading_symmetries"),
    leading_eigenvectors("leading_eigenvectors"),
    
    S_k_cut("S_k_cut"),
    a_k_cut("a_k_cut")
  {
    parameters.get_output_file_name() = parameters.get_output_susceptibilities_file_name();

    initialize_wave_functions();

    {
      profiler_t prof("compute-H(k)", "input", __LINE__);
      
      wannier_interpolation<k_LDA, k_DCA >::execute(MOMS.H_LDA, MOMS.H_DCA);
      wannier_interpolation<k_LDA, k_HOST>::execute(MOMS.H_LDA, MOMS.H_HOST);
    }
    
    {
      profiler_t prof("compute-band-structure", "input", __LINE__);    
      compute_band_structure::execute(parameters, MOMS.H_LDA, MOMS.band_structure);
    }
    
    {
      profiler_t prof("initialize-coarsegraining", "input", __LINE__);
      MOMS.initialize_coarsegraining();
    }
  }

  template<class parameter_type, class MOMS_type>
  analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::~analysis()
  {}

  template<class parameter_type, class MOMS_type>
  template<class stream_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::to_JSON(stream_type& ss)
  {    
    leading_eigenvalues.to_JSON(ss);
    ss << ",\n";

    leading_symmetries.to_JSON(ss);
    ss << ",\n";

    if(true)
      {
	MOMS.compute_Sigma_bands();

	MOMS.band_structure.to_JSON(ss);
	ss << ",";
	
	MOMS.Sigma_band_structure.to_JSON(ss);
	ss << ",";
	
	MOMS.Sigma_cluster_band_structure.to_JSON(ss);
	ss << ",";
	
	MOMS.Sigma_lattice_band_structure.to_JSON(ss);
	ss << ",";
	
	MOMS.Sigma_band_structure_interpolated.to_JSON(ss);
	ss << ",";
	
	MOMS.Sigma_band_structure_coarsegrained.to_JSON(ss);
	ss << ",";
      }

    if(true)
      {
	FUNC_LIB::function<std::complex<scalartype>, cluster_eigenvector_dmn_t> tmp("chi_0_function");
	
	for(int i=0; i<tmp.size(); ++i)
	  tmp(i) = G4_0(i,i);
	
	tmp.to_JSON(ss);
	ss << ",\n";
      }

    if(true)
      {
	FUNC_LIB::function<std::complex<scalartype>, lattice_eigenvector_dmn_t> tmp("full_chi_0_function");
	
	for(int i=0; i<tmp.size(); ++i)
	  tmp(i) = chi_0(i,i);
	
	tmp.to_JSON(ss);
	ss << ",\n";
      }
    
    leading_eigenvectors.to_JSON(ss);
  }

  
  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::initialize_wave_functions()
  {
    wave_functions_names = "no-name";

    wave_functions_names(0) = "s-wave";
    wave_functions_names(1) = "p-wave";
    wave_functions_names(2) = "d-wave";

    //wave_functions_names(1) = "cos(kx)+cos(ky)";
    //wave_functions_names(2) = "cos(kx)-cos(ky)";

    {// s-wave
      std::complex<scalartype> norm_psi=0;

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 0) = 1.;

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	norm_psi += harmonics(k_ind, 0)*conj(harmonics(k_ind, 0));
      
      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 0) /= std::sqrt(norm_psi);
    }

    {// p-wave
      std::complex<scalartype> norm_psi=0;

      scalartype alpha_x = host_cluster_type::get_r_basis()[0][0];
      scalartype alpha_y = host_cluster_type::get_r_basis()[1][1];

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 1) = (cos(alpha_x*k_HOST::get_elements()[k_ind][0]) 
			       + cos(alpha_y*k_HOST::get_elements()[k_ind][1]));

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	norm_psi += harmonics(k_ind, 1)*conj(harmonics(k_ind, 1));

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 1) /= std::sqrt(norm_psi);
    }

    {// d-wave
      std::complex<scalartype> norm_psi=0;

      scalartype alpha_x = host_cluster_type::get_r_basis()[0][0];
      scalartype alpha_y = host_cluster_type::get_r_basis()[1][1];

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 2) = (cos(alpha_x*k_HOST::get_elements()[k_ind][0]) 
			       - cos(alpha_y*k_HOST::get_elements()[k_ind][1]));

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	norm_psi += harmonics(k_ind, 2)*conj(harmonics(k_ind, 2));

      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, 2) /= std::sqrt(norm_psi);
    }
  }
  
  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::calculate_susceptibilities()
  {
    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl;

    apply_symmetries();

    load_the_matrices();

    compute_Gamma_cluster();

    compute_Gamma_lattice();

    compute_chi_0();
      
    if(concurrency.id()==concurrency.last())
      diagonolize_Gamma_times_chi_0();
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::apply_symmetries()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl << endl;

    symmetrize::execute(MOMS.Sigma, MOMS.H_symmetry);

    symmetrize::execute(MOMS.G_k_w, MOMS.H_symmetry);
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::load_the_matrices()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl << endl;

    make_G4_matrix  <parameter_type, MOMS_type>::execute(MOMS.G4_k_k_w_w, G4);

    make_G4_0_matrix<parameter_type, MOMS_type>::execute(parameters, MOMS.G_k_w, G4_0);

  if(parameters.do_symmetrization_of_Gamma())
    {
      if(true){
	concurrency << "symmetrize Gamma_lattice according to the symmetry-group \n\n";     

	symmetrize::execute(G4  , parameters.get_q_vector());
	symmetrize::execute(G4_0, parameters.get_q_vector());
      }
      
      if(true){
	concurrency << "symmetrize Gamma_lattice according to diagrammatic symmetries \n\n";     

	diagrammatic_symmetries<parameter_type> diagrammatic_symmetries_obj(parameters);

	diagrammatic_symmetries_obj.execute(G4  );
	diagrammatic_symmetries_obj.execute(G4_0);
      }        
    }
  }
    
  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::compute_Gamma_cluster()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl << endl;

    scalartype renorm = 1./(parameters.get_beta()*k_DCA::dmn_size());

    int N = cluster_eigenvector_dmn.get_size();
    invert_plan<std::complex<double> > invert_G4  (N);
    invert_plan<std::complex<double> > invert_G4_0(N);

    {
      G4 *= renorm;
      
      memcpy(&invert_G4.Matrix[0], &G4(0), sizeof(std::complex<double>)*N*N);
      invert_G4.execute_plan();
    }

    {
      G4_0 *= renorm;
      
      memcpy(&invert_G4_0.Matrix[0], &G4_0(0), sizeof(std::complex<double>)*N*N);
      invert_G4_0.execute_plan();
    }

    for(int j=0; j<N; j++)
      for(int i=0; i<N; i++)
	Gamma_cluster(i, j) = (invert_G4_0.inverted_matrix[i+j*N]
			       -invert_G4 .inverted_matrix[i+j*N]);
    
//     if(concurrency.id()==concurrency.last())
//       cout << "symmetrize Gamma_cluster" << endl;

//     symmetrize::execute(Gamma_cluster, MOMS.H_symmetry, parameters.get_q_vector(), false);
//     diagrammatic_symmetries_obj.execute(Gamma_cluster);
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::compute_Gamma_lattice()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl << endl;

    MOMS.coarsegrain_inversion_obj.execute(Gamma_cluster, Gamma_lattice);

//     {
//       profiler_t prof(concurrency, "compute_Gamma_lattice hspline Gamma", __FILE__, __LINE__);
//       MOMS.coarsegrain_inversion_obj.interpolate(Gamma_cluster, Gamma_lattice);
//     }

//     {
//       symmetrize::execute(Gamma_lattice, MOMS.H_symmetry, parameters.get_q_vector());
//       diagrammatic_symmetries_obj.execute(Gamma_lattice);
//     }


//     if(false)
//       {// works apparently well, but need to test with interpolation first ...
// 	profiler_t prof(concurrency, "compute_Gamma_lattice CG-inv Gamma", __FILE__, __LINE__);
	
// 	FUNC_LIB::function<std::complex<scalartype>, HOST_matrix_dmn_t> tmp2;
// 	MOMS.coarsegrain_inversion_obj.execute(Gamma_lattice, tmp2);
	
// 	Gamma_lattice = tmp2;
//       }

//     {
//       symmetrize::execute(Gamma_lattice, MOMS.H_symmetry, parameters.get_q_vector());
//       diagrammatic_symmetries_obj.execute(Gamma_lattice);
//     }        
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::compute_chi_0()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    if(concurrency.id()==concurrency.last())
      cout << __FUNCTION__ << endl << endl;

    {
      profiler_t prof("compute_chi_0 CG-inv Sigma", __FILE__, __LINE__);
      MOMS.coarsegrain_inversion_obj.execute(MOMS.Sigma, MOMS.Sigma_lattice_interpolated, MOMS.Sigma_lattice_coarsegrained, MOMS.Sigma_lattice); 
      //MOMS.coarsegrain_inversion_obj.execute(MOMS.Sigma, MOMS.Sigma_lattice_coarsegrained, MOMS.Sigma_lattice);
    }

    {
      profiler_t prof("compute_chi_0 compute_bubble", __FILE__, __LINE__);

      //compute_bubble<parameter_type, k_HOST, w_VERTEX, TRAPEZIUM_INTEGRATION> make_G4_0_CG_obj(parameters);
      compute_bubble<parameter_type, k_HOST, w_VERTEX, QUADRATURE_INTEGRATION> make_G4_0_CG_obj(parameters);
      
      make_G4_0_CG_obj.execute(MOMS.H_LDA, MOMS.Sigma_lattice, chi_0);
    }
    
    scalartype renorm = 1./(parameters.get_beta()*k_HOST::dmn_size());
    chi_0 *= renorm;
    
//     symmetrize::execute(chi_0, MOMS.H_symmetry, parameters.get_q_vector());
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::diagonolize_Gamma_times_chi_0()
  {
    if(parameters.get_vertex_measurement_type() == PARTICLE_PARTICLE_SUPERCONDUCTING)
      {
	int start = clock();
	diagonolize_Gamma_times_chi_0_symmetric();
	cout << "\n\n time to diagonolize : " << scalartype(clock()-start)/scalartype(CLOCKS_PER_SEC) << "\n\n";

	find_harmonic_expansion();
	write_on_shell();
      }
    else
      {
	int start = clock();
	diagonolize_Gamma_times_chi_0_general();
	cout << "\n\n time to diagonolize : " << scalartype(clock()-start)/scalartype(CLOCKS_PER_SEC) << "\n\n";
	
	find_harmonic_expansion();
	write_on_shell();
      }
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::diagonolize_Gamma_times_chi_0_symmetric()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);
    cout << __FUNCTION__ << endl << endl;

    int N = lattice_eigenvector_dmn.get_size();

    eigensystem_plan<float, HERMITIAN> eigensystem_pln(N);
      
    for(int j=0; j<N; ++j)
      for(int i=0; i<N; ++i)
	eigensystem_pln.A[i+j*N] = std::sqrt(real(chi_0(i,i)))*real(Gamma_lattice(i,j))*std::sqrt(real(chi_0(j,j)));

    for(int j=0; j<N; ++j){
      for(int i=0; i<N; ++i){
	float a_ij = eigensystem_pln.A[i+j*N];
	float a_ji = eigensystem_pln.A[j+i*N];

	eigensystem_pln.A[i+j*N] = (a_ij+a_ji)/2.;
	eigensystem_pln.A[j+i*N] = (a_ij+a_ji)/2.;
      }
    }
      
    std::vector<std::pair<std::complex<scalartype>, int> > eigenvals;
    { 
      int start = clock();
      
      int M = eigensystem_pln.execute_plan(0., 2.);
      
      cout << "\n\n time to diagonolize : " << scalartype(clock()-start)/scalartype(CLOCKS_PER_SEC) << "\n\n number of eigenvalues in [0, 2] : " << M << "\n\n";
      
      for(int i=0; i<M; i++)
	eigenvals.push_back(std::pair<std::complex<scalartype>, int>(eigensystem_pln.W[i], i));
    }
      
    stable_sort(eigenvals.begin(), eigenvals.end(), &susceptibility_less_pairs);
    
    { // write down leading eigenvalues ...
      for(int i=0; i<N_LAMBDAS; i++)
	{
	  leading_eigenvalues(i) = eigenvals[eigenvals.size()-1-i].first;
	  
	  int leading_lambda_index = eigenvals[eigenvals.size()-1-i].second;
	  for(int l=0; l<N; l++)
	    leading_eigenvectors(i, l) = scalartype(eigensystem_pln.A[l + leading_lambda_index*N])/std::sqrt(chi_0(l,l));
	}
    }
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::diagonolize_Gamma_times_chi_0_general()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    cout << __FUNCTION__ << endl << endl;

    int N = lattice_eigenvector_dmn.get_size();

    eigensystem_plan<std::complex<scalartype>, GENERAL> eigensystem_pln(N);
    
    {
      FUNC_LIB::function<std::complex<scalartype>, HOST_matrix_dmn_t> Gamma_times_chi_0("Gamma_times_chi_0");

      gemm_plan<std::complex<scalartype> > gemm_pln(N);
      
      gemm_pln.A = &Gamma_lattice(0);
      gemm_pln.B = &chi_0(0);
      gemm_pln.C = &Gamma_times_chi_0(0);
      
      gemm_pln.execute_plan();

      memcpy(&eigensystem_pln.A[0], &Gamma_times_chi_0(0), sizeof(std::complex<scalartype>)*N*N); 
    }

    std::vector<std::pair<std::complex<scalartype>, int> > eigenvals;
    { 
      eigensystem_pln.execute_plan();
      
      for(int i=0; i<N; i++)
	eigenvals.push_back(std::pair<std::complex<scalartype>, int>(eigensystem_pln.W[i], i));
    }

   stable_sort(eigenvals.begin(), eigenvals.end(), &susceptibility_less_pairs);
   
   { // write down leading eigenvalues ...
     for(int i=0; i<N_LAMBDAS; i++)
       {
	 leading_eigenvalues(i) = eigenvals[eigenvals.size()-1-i].first;

	 int leading_lambda_index = eigenvals[eigenvals.size()-1-i].second;
	 for(int l=0; l<N; l++)
	   leading_eigenvectors(i, l) = eigensystem_pln.VR[l + leading_lambda_index*N];
       }
   }

   if(false)
     {
       compute_chi(eigensystem_pln);
     }
  }
  
  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::find_harmonic_expansion()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

//     {
//       int MATRIX_DIM = square(b::dmn_size())*k_HOST::dmn_size()*w_VERTEX::dmn_size();      
//       for(int i=0; i<N_LAMBDAS; i++){
	
// 	std::complex<scalartype> max=0;
      
// 	for(int l=0; l<MATRIX_DIM; l++)
// 	  if(abs(leading_eigenvectors(i,l))> abs(max))
// 	    max = leading_eigenvectors(i,l);
	
// 	for(int l=0; l<MATRIX_DIM; l++)
// 	  leading_eigenvectors(i,l) = leading_eigenvectors(i,l)/max;
//       }
//     }

     {
       int MATRIX_DIM = square(b::dmn_size())*k_HOST::dmn_size()*w_VERTEX::dmn_size();      

       for(int i=0; i<N_LAMBDAS; i++)
	 {
	   scalartype N=1.e4;
	   
	   std::complex<scalartype> alpha_min=0;
	   scalartype norm = 1.e6;
	   
	   for(int l=0; l<N; l++)
	     {
	       std::complex<scalartype> alpha = std::complex<scalartype>(cos(2.*M_PI*l/N), sin(2.*M_PI*l/N)); 
	       
	       scalartype result=0;
	       
	       for(int w1=0; w1<w_VERTEX::dmn_size()/2; w1++)
		 for(int K1=0; K1<k_HOST::dmn_size(); K1++)
		   for(int m1=0; m1<b::dmn_size(); m1++)
		     for(int n1=0; n1<b::dmn_size(); n1++)
		       result += abs(alpha*leading_eigenvectors(i, n1,m1,K1,w1)
				     - conj(alpha*leading_eigenvectors(i, n1,m1,K1,w_VERTEX::dmn_size()-1-w1)));
	  
	       if(result < norm){
		 norm = result;
		 alpha_min = alpha;
	       }
	     }
	   
	   for(int l=0; l<MATRIX_DIM; l++)
	     leading_eigenvectors(i, l) *= alpha_min;
	 }
     }

    for(int i=0; i<N_LAMBDAS; i++){
      for(int H_ind=0; H_ind<N_HARMONICS; ++H_ind){

	std::complex<scalartype> coef=0;
	std::complex<scalartype> norm_phi=0;

	for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++){
	  
	  std::complex<scalartype> phi = leading_eigenvectors(i, 0,0,k_ind, w_VERTEX::dmn_size()/2);
	  std::complex<scalartype> psi = harmonics(k_ind, H_ind);
	  
	  coef     += psi*phi;
	  norm_phi += phi*std::conj(phi);
	}
	
	leading_symmetries(i, H_ind) = abs(coef)/sqrt(norm_phi);
      }
    }
  }

  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::write_on_shell()
  {
    profiler_t prof(__FUNCTION__, __FILE__, __LINE__);

    int size_value = 6;
    cout.precision(size_value);

    int size_begin = 0;
    size_value    += (2+4);

    {
      std::stringstream ss;
    
      ss << "    ---> (leading) j = " /*<< 0*/ 
	 << "    sval = "             /*<< sqrt(square(1.0 - real(lambda)) + square(imag(lambda)))*/
	 << "    eigenval = "         /*<< real(lambda)*/ << " ; " /*<< fabs(imag(lambda))*/
	 << "    |    ";
      
      size_begin = ss.str().size() + 1 + 3*size_value;
    }

    cout << string(size_begin, ' ');
    for(int H_ind=0; H_ind<N_HARMONICS; H_ind++){
      std::string tmp = wave_functions_names(H_ind);
      tmp.resize(size_value, ' ');
      cout << "    " << tmp;
    }
    cout << endl;

    for(int i=0; i<N_LAMBDAS; i++) 
      {
	std::complex<scalartype> lambda = leading_eigenvalues(i);

	cout << "    ---> (leading) j = " << i 
	     << "    sval = "             << sqrt(square(1.0 - real(lambda)) + square(imag(lambda)))
	     << "    eigenval = "         << real(lambda) << " ; " << fabs(imag(lambda)) 
	     << "    |    ";
	
	for(int H_ind=0; H_ind<N_HARMONICS; H_ind++)
	  cout << "    " << abs(leading_symmetries(i, H_ind));
	
	cout << "\n";
      }
    cout << "\n\n";
  }


  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::compute_chi(eigensystem_plan<std::complex<scalartype>, GENERAL>& eigensystem_pln)
  {
    cout << __FUNCTION__ << endl;

    // eqn 20-21 in PRB 64 195130
    // \chi(Q,T) = \frac{1}{(\beta*N_c)^2} \sum_{K1,K2} \chi[Q,K1,K2]
    //           ===> \chi[Q,K1,K2] = [1-\chi^0 * \Gamma]^{-1} * \chi^0[Q,K1,K2]

    int N = lattice_eigenvector_dmn.get_size();

    FUNC_LIB::function<std::complex<scalartype>, HOST_HOST_matrix_dmn_t> chi("chi");
    FUNC_LIB::function<std::complex<scalartype>, HOST_HOST_matrix_dmn_t> denominator("temporary");

//     {// \Chi_0 * \Gamma --> Gamma_times_full_chi_0(0);
//       gemm_plan<std::complex<scalartype> > gemm_pln(N);
      
//       gemm_pln.A = &full_chi_0_b_k_w__b_k_w(0);
//       gemm_pln.B = &Gamma_b_k_w__b_k_w(0);
//       gemm_pln.C = &denominator(0);
      
//       gemm_pln.execute_plan();
//     }

    cout << "\t compute denominator \n";

    //eigensystem_plan<std::complex<scalartype>, GENERAL> eigensystem_pln(N,'V','V');
    { 
//       memcpy(&eigensystem_pln.A[0], &denominator(0), sizeof(std::complex<scalartype>)*denominator.size()); 
//       eigensystem_pln.execute_plan();

      invert_plan<std::complex<scalartype> > invert_pln(N);
	
      memcpy(invert_pln.Matrix, eigensystem_pln.VR, sizeof(std::complex<scalartype>)*N*N);
      invert_pln.execute_plan();
// 	memcpy(eigensystem_pln.VL, invert_pln.inverted_matrix, sizeof(std::complex<scalartype>)*N*N);

      {
	scalartype ONE(1);

	for(int j=0; j<N; j++)
	  for(int i=0; i<N; i++)
	    invert_pln.inverted_matrix[i+j*N] = (ONE/(ONE-eigensystem_pln.W[i]))*invert_pln.inverted_matrix[i+j*N];

	gemm_plan<std::complex<scalartype> > gemm_pln(N);
	gemm_pln.A = eigensystem_pln.VR;
	gemm_pln.B = invert_pln.inverted_matrix;
	gemm_pln.C = &denominator(0,0);

	gemm_pln.execute_plan();
      }

      cout << "\t compute chi_k_k' \n";

      {
	gemm_plan<std::complex<scalartype> > gemm_pln(N);
	gemm_pln.A = &denominator(0,0);
	gemm_pln.B = &chi_0(0,0);
	gemm_pln.C = &chi(0,0);

	gemm_pln.execute_plan();
      }  

      //std::complex<scalartype> result = 0.;
      for(int i=0; i < chi.size(); i++) 
	chi_q(0) += chi(i);
    
      //chi_q(0) *= 1./( parameters.get_beta() /* host_cluster_type::get_cluster_size()*/ );
    }
  }

}

#endif






  /*
  template<class parameter_type, class MOMS_type>
  void analysis<parameter_type, MOMS_type, ANALYSIS_INTERPOLATION>::initialize_wave_functions()
  {
    wave_functions_names = "no-name";

    wave_functions_names(0) = "s-wave";

    wave_functions_names(1) = "p_x-wave";
    wave_functions_names(2) = "p_y-wave";

    wave_functions_names(3) = "d_{x^2-y^2}-wave";
    wave_functions_names(4) = "d_{xy}-wave";

    double* f_vals = new double[N_HARMONICS+1];
    double* P_vals = new double[5];

    for(int l=1; l<=4; ++l){
      //Y_lm::evaluate_azimuthal(l, l, M_PI/2., f_vals);
      P_vals[l] = 1.;
      //cout << P_vals[l] << endl;
    }

    for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++){

      std::vector<double> k = k_HOST::get_elements()[k_ind];
      k = (host_k_cluster_type::find_equivalent_vectors_with_minimal_distance_to_origin(k).second)[0];

      double phi = std::atan(k[1]/std::sqrt(square(k[0])+1.e-6));
      
      harmonics(k_ind, 0) = 1.;
      
      harmonics(k_ind, 1) = P_vals[1]*cos(phi);
      harmonics(k_ind, 2) = P_vals[1]*sin(phi);

      harmonics(k_ind, 3) = P_vals[2]*cos(2.*phi);
      harmonics(k_ind, 4) = P_vals[2]*sin(2.*phi);

      harmonics(k_ind, 5) = P_vals[3]*cos(3.*phi);
      harmonics(k_ind, 6) = P_vals[3]*sin(3.*phi);

      harmonics(k_ind, 7) = P_vals[4]*cos(4.*phi);
      harmonics(k_ind, 8) = P_vals[4]*sin(4.*phi);
    }

    delete [] P_vals;
    delete [] f_vals;

    for(int H_ind=0; H_ind<N_HARMONICS; H_ind++){

      std::complex<double> norm_psi=0;
      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	norm_psi += harmonics(k_ind, H_ind)*conj(harmonics(k_ind, H_ind));
    
      for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++)
	harmonics(k_ind, H_ind) /= std::sqrt(real(norm_psi) + 1.e-16);    
    }

    for(int k_ind=0; k_ind<k_HOST::dmn_size(); k_ind++){

      std::vector<double> k = k_HOST::get_elements()[k_ind];
      k = (host_k_cluster_type::find_equivalent_vectors_with_minimal_distance_to_origin(k).second)[0];

      VECTOR_OPERATIONS::PRINT(k);
      for(int H_ind=0; H_ind<N_HARMONICS; H_ind++)
	cout << real(harmonics(k_ind, H_ind)) << "\t";
      cout << endl;
    }
    cout << endl;
  }
  */