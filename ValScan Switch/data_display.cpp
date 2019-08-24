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
                                    float start_u,float start_v,
                                    float alpha_u,float alpha_v,
                                    float beta_u,float beta_v,
                                    float iptg_conc_start,float iptg_const,float iptg_pow)
{
    m_time_start=start_time;
    m_time_end=end_time;
    m_step_size=step_size;

    m_u_start=m_curr_u=start_u;
    m_v_start=m_curr_v=start_v;
    m_alpha_u=alpha_u;
    m_alpha_v=alpha_v;
    m_beta_u=beta_u;
    m_beta_v=beta_v;
    m_iptg_conc=iptg_conc_start;
    m_iptg_const=iptg_const;
    m_iptg_pow=iptg_pow;

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
    //test if result looks good
    float error_tol=0.10;// 10%

    float values_u[3];
    float values_v[3];
    float derivates_u[3];
    float derivates_v[3];
    int numbof_values=(int)m_vec_points_u.size();//should be same as for v

    if(numbof_values==0) return false;//ERROR

    values_u[0]=m_vec_points_u[int(numbof_values/2)].y;
    values_u[1]=m_vec_points_u[int(numbof_values/2+1)].y;
    values_u[2]=m_vec_points_u[int(numbof_values*3/4)].y;
    values_v[0]=m_vec_points_v[int(numbof_values/2)].y;
    values_v[1]=m_vec_points_v[int(numbof_values/2+1)].y;
    values_v[2]=m_vec_points_v[int(numbof_values*3/4)].y;
    derivates_u[0]=m_vec_deriv_u[int(numbof_values/2)].y;
    derivates_u[1]=m_vec_deriv_u[int(numbof_values/2+1)].y;
    derivates_u[2]=m_vec_deriv_u[int(numbof_values*3/4)].y;
    derivates_v[0]=m_vec_deriv_v[int(numbof_values/2)].y;
    derivates_v[1]=m_vec_deriv_v[int(numbof_values/2+1)].y;
    derivates_v[2]=m_vec_deriv_v[int(numbof_values*3/4)].y;

    //is u larger than v -> proceed, can be a hit
    /*if( values_u[0]<values_v[0] )
    {
        return false;
    }*/

    //is v larger than u -> proceed, can be a hit
    if( values_u[0]>values_v[0] )
    {
        return false;
    }

    // v have to be larger than 1
    if( values_v[0]<1.0 ) return false;

    //v have to be more than twice u
    if(values_u[0]*1000 > values_v[0]) return false;

    //if(values[0]<0.01) return false;//close to zero will create false hits

    //slope not close to zero test
    /*float threshold=0.1;
    if( derivates[2]<threshold )
    {
        return false;
    }*/

    if( isnan( values_u[0] ) ) return false;//not a number
    if( isnan( values_v[0] ) ) return false;//not a number

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
    float time_for_iptg_addition=(m_time_end-m_time_start)/3.0;
    float new_iptg_conc=0.001;
    float time_for_iptg_removal=(m_time_end-m_time_start)*2.0/3.0;
    for(float t=m_time_start; t<=m_time_end; t+=m_step_size)
    {
        //IPTG test
        if( t>time_for_iptg_addition ) m_iptg_conc=new_iptg_conc;
        if( t>time_for_iptg_removal )  m_iptg_conc=0.0;

        //get derivative
        float deriv_u,deriv_v;
        if( !get_deriv(m_curr_u, m_curr_v,
                            m_alpha_u, m_alpha_v,
                            m_beta_u, m_beta_v,
                            deriv_u, deriv_v) )
        /*if( !get_deriv_iptg(m_curr_u, m_curr_v,
                            m_alpha_u, m_alpha_v,
                            m_beta_u, m_beta_v,
                            m_iptg_conc,m_iptg_const,m_iptg_pow,
                            deriv_u, deriv_v) )*/
        {
            m_error=1;
            cout<<"ERROR: Could not get derivative!\n";
            break;
        }

        //update u and v
        m_curr_u+= m_step_size * deriv_u;
        m_curr_v+= m_step_size * deriv_v;

        //graph_u.add_point_without_calc( t,m_curr_u );
        //graph_v.add_point_without_calc( t,m_curr_v );

        //add point to vector for reporter protein (u)
        st_point temp_point;
        temp_point.x=t;
        temp_point.y=m_curr_u;
        m_vec_points_u.push_back(temp_point);
        temp_point.y=m_curr_v;
        m_vec_points_v.push_back(temp_point);

        //add derivative for reporter protein (u)
        temp_point.y=deriv_u;
        m_vec_deriv_u.push_back(temp_point);
        temp_point.y=deriv_v;
        m_vec_deriv_v.push_back(temp_point);

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

bool data_display::get_deriv(float curr_u,float curr_v,
                             float alpha_u,float alpha_v,
                             float beta_u,float beta_v,
                             float& deriv_u,float& deriv_v)
{
    if(!m_have_start_values) return false;

    deriv_u= alpha_u / ( 1 + pow( curr_v,beta_u ) ) - curr_u;
    deriv_v= alpha_v / ( 1 + pow( curr_u,beta_v ) ) - curr_v;

    return true;
}

bool data_display::get_deriv_iptg(float curr_u,float curr_v,
                                  float alpha_u,float alpha_v,
                                  float beta_u,float beta_v,
                                  float iptg_conc,float iptg_const,float iptg_pow,
                                  float& deriv_u,float& deriv_v)
{
    if(!m_have_start_values) return false;

    float u_iptg= curr_u / ( pow( ( 1 + iptg_conc / iptg_const ), iptg_pow ) );

    deriv_u= alpha_u / ( 1 + pow( curr_v,beta_u ) ) - curr_u;
    deriv_v= alpha_v / ( 1 + pow( u_iptg,beta_v ) ) - curr_v;

    return true;
}

