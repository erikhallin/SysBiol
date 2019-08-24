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

    m_graph_u= graph(20,50,750,530,true,true);
    m_graph_u.set_color( 0.3, 1.0, 0.1, 1.0 );
    m_graph_v= graph(20,50,750,530,true,true);
    m_graph_v.set_color( 1.0, 0.3, 0.1, 1.0 );

    return true;
}

bool data_display::set_start_values(st_hit hit)
{
    cout<<hit.hit_id<<endl;
    cout<<hit.start_time<<endl;
    cout<<hit.end_time<<endl;
    cout<<hit.step_size<<endl;

    cout<<hit.start_u<<endl;
    cout<<hit.start_v<<endl;
    cout<<hit.alpha_u<<endl;
    cout<<hit.alpha_v<<endl;
    cout<<hit.beta_u<<endl;
    cout<<hit.beta_v<<endl;

    cout<<hit.iptg_conc_start<<endl;
    cout<<hit.iptg_const<<endl;
    cout<<hit.iptg_pow<<endl;

    m_hit_id=hit.hit_id;

    m_time_start=hit.start_time;
    m_time_end=hit.end_time;
    m_step_size=hit.step_size;

    m_u_start=m_curr_u=hit.start_u;
    m_v_start=m_curr_v=hit.start_v;
    m_alpha_u=hit.alpha_u;
    m_alpha_v=hit.alpha_v;
    m_beta_u=hit.beta_u;
    m_beta_v=hit.beta_v;

    m_iptg_conc=hit.iptg_conc_start;
    m_iptg_const=hit.iptg_const;
    m_iptg_pow=hit.iptg_pow;

    m_have_start_values=true;

    //generate data
    m_have_data=generate_data();

    /*//use same window size
    float extremes[4];
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

bool data_display::cycle(st_hit hit)
{
    set_start_values(hit);//calc

    draw();

    return true;
}


//------------------

bool data_display::draw(void)
{
    m_graph_u.draw_graph();
    m_graph_v.draw_graph();

    return true;
}

bool data_display::generate_data(void)
{
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
            break;
        }

        //update u and v
        m_curr_u+= m_step_size * deriv_u;
        m_curr_v+= m_step_size * deriv_v;

        m_graph_u.add_point_without_calc( t,m_curr_u );
        m_graph_v.add_point_without_calc( t,m_curr_v );
    }

    m_graph_u.force_pre_draw_calc();
    m_graph_v.force_pre_draw_calc();

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

