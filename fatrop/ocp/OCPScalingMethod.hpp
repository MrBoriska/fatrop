#ifndef SCALINGMETHODINCLUDED
#define SCALINGMETHODINCLUDED
#include "solver/FatropData.hpp"
#include "templates/NLPAlg.hpp"
#include "aux/SmartPtr.hpp"
#include "solver/AlgStrategy.hpp"
#include "ocp/OCPKKT.hpp"
#include <memory>
using namespace std;
namespace fatrop
{
    class OCPScalingMethod : public AlgStrategy // this is an OCP strategy
    {
    public:
        OCPScalingMethod(const shared_ptr<FatropParams> &fatrop_params) : AlgStrategy(fatrop_params){};
        virtual int ComputeScalings(
            OCPKKTMemory *OCP,
            double &obj_scale,
            FatropVecBF &x_scales,
            FatropVecBF &lam_scales,
            const FatropVecBF &grad_curr) = 0;
    };

} // namespace fatrop
#endif // !SCALINGMETHODINCLUDED