#include <iostream>
#include <fstream>
#include <vector>

#include "data_display.h"

using namespace std;

int main()
{
    cout<<"Will test start values on equation\n";

    //create outputfile
    ofstream ofs_outputfile("values_of_interest.txt");

    int analysis_counter=0;
    int hit_counter=0;

    //start values
    float start_time=0.0;
    float end_time=50.0;
    float step_size=0.1;

    float start_u_start=0.0;
    float start_u_end=0.0;
    float start_u_step=0.5;

    float start_v_start=1.0;
    float start_v_end=1.0;
    float start_v_step=0.5;

    float alpha_u_start=0.0;
    float alpha_u_end=10.0;
    float alpha_u_step=0.1;

    float alpha_v_start=0.0;
    float alpha_v_end=10.0;
    float alpha_v_step=0.1;

    float beta_u_start=0.0;
    float beta_u_end=10.0;
    float beta_u_step=0.1;

    float beta_v_start=0.0;
    float beta_v_end=10.0;
    float beta_v_step=0.1;

    float iptg_conc_start_start=0.0;
    float iptg_conc_start_end=0.0;
    float iptg_conc_start_step=0.1;

    float iptg_const_start=2.9618/100000.0;
    float iptg_const_end=2.9618/100000.0;
    float iptg_const_step=0.1;

    float iptg_pow_start=2.0015;
    float iptg_pow_end=2.0015;
    float iptg_pow_step=0.1;

    //calc theoretical number of analysis
    int numof_analyses_total=1;
    numof_analyses_total*=(start_u_end-start_u_start)/start_u_step+1.0;
    numof_analyses_total*=(start_v_end-start_v_start)/start_v_step+1.0;
    numof_analyses_total*=(alpha_u_end-alpha_u_start)/alpha_u_step+1.0;
    numof_analyses_total*=(alpha_v_end-alpha_v_start)/alpha_v_step+1.0;
    numof_analyses_total*=(beta_u_end-beta_u_start)/beta_u_step+1.0;
    numof_analyses_total*=(beta_v_end-beta_v_start)/beta_v_step+1.0;
    numof_analyses_total*=(iptg_conc_start_end-iptg_conc_start_start)/iptg_conc_start_step+1.0;
    numof_analyses_total*=(iptg_const_end-iptg_const_start)/iptg_const_step+1.0;
    numof_analyses_total*=(iptg_pow_end-iptg_pow_start)/iptg_pow_step+1.0;
    cout<<"Total number of analyses to be made: "<<numof_analyses_total<<endl;//only works if step sizes will fall on end point

    //loop through all values
    for(float start_u=start_u_start;start_u<=start_u_end;start_u+=start_u_step)
    for(float start_v=start_v_start;start_v<=start_v_end;start_v+=start_v_step)
    for(float alpha_u=alpha_u_start;alpha_u<=alpha_u_end;alpha_u+=alpha_u_step)
    for(float alpha_v=alpha_v_start;alpha_v<=alpha_v_end;alpha_v+=alpha_v_step)
    for(float beta_u=beta_u_start;beta_u<=beta_u_end;beta_u+=beta_u_step)
    for(float beta_v=beta_v_start;beta_v<=beta_v_end;beta_v+=beta_v_step)
    for(float iptg_conc_start=iptg_conc_start_start;iptg_conc_start<=iptg_conc_start_end;iptg_conc_start+=iptg_conc_start_step)
    for(float iptg_const=iptg_const_start;iptg_const<=iptg_const_end;iptg_const+=iptg_const_step)
    for(float iptg_pow=iptg_pow_start;iptg_pow<=iptg_pow_end;iptg_pow+=iptg_pow_step)
    {
        //run eqv
        data_display Analysis;
        Analysis.set_start_values(start_time,end_time,step_size,
                              start_u,start_v,
                              alpha_u,alpha_v,
                              beta_u,beta_v,
                              iptg_conc_start,iptg_const,iptg_pow);

        analysis_counter++;

        //test values
        if( Analysis.test_result() )
        {//print if success
            hit_counter++;
            ofs_outputfile<<"HIT: "<<hit_counter<<endl;
            //print values
            ofs_outputfile<<"start_u: "<<start_u<<endl;
            ofs_outputfile<<"start_v: "<<start_v<<endl;
            ofs_outputfile<<"alpha_u: "<<alpha_u<<endl;
            ofs_outputfile<<"alpha_v: "<<alpha_v<<endl;
            ofs_outputfile<<"beta_u: "<<beta_u<<endl;
            ofs_outputfile<<"beta_v: "<<beta_v<<endl;
            ofs_outputfile<<"iptg_conc_start: "<<iptg_conc_start<<endl;
            ofs_outputfile<<"iptg_const: "<<iptg_const<<endl;
            ofs_outputfile<<"iptg_pow: "<<iptg_pow<<endl;
            ofs_outputfile<<endl;
        }

        //update screen status
        cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
        cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
        cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
        cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

        cout<<"Number of analyses: "<<analysis_counter<<"\tHits: "<<hit_counter;
    }

    //close
    ofs_outputfile<<"END";
    ofs_outputfile.close();
    cout<<"\n\nAnalysis COMPLETE\n";
    cout<<"Number of hits: "<<hit_counter<<endl;

    return 0;
}
