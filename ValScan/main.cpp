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
    float end_time=20.0;
    float step_size=0.01;

    float alpha_start=0.0;
    float alpha_end=1000.0;
    float alpha_step=10.0;

    float alpha_0_start=0.0;
    float alpha_0_end=1.0;
    float alpha_0_step=0.1;

    float hill_coef_start=1.0;
    float hill_coef_end=2.0;
    float hill_coef_step=0.2;

    float beta_start=0.0;
    float beta_end=10.0;
    float beta_step=1.0;

    float mrna_lacI_start=0.0;
    float mrna_lacI_end=1.0;
    float mrna_lacI_step=0.5;

    float mrna_tetR_start=0.0;
    float mrna_tetR_end=1.0;
    float mrna_tetR_step=0.5;

    float mrna_cl_start=0.0;
    float mrna_cl_end=1.0;
    float mrna_cl_step=0.5;

    float prot_lacI_start=0.0;
    float prot_lacI_end=1.0;
    float prot_lacI_step=0.5;

    float prot_tetR_start=0.0;//reporter
    float prot_tetR_end=1.0;
    float prot_tetR_step=0.5;

    float prot_cl_start=0.0;
    float prot_cl_end=1.0;
    float prot_cl_step=0.5;

    //calc theoretical number of analysis
    int numof_analyses_total=1;
    numof_analyses_total*=(alpha_end-alpha_start)/alpha_step+1.0;
    numof_analyses_total*=(alpha_0_end-alpha_0_start)/alpha_0_step+1.0;
    numof_analyses_total*=(hill_coef_end-hill_coef_start)/hill_coef_step+1.0;
    numof_analyses_total*=(beta_end-beta_start)/beta_step+1.0;
    numof_analyses_total*=(mrna_lacI_end-mrna_lacI_start)/mrna_lacI_step+1.0;
    numof_analyses_total*=(mrna_tetR_end-mrna_tetR_start)/mrna_tetR_step+1.0;
    numof_analyses_total*=(mrna_cl_end-mrna_cl_start)/mrna_cl_step+1.0;
    numof_analyses_total*=(prot_lacI_end-prot_lacI_start)/prot_lacI_step+1.0;
    numof_analyses_total*=(prot_tetR_end-prot_tetR_start)/prot_tetR_step+1.0;
    numof_analyses_total*=(prot_cl_end-prot_cl_start)/prot_cl_step+1.0;
    cout<<"Total number of analyses to be made: "<<numof_analyses_total<<endl;//only works if step sizes will fall on end point

    //loop through all values
    for(float alpha=alpha_start;alpha<=alpha_end;alpha+=alpha_step)
    for(float alpha_0=alpha_0_start;alpha_0<=alpha_0_end;alpha_0+=alpha_0_step)
    for(float hill_coef=hill_coef_start;hill_coef<=hill_coef_end;hill_coef+=hill_coef_step)
    for(float beta=beta_start;beta<=beta_end;beta+=beta_step)
    for(float mrna_lacI=mrna_lacI_start;mrna_lacI<=mrna_lacI_end;mrna_lacI+=mrna_lacI_step)
    for(float mrna_tetR=mrna_tetR_start;mrna_tetR<=mrna_tetR_end;mrna_tetR+=mrna_tetR_step)
    for(float mrna_cl=mrna_cl_start;mrna_cl<=mrna_cl_end;mrna_cl+=mrna_cl_step)
    for(float prot_lacI=prot_lacI_start;prot_lacI<=prot_lacI_end;prot_lacI+=prot_lacI_step)
    for(float prot_tetR=prot_tetR_start;prot_tetR<=prot_tetR_end;prot_tetR+=prot_tetR_step)
    for(float prot_cl=prot_cl_start;prot_cl<=prot_cl_end;prot_cl+=prot_cl_step)
    {
        //run eqv
        data_display Analysis;
        Analysis.set_start_values(start_time,end_time,step_size,
                                  alpha,alpha_0,
                                  hill_coef,beta,
                                  mrna_lacI,mrna_tetR,mrna_cl,
                                  prot_lacI,prot_tetR,prot_cl);

        analysis_counter++;

        //test values
        if( Analysis.test_result() )
        {//print if success
            hit_counter++;
            ofs_outputfile<<"HIT: "<<hit_counter<<endl;
            //print values
            ofs_outputfile<<"alpha: "<<alpha<<endl;
            ofs_outputfile<<"alpha_0: "<<alpha_0<<endl;
            ofs_outputfile<<"hill_coef: "<<hill_coef<<endl;
            ofs_outputfile<<"beta: "<<beta<<endl;
            ofs_outputfile<<"mrna_lacI: "<<mrna_lacI<<endl;
            ofs_outputfile<<"mrna_tetR: "<<mrna_tetR<<endl;
            ofs_outputfile<<"mrna_cl: "<<mrna_cl<<endl;
            ofs_outputfile<<"prot_lacI: "<<prot_lacI<<endl;
            ofs_outputfile<<"prot_tetR: "<<prot_tetR<<endl;
            ofs_outputfile<<"prot_cl: "<<prot_cl<<endl;
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
    ofs_outputfile.close();
    cout<<"\n\nAnalysis COMPLETE\n";
    cout<<"Number of hits: "<<hit_counter<<endl;

    return 0;
}
