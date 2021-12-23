#include "Fatrop.hpp"
#include "SPARSE/SparseOCP.hpp"
#include "SPARSE/InterfaceMUMPS.hpp"
#include "DEBUG/FatropDebugTools.hpp"
#include "AUX/FatropVector.hpp"
using namespace fatrop;
int main()
{
    /// sparse ocp
    OCP_dims dims;
    // next parameters give problem
    // dims.K = 3;
    // int nu = 10;
    // int nx = 9;
    // int ng = 4;
    // dims.nx.at(2) = 3*nx;
    dims.K = 1000;
    int nu = 50;
    int nx = 200;
    int ng = 0;
    dims.nx = vector<int>(dims.K, nx);
    dims.nu = vector<int>(dims.K, nu);
    dims.ng = vector<int>(dims.K, 0);
    dims.ng.at(0) = nx;
    dims.ng.at(dims.K-1) = nx;
    // dims.ng.at(1) = 0;
    dims.nx.at(2) = 100;
    // dims.ng.at(2) = 0;
    // dims.ng.at(0) = 0;
    // dims.nu.at(2) = 2*nu;
    // dims.nu.at(20) = 0.5*nu;
    dims.ng.at(dims.K-1) = 0;
    dims.nu.at(dims.K-1) = 0;
    // dims.ng.at(dims.K-1) = 0;
    // memory allocation
    fatrop_memory_allocator fma;
    OCP_KKT KKTocp(dims, fma);
    OCP_KKT_solver OCP_solver(dims, fma);

    int N_opti_vars = sum(dims.nu + dims.nx);
    int N_lags = sum(dims.nx) - dims.nx.at(0) + sum(dims.ng);
    fatrop_memory_vector_bf ux(N_opti_vars, 1, fma);
    fatrop_memory_vector_bf lags(N_lags, 1, fma);
    fatrop_memory_vector_bf lags2(N_lags, 1, fma);
    fatrop_memory_vector_bf ux2(N_opti_vars, 1, fma);
    fma.allocate();
    random_OCP(KKTocp, dims, 0);
    // KKTocp.BAbt[0].print();
    Sparse_OCP KOCP(dims, KKTocp);
    blasfeo_timer timer;
    cout << "solving using MUMPS" << endl;
    // KOCP.KKT.print("numpy");
    blasfeo_tic(&timer);
    KOCP.fact_solve(ux[0], lags[0]);
    double el = blasfeo_toc(&timer);
    // ux[0].print();
    cout << "solving using fatrop" << endl;
    blasfeo_tic(&timer);
    // for(int i=0; i<1000; i++){
    OCP_solver.fact_solve(&KKTocp, ux2[0], lags2[0]);
    // }
    double el2 = blasfeo_toc(&timer);
    // ux2[0].print();
    cout << "el time mumps " << el << endl;
    cout << "el time recursion " << el2 << endl;
    cout << "inf-norm difference MUMPS - Fatrop  primal " << (Eig(ux[0]) - Eig(ux2[0])).lpNorm<Eigen::Infinity>() << endl;
    cout << "inf-norm difference MUMPS - Fatrop  dual " << (Eig(lags[0]) - Eig(lags2[0])).lpNorm<Eigen::Infinity>() << endl;
    cout << "inf-norm   primal " << Eig(ux[0]).lpNorm<Eigen::Infinity>() << endl;
    cout << "inf-norm   dual " << Eig(lags[0]).lpNorm<Eigen::Infinity>() << endl;
    return 0;
}