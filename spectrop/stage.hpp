
#pragma once
#include <casadi/casadi.hpp>
#include <string>
#include <memory>
#include "casadi_utilities.hpp"
namespace fatrop
{
    namespace spectrop
    {
        namespace cs = casadi;
        class Stage;
        class OcpInternal;
        class StageInternal
        {
        private:
            StageInternal(int K, const std::shared_ptr<OcpInternal> &ocp) : K_(K), ocp_(ocp)
            {
            }

        private:
            friend class Stage;
            friend class Ocp;
            void add_variables(const cs::MX &expr);

        public:
            const std::vector<cs::MX> &get_objective_terms();
            const std::vector<cs::MX> &get_constraints();
            const uo_map_mx<cs::MX> &get_next_states();
            const std::vector<cs::MX> &get_states();
            const uo_map_mx<cs::MX> &get_state_syms();
            const std::vector<cs::MX> &get_controls();
            const uo_map_mx<cs::MX> &get_control_syms();
            const std::vector<cs::MX> &get_control_parameters();
            const uo_map_mx<cs::MX> &get_control_parameter_syms();
            const std::shared_ptr<OcpInternal> &get_ocp();
            const std::shared_ptr<StageInternal> &get_next_stage();

        private:
            void register_state(const cs::MX &state);
            void register_control(const cs::MX &control);
            void register_control_parameter(const cs::MX &control_parameter);
            bool has_variable(const cs::MX &var);
            const int K_;
            std::vector<cs::MX> objective_terms_;
            std::vector<cs::MX> constraints_;
            uo_map_mx<cs::MX> next_states_;
            std::vector<cs::MX> states_;
            uo_set_mx states_set_;
            uo_map_mx<cs::MX> state_syms_;
            std::vector<cs::MX> controls_;
            uo_set_mx controls_set_;
            uo_map_mx<cs::MX> control_syms_;
            std::vector<cs::MX> control_parameters_;
            uo_set_mx control_parameters_set_;
            uo_map_mx<cs::MX> control_parameter_syms_;
            std::shared_ptr<OcpInternal> ocp_;
            std::shared_ptr<StageInternal> next_stage_;
        };
        class Stage : private std::shared_ptr<StageInternal>
        {
        public:
            friend class Ocp;
            Stage(int K, const std::shared_ptr<OcpInternal> &ocp) : std::shared_ptr<StageInternal>(new StageInternal(K, ocp))
            {
            }
            void subject_to(const cs::MX &constraint);
            void add_objective(const cs::MX &objective);
            void set_next(const cs::MX &state, const cs::MX &next_state);
            void set_next(const uo_map_mx<cs::MX> &next_states);
            const uo_map_mx<cs::MX> &dynamics();
            std::shared_ptr<const StageInternal> get_internal() const
            {
                return *this;
            }
        };
    } // namespace spectrop
} // namespace fatrop