#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "data_display.h"

using namespace std;

int main()
{
    cout<<"Software Startup - Bistable Switch\n\n";

    //read input from file
    cout<<"Reading starting values from file \"input.txt\"\n\n";
    ifstream ifs_file("input.txt");
    if(!ifs_file)
    {//error
        cout<<"ERROR: Could not find input file!\n";
        return 1;
    }
    string line,word;
    getline( ifs_file,line );//skip first line
    getline( ifs_file,line );
    stringstream ss0(line);
    ss0>>word;//skip
    ss0>>word;
    float start_u=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss1(line);
    ss1>>word;//skip
    ss1>>word;
    float start_v=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss2(line);
    ss2>>word;//skip
    ss2>>word;
    float start_time=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss3(line);
    ss3>>word;//skip
    ss3>>word;
    float end_time=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss4(line);
    ss4>>word;//skip
    ss4>>word;
    float step_size=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss5(line);
    ss5>>word;//skip
    ss5>>word;
    float alpha_u=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss6(line);
    ss6>>word;//skip
    ss6>>word;
    float alpha_v=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss7(line);
    ss7>>word;//skip
    ss7>>word;
    float beta_u=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss8(line);
    ss8>>word;//skip
    ss8>>word;
    float beta_v=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss9(line);
    ss9>>word;//skip
    ss9>>word;
    float iptg_start_conc=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss10(line);
    ss10>>word;//skip
    ss10>>word;
    float iptg_const=atof( word.c_str() );
    getline( ifs_file,line );
    stringstream ss11(line);
    ss11>>word;//skip
    ss11>>word;
    float iptg_pow=atof( word.c_str() );

    //display settings
    cout<<"Settings:\n";
    cout<<"start_u: "<<start_u<<endl;
    cout<<"start_v: "<<start_v<<endl;
    cout<<"start_time: "<<start_time<<endl;
    cout<<"end_time: "<<end_time<<endl;
    cout<<"step_size: "<<step_size<<endl;
    cout<<"alpha_u: "<<alpha_u<<endl;
    cout<<"alpha_v: "<<alpha_v<<endl;
    cout<<"beta_u: "<<beta_u<<endl;
    cout<<"beta_v: "<<beta_v<<endl;
    cout<<"iptg_start_conc: "<<iptg_start_conc<<endl;
    cout<<"iptg_const: "<<iptg_const<<endl;
    cout<<"iptg_pow: "<<iptg_pow<<endl;

    //calc
    cout<<"\nCalculating...";
    data_display analysis;
    analysis.set_start_values(start_u,start_v,
                              start_time,end_time,step_size,
                              alpha_u,alpha_v,
                              beta_u,beta_v,
                              iptg_start_conc,iptg_const,iptg_pow);
    cout<<"DONE\n";

    //print output
    if( analysis.print_result() )
    {
        cout<<"\nResult written to files: output_u.txt, output_v.txt, output_du.txt, output_dv.txt\n";
    }
    else
    {
        cout<<"ERROR: Could not write result to files!\n";
        return 1;
    }

    //done
    cout<<"\nAnalysis COMPLETE\n";

    return 0;
}
