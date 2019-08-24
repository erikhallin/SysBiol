#ifndef DATA_DISPLAY_H
#define DATA_DISPLAY_H

#define _deg2rad 0.0174532925

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct st_point
{
    st_point(float n1,float n2)
    {
        x=n1; y=n2;
    }
    float x,y;
};

class data_display
{
    public:
        data_display();

        int  m_error;

        bool set_start_values(float start_u,float start_v,
                              float start_time,float end_time,float step_size,
                              float alpha_u,float alpha_v,
                              float beta_u,float beta_v,
                              float iptg_conc,float iptg_const,float iptg_pow);
        bool print_result(void);

    private:

        float m_u_start,m_v_start,m_time_start,m_time_end,m_step_size;
        float m_curr_u,m_curr_v;
        float m_alpha_u,m_alpha_v,m_beta_u,m_beta_v;
        float m_iptg_conc,m_iptg_const,m_iptg_pow;
        float m_deriv_u,m_deriv_v;
        bool  m_have_start_values,m_have_data;

        vector<st_point> m_vec_table_u;
        vector<st_point> m_vec_table_v;
        vector<st_point> m_vec_table_deriv_u;
        vector<st_point> m_vec_table_deriv_v;

        bool generate_data(void);
        bool update_deriv(void);
        bool update_deriv_iptg(void);
};

#endif // DATA_DISPLAY_H
