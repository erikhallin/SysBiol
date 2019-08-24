#include "data_display.h"

data_display::data_display()
{
    m_numbof_graphs=1;
    m_have_start_values=false;
    m_have_data=false;
    m_error=0;
}

bool data_display::init(int window_width,int window_height,int numbof_graphs)
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
    int center_graph=numbof_graphs/2;
    for(int i=0;i<numbof_graphs;i++)
    {
        float intensity=1.0-0.2*fabs( center_graph-i );
        if(intensity<0.3) intensity=0.3;

        //GREEN
        m_vec_graphs_u.push_back( graph(20,50,750,530,true,true) );
        m_vec_graphs_u.back().set_color( 0.2, 1.0*intensity, 0.2, 1.0 );

        //RED
        m_vec_graphs_v.push_back( graph(20,50,750,530,true,true) );
        m_vec_graphs_v.back().set_color( 1.0*intensity, 0.2, 0.2, 1.0 );
    }

    //init u vs v graph (BLUE)
    m_graph_u_vs_v=graph( graph(20,50,750,530,true,true) );
    m_graph_u_vs_v.set_color( 0.1,0.1,1.0,1.0 );

    return true;
}

bool data_display::set_start_values(float start_u,float start_v,
                                    float start_time,float end_time,float step_size,
                                    float alpha_u,float alpha_v,
                                    float beta_u,float beta_v,
                                    float iptg_conc,float iptg_const,float iptg_pow)
{
    m_u_start=m_curr_u=start_u;
    m_v_start=m_curr_v=start_v;
    m_alpha_u=alpha_u;
    m_alpha_v=alpha_v;
    m_beta_u=beta_u;
    m_beta_v=beta_v;
    m_time_start=start_time;
    m_time_end=end_time;
    m_step_size=step_size;
    m_iptg_conc=iptg_conc;
    m_iptg_const=iptg_const;
    m_iptg_pow=iptg_pow;

    m_have_start_values=true;

    //generate data
    //m_have_data=generate_data(m_graph_u,m_graph_v);

    //more data
    int numbof_graphs=m_numbof_graphs;
    for(int i=0;i<(int)m_vec_graphs_u.size();i++)
    {
        generate_data( m_vec_graphs_u[i],m_vec_graphs_v[i],i,numbof_graphs );
    }

    //use same window size
    float extremes[4];
    m_vec_graphs_u[0].get_view_window(extremes);
    float xmin=extremes[0];
    float xmax=extremes[1];
    float ymin=extremes[2];
    float ymax=extremes[3];
    //find extremes for all graphs
    for(int i=0;i<(int)m_vec_graphs_u.size();i++)
    {
        m_vec_graphs_u[i].get_view_window(extremes);
        if(extremes[0]<xmin) xmin=extremes[0];
        if(extremes[1]>xmax) xmax=extremes[1];
        if(extremes[2]<ymin) ymin=extremes[2];
        if(extremes[3]>ymax) ymax=extremes[3];
    }
    //set same window for all
    extremes[0]=xmin;
    extremes[1]=xmax;
    extremes[2]=ymin;
    extremes[3]=ymax;
    for(int i=0;i<(int)m_vec_graphs_u.size();i++)
    {
        m_vec_graphs_u[i].set_view_window(extremes);
    }
    cout<<"xmin: "<<xmin<<endl;
    cout<<"xmax: "<<xmax<<endl;
    cout<<"ymin: "<<ymin<<endl;
    cout<<"ymax: "<<ymax<<endl;

    return true;
}

bool data_display::cycle(void)
{
    draw();

    return true;
}


//------------------

bool data_display::draw(void)
{
    for(int i=0;i<(int)m_vec_graphs_u.size();i++)
    {
        m_vec_graphs_u[i].draw_graph();
    }
    for(int i=0;i<(int)m_vec_graphs_u.size();i++)
    {
        m_vec_graphs_v[i].draw_graph();
    }

    //m_graph_u_vs_v.draw_graph();

    return true;
}

bool data_display::generate_data(graph& graph_u,graph& graph_v,int curr_index,int numbof_graphs)
{
    int center_graph=numbof_graphs/2;
    float sizeof_offset=0.1;
    float value_offset=1.0+sizeof_offset*float( center_graph-curr_index );

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
        /*if( !get_deriv(m_curr_u, m_curr_v,
                            m_alpha_u*value_offset, m_alpha_v*value_offset,
                            m_beta_u, m_beta_v,
                            deriv_u, deriv_v) )*/
        if( !get_deriv_iptg(m_curr_u, m_curr_v,
                            m_alpha_u*value_offset, m_alpha_v*value_offset,
                            m_beta_u, m_beta_v,
                            m_iptg_conc,m_iptg_const,m_iptg_pow,
                            deriv_u, deriv_v) )
        {
            m_error=1;
            break;
        }

        //update u and v
        m_curr_u+= m_step_size * deriv_u;
        m_curr_v+= m_step_size * deriv_v;

        graph_u.add_point_without_calc( t,m_curr_u );
        graph_v.add_point_without_calc( t,m_curr_v );

        //add point to u vs v graph, if only one graph shown
        if(m_numbof_graphs==1)
        {
            m_graph_u_vs_v.add_point_without_calc( m_curr_u,m_curr_v );
        }
    }

    graph_u.force_pre_draw_calc();
    graph_v.force_pre_draw_calc();

    if(m_numbof_graphs==1)
    {
        m_graph_u_vs_v.force_pre_draw_calc();
    }

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
