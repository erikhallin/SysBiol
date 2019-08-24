#ifndef DATA_DISPLAY_H
#define DATA_DISPLAY_H

#define _deg2rad 0.0174532925

#include <iostream>
#include <math.h>
#include <vector>
//#include "graph.h"

using namespace std;

struct st_point
{
    float x,y;
};

class data_display
{
    public:
        data_display();

        int m_error;

        bool init(int window_width,int window_height);
        bool set_start_values(float start_time,float end_time,float step_size,
                              float start_u,float start_v,
                              float alpha_u,float alpha_v,
                              float beta_u,float beta_v,
                              float iptg_conc_start,float iptg_const,float iptg_pow);
        bool cycle(void);
        bool test_result(void);

    private:

        int m_window_width,m_window_height;
        float m_time_start,m_time_end,m_step_size;
        float m_u_start,m_v_start;
        float m_curr_u,m_curr_v;
        float m_alpha_u,m_alpha_v,m_beta_u,m_beta_v;
        float m_iptg_conc,m_iptg_const,m_iptg_pow;

        bool m_have_start_values,m_have_data;

        vector<st_point> m_vec_points_u;//for repoter (u)
        vector<st_point> m_vec_points_v;//for (v)
        vector<st_point> m_vec_deriv_u;//for repoter (u)
        vector<st_point> m_vec_deriv_v;//for (v)

        bool draw(void);
        bool generate_data(void);
        bool get_deriv(float curr_u,float curr_v,
                       float alpha_u,float alpha_v,
                       float beta_u,float beta_v,
                       float& deriv_u,float& deriv_v);
        bool get_deriv_iptg(float curr_u,float curr_v,
                            float alpha_u,float alpha_v,
                            float beta_u,float beta_v,
                            float iptg_conc,float iptg_const,float iptg_pow,
                            float& deriv_u,float& deriv_v);
};

#endif // DATA_DISPLAY_H
