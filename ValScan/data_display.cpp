#include "data_display.h"

data_display::data_display()
{
    m_have_start_values=false;
    m_have_data=false;
    m_error=0;
}

bool data_display::init(int window_width,int window_height)
{
    m_window_width=window_width;
    m_window_height=window_height;

    //init display
    /*// 2 graphs ontop (relative values)
    m_graph_u=graph(20,50,750,240,true,true);
    m_graph_v=graph(20,350,750,240,true,true);
    */
    /*// 1 graph overlay (relative values)
    m_graph_u=graph(20,50,750,530,true,true);
    m_graph_v=graph(20,50,750,530,true,true);
    m_graph_u.set_color(1.0,0.3,0.3,1.0);
    m_graph_v.set_color(0.3,1.0,0.1,1.0);*/
    // n graphs overlay, center pos is exact

    /*//mRNA (GREEN)
    m_graph_mrna_lacI= graph(20,50,750,530,true,true);
    m_graph_mrna_lacI.set_color( 0.3, 1.0, 0.3, 1.0 );
    m_graph_mrna_tetR= graph(20,50,750,530,true,true);
    m_graph_mrna_tetR.set_color( 0.3, 1.0, 0.1, 1.0 );
    m_graph_mrna_cl= graph(20,50,750,530,true,true);
    m_graph_mrna_cl.set_color( 0.1, 1.0, 0.3, 1.0 );

    //Protein (RED)
    m_graph_prot_lacI= graph(20,50,750,530,true,true);
    m_graph_prot_lacI.set_color( 1.0, 0.3, 0.3, 1.0 );
    m_graph_prot_tetR= graph(20,50,750,530,true,true);
    m_graph_prot_tetR.set_color( 1.0, 0.3, 0.1, 1.0 );
    m_graph_prot_cl= graph(20,50,750,530,true,true);
    m_graph_prot_cl.set_color( 1.0, 0.1, 0.3, 1.0 );*/

    return true;
}

bool data_display::set_start_values(float start_time,float end_time,float step_size,
                                    float alpha,float alpha_0,
                                    float hill_coef,float beta,
                                    float mrna_lacI,float mrna_tetR,float mrna_cl,
                                    float prot_lacI,float prot_tetR,float prot_cl)
{
    m_time_start=start_time;
    m_time_end=end_time;
    m_step_size=step_size;

    m_alpha=alpha;
    m_alpha_0=alpha_0;
    m_hill_coef=hill_coef;
    m_beta=beta;

    m_curr_mrna_lacI=mrna_lacI;
    m_curr_mrna_tetR=mrna_tetR;
    m_curr_mrna_cl=mrna_cl;
    m_curr_prot_lacI=prot_lacI;
    m_curr_prot_tetR=prot_tetR;
    m_curr_prot_cl=prot_cl;

    m_have_start_values=true;

    //generate data
    m_have_data=generate_data();

    //use same window size
    /*float extremes[4];
    m_graph_mrna_lacI.get_view_window(extremes);
    float xmin=extremes[0];
    float xmax=extremes[1];
    float ymin=extremes[2];
    float ymax=extremes[3];
    //find extremes for all graphs
    m_graph_mrna_lacI.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];
    m_graph_mrna_tetR.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];
    m_graph_mrna_cl.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];
    m_graph_prot_lacI.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];
    m_graph_prot_tetR.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];
    m_graph_prot_cl.get_view_window(extremes);
    if(extremes[0]<xmin) xmin=extremes[0]; if(extremes[1]>xmax) xmax=extremes[1];
    if(extremes[2]<ymin) ymin=extremes[2]; if(extremes[3]>ymax) ymax=extremes[3];

    //set same window for all
    extremes[0]=xmin;
    extremes[1]=xmax;
    extremes[2]=ymin;
    extremes[3]=ymax;

    m_graph_mrna_lacI.set_view_window(extremes);
    m_graph_mrna_tetR.set_view_window(extremes);
    m_graph_mrna_cl.set_view_window(extremes);
    m_graph_prot_lacI.set_view_window(extremes);
    m_graph_prot_tetR.set_view_window(extremes);
    m_graph_prot_cl.set_view_window(extremes);*/

    return true;
}

bool data_display::cycle(void)
{
    draw();

    return true;
}

