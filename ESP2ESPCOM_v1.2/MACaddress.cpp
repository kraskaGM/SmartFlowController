/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
    cout<<"Hello World \n";
    //char colour[4][10] = { "Blue", "Red", "Orange", "Yellow" }; 
    char array[6][3];
    //{ "4C", "11", "AE", "EA", "EE", "B4" };
    char input[]="[\"0x4C\",\"0x11\",\"0xAE\",\"0xEA\",\"0xEE\",\"0xB4\"]";
    //uint8_t Output[]=[0x4C, 0x11, 0xAE, 0xEA, 0xEE, 0xB4];
    //int ArraySize = sizeof(Input);
    int size = sizeof(input);
    cout<<input<<"\n";
    cout <<"Size of tthe array is: ";
    cout<<size<<"\n";
    int j=0;
    int k=0;

    for(int i=0; i<size;++i)
    {   
        char comp = 'x';
        if(input[i]==comp)
        {
            array[j][k]=input[i+1];
            cout<<input[i+1];
            array[j][k+1]=input[i+2];
            cout<<input[i+2];
            cout<<" \n";
            ++j;
        }
        
    }
    int s=0;
    cout <<"Array is: \n";
    for (int i=0;i<6;++i)
    {
         cout<<array[i][s];
         cout<<array[i][s+1];
         cout<<" \n";
    }

    int mac1=(array[0][0]);//array[0][1];
    int ma1_2=(array[0][1]);
    //uint8_t ma1_2 =0x43;
    cout <<"MAC 2nd char is: \n";
    cout<< ma1_2<<"\n";

    cout <<"MAC: \n";
    cout<< mac1<<"\n";
    //mac1= mac1<<4;
    cout <<"Shifted MAC is: \n";
    cout<< mac1<<"\n";
    
    mac1|=ma1_2;
    mac1=mac1^0x38;
    cout <<"Merged MAC is: \n";
    cout<< mac1<<"\n";   
    
    unsigned int test =4;
    unsigned int test_2=12;
    
    cout <<"test is: \n";
    cout << test;
    
    cout<<" \n";
    test = test <<4;
    test = test;
    cout <<"Shifted test is: \n";
    cout << test<<"\n";
    
    test = test|test_2;
    cout <<"Merged MAC is: \n";
    cout<< test<<"\n";  
    
    uint8_t mac2=array[1][0];//&array[1][1];
    uint8_t mac3=array[2][0];//&array[2][1];
    uint8_t mac4=array[3][0];//&array[3][1];
    uint8_t mac5=array[4][0];//&array[4][1];
    uint8_t mac6=array[5][0];//&array[5][1];
    /*cout<< test<<"\n";*/
    /*cout<< mac1<<"\n";
    cout<< mac2<<"\n";
    cout<< mac3<<"\n";
    cout<< mac4<<"\n";
    cout<< mac5<<"\n";
    cout<< mac6<<"\n";*/
    return 0;
}
