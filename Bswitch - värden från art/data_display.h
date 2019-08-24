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
        bool m_have_start_values,m_have_data;
        graph m_graph_u_vs_v;
        vector<graph> m_vec_graphs_u;
        vector<graph> m_vec_graphs_v;

        bool draw(void);
        bool generate_data(graph& graph_u,graph& graph_v,int curr_index,int numbof_graphs);
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