bool data_display::test_result(void)
{
    //test if result looks good, if reporter protein tetR varies after 10 time units
    float error_tol=0.10;// 10%
    //get 3 values to compare, center with center+1 and 3/4
    float values[4];
    int numbof_values=(int)m_vec_points.size();

    if(numbof_values==0) return false;//ERROR

    values[0]=m_vec_points[int(numbof_values/2)].y;
    values[1]=m_vec_points[int(numbof_values/2+1)].y;
    values[2]=m_vec_points[int(numbof_values*3/4)].y;

    if(values[0]<0.01) return false;//cloase to zero will create false hits

    float error_tol_abs=values[0]*error_tol;
    if( (values[0]<values[1]+error_tol_abs && values[0]>values[1]-error_tol_abs) &&
        (values[0]<values[2]+error_tol_abs && values[0]>values[2]-error_tol_abs) )
    {//no variation
        return false;//hit
    }

    return true;//hit
}


//------------------

bool data_display::draw(void)
{
    /*m_graph_mrna_lacI.draw_graph();
    m_graph_mrna_tetR.draw_graph();
    m_graph_mrna_cl.draw_graph();

    m_graph_prot_lacI.draw_graph();
    m_graph_prot_tetR.draw_graph();
    m_graph_prot_cl.draw_graph();*/

    return true;
}

bool data_display::generate_data(void)
{
    //Euler steps
    for(float t=m_time_start; t<=m_time_end; t+=m_step_size)
    {
        //update derivative
        if( !update_deriv() )
        {
            m_error=1;
            break;
        }

        //update mRNA and Protein concentrations
        m_curr_mrna_lacI+= m_step_size*m_deriv_mrna_lacI;
        m_curr_mrna_tetR+= m_step_size*m_deriv_mrna_tetR;
        m_curr_mrna_cl+= m_step_size*m_deriv_mrna_cl;
        m_curr_prot_lacI+= m_step_size*m_deriv_prot_lacI;
        m_curr_prot_tetR+= m_step_size*m_deriv_prot_tetR;
        m_curr_prot_tetR+= m_step_size*m_deriv_prot_tetR;

        //add point to vector for reporter protein (tetR)
        st_point temp_point;
        temp_point.x=t;
        temp_point.y=m_curr_prot_tetR;
        m_vec_points.push_back(temp_point);

        /*//add points to graphs
        m_graph_mrna_lacI.add_point_without_calc( t,m_curr_mrna_lacI );
        m_graph_mrna_tetR.add_point_without_calc( t,m_curr_mrna_tetR );
        m_graph_mrna_cl.add_point_without_calc( t,m_curr_mrna_cl );
        m_graph_prot_lacI.add_point_without_calc( t,m_curr_prot_lacI );
        m_graph_prot_tetR.add_point_without_calc( t,m_curr_prot_tetR );
        m_graph_prot_cl.add_point_without_calc( t,m_curr_prot_cl );*/
    }

    /*m_graph_mrna_lacI.force_pre_draw_calc();
    m_graph_mrna_tetR.force_pre_draw_calc();
    m_graph_mrna_cl.force_pre_draw_calc();
    m_graph_prot_lacI.force_pre_draw_calc();
    m_graph_prot_tetR.force_pre_draw_calc();
    m_graph_prot_cl.force_pre_draw_calc();*/

    return true;
}

bool data_display::update_deriv(void)
{
    if(!m_have_start_values) return false;

    m_deriv_mrna_lacI= -m_curr_mrna_lacI+ ( m_alpha/( 1 + pow(m_curr_prot_cl,m_hill_coef) ) ) + m_alpha_0;
    m_deriv_mrna_tetR= -m_curr_mrna_tetR+ ( m_alpha/( 1 + pow(m_curr_prot_lacI,m_hill_coef) ) ) + m_alpha_0;
    m_deriv_mrna_cl= -m_curr_mrna_cl+ ( m_alpha/( 1 + pow(m_curr_prot_tetR,m_hill_coef) ) ) + m_alpha_0;

    m_deriv_prot_lacI= -m_beta * ( m_curr_prot_lacI - m_curr_mrna_lacI );
    m_deriv_prot_tetR= -m_beta * ( m_curr_prot_tetR - m_curr_mrna_tetR );
    m_deriv_prot_cl= -m_beta * ( m_curr_prot_cl - m_curr_mrna_cl );

    return true;
}

