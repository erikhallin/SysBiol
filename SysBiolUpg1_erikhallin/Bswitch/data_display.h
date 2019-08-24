#ifndef DATA_DISPLAY_H
#define DATA_DISPLAY_H

#define _deg2rad 0.0174532925

#include <math.h>
#include <vector>
#include "graph.h"

using namespace std;

class data_display
{
    public:
        data_display();

        int m_error;

        bool init(int window_width,int window_height,int numbof_graphs);
        bool set_start_values(float start_u,float start_v,
                              float start_time,float end_time,float step_size,
                              float alpha_u,float alpha_v,
                              float beta_u,float beta_v,
                              float iptg_conc,float iptg_const,float iptg_pow);
        bool cycle(void);

    private:

        int m_window_width,m_window_height;
        int m_numbof_graphs;
        float m_u_start,m_v_start,m_time_start,m_time_end,m_step_size;
        float m_curr_u,m_curr_v;
        float m_alpha_u,m_alpha_v,m_beta_u,m_beta_v;
        float m_iptg_conc,m_iptg_const,m_iptg_pow;
        float m_deriv_u,m_deriv_v;
        bool m_have_start_values,m_have_data;
        graph m_graph_u_vs_v;
        graph m_graph_u;
        graph m_graph_v;

        bool draw(void);
        bool generate_data(void);
        bool update_deriv(void);
        bool update_deriv_iptg(void);
};

#endif // DATA_DISPLAY_H
