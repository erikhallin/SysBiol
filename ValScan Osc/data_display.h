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
                              float alpha,float alpha_0,
                              float hill_coef,float beta,
                              float mrna_lacI,float mrna_tetR,float mrna_cl,
                              float prot_lacI,float prot_tetR,float prot_cl);
        bool cycle(void);
        bool test_result(void);

    private:

        int m_window_width,m_window_height;
        float m_time_start,m_time_end,m_step_size;
        float m_alpha,m_alpha_0,m_hill_coef,m_beta;
        float m_curr_mrna_lacI,m_curr_mrna_tetR,m_curr_mrna_cl;
        float m_curr_prot_lacI,m_curr_prot_tetR,m_curr_prot_cl;

        float m_deriv_mrna_lacI,m_deriv_mrna_tetR,m_deriv_mrna_cl;
        float m_deriv_prot_lacI,m_deriv_prot_tetR,m_deriv_prot_cl;

        bool m_have_start_values,m_have_data;
        //graph m_graph_mrna_lacI,m_graph_mrna_tetR,m_graph_mrna_cl;
        //graph m_graph_prot_lacI,m_graph_prot_tetR,m_graph_prot_cl;

        vector<st_point> m_vec_points;//for repoter (tetR prot)
        vector<st_point> m_vec_deriv;//for repoter (tetR prot)

        bool draw(void);
        bool generate_data(void);
        bool update_deriv(void);//will update static members
};

#endif // DATA_DISPLAY_H
