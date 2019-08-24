#include "data_display.h"

data_display::data_display()
{
    m_have_start_values=false;
    m_have_data=false;
    m_error=0;
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
    m_have_data=generate_data();

    return true;
}

bool data_display::print_result(void)
{
    //create file
    ofstream ofs_file_u("output_u.txt");
    if(!ofs_file_u) return false;//error

    ofs_file_u<<"Concentration of u"<<endl;
    for(int i=0;i<(int)m_vec_table_u.size();i++)
    {
        ofs_file_u<<m_vec_table_u[i].x<<","<<m_vec_table_u[i].y<<endl;
    }
    ofs_file_u.close();

    //create file
    ofstream ofs_file_v("output_v.txt");
    if(!ofs_file_v) return false;//error

    ofs_file_v<<"Concentration of v"<<endl;
    for(int i=0;i<(int)m_vec_table_v.size();i++)
    {
        ofs_file_v<<m_vec_table_v[i].x<<","<<m_vec_table_v[i].y<<endl;
    }
    ofs_file_v.close();

    //create file
    ofstream ofs_file_du("output_du.txt");
    if(!ofs_file_du) return false;//error

    ofs_file_du<<"Derivative of u"<<endl;
    for(int i=0;i<(int)m_vec_table_deriv_u.size();i++)
    {
        ofs_file_du<<m_vec_table_deriv_u[i].x<<","<<m_vec_table_deriv_u[i].y<<endl;
    }
    ofs_file_du.close();

    //create file
    ofstream ofs_file_dv("output_dv.txt");
    if(!ofs_file_dv) return false;//error

    ofs_file_dv<<"Derivative of v"<<endl;
    for(int i=0;i<(int)m_vec_table_deriv_v.size();i++)
    {
        ofs_file_dv<<m_vec_table_deriv_v[i].x<<","<<m_vec_table_deriv_v[i].y<<endl;
    }
    ofs_file_dv.close();

    return true;
}


//------------------

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

        //update derivative
        //if( !update_deriv() ) // without IPTG
        if( !update_deriv_iptg() ) //with IPTG
        {
            m_error=1;
            break;
        }

        //update u and v
        m_curr_u+= m_step_size * m_deriv_u;
        m_curr_v+= m_step_size * m_deriv_v;

        //store data
        m_vec_table_u.push_back( st_point(t,m_curr_u) );
        m_vec_table_v.push_back( st_point(t,m_curr_v) );
        m_vec_table_deriv_u.push_back( st_point(t,m_deriv_u) );
        m_vec_table_deriv_v.push_back( st_point(t,m_deriv_v) );
    }

    return true;
}

bool data_display::update_deriv(void)
{
    if(!m_have_start_values) return false;

    m_deriv_u= ( m_alpha_u / ( 1.0 + pow( m_curr_v,m_beta_u ) ) ) - m_curr_u;
    m_deriv_v= ( m_alpha_v / ( 1.0 + pow( m_curr_u,m_beta_v ) ) ) - m_curr_v;

    return true;
}

bool data_display::update_deriv_iptg(void)
{
    if(!m_have_start_values) return false;

    float u_iptg= m_curr_u / ( pow( ( 1 + m_iptg_conc / m_iptg_const ), m_iptg_pow ) );

    m_deriv_u= m_alpha_u / ( 1 + pow( m_curr_v,m_beta_u ) ) - m_curr_u;
    m_deriv_v= m_alpha_v / ( 1 + pow( u_iptg,m_beta_v ) ) - m_curr_v;

    return true;
}
