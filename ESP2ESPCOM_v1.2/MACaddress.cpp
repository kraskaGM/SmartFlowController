/******************************************************************************
                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.
*******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

//Getting the MAC address characters from the input
char** MACextraction(char input[])
{
    int size = strlen(input);
    cout <<"MACextraction Input array size is: \n";
    cout<< size<<"\n";
    char** result = new char*[6];
    for (int i=0; i<6;++i)
    {
        result[i]=new char[3];
    }
    cout <<"MACextraction array generation  is: \n";
    int j=0;
    for(int i=0; i<size;++i)
    {   
        int k=0;
        char comp = 'x';
        if(input[i]==comp)
        {
            result[j][k]=input[i+1];
            cout<<input[i+1];
            result[j][k+1]=input[i+2];
            cout<<input[i+2];
            cout<<" \n";
            ++j;
        }
    }  
    return result;
}

//Adding character together
char** CharConc(char* input[])
{
    //Creating a new array to restore the conversion results.
    char** result = new char*[6];
    for (int i=0; i<6;++i)
    {
        result[i]=new char[3];
    }
    //Converting
    cout <<"Converted char array in int is: \n";
    for(int i=0; i<6;++i)
    {   
        result[i][0]=(input[i][0]);
        strncat(result[i], &input[i][1], 1);
        cout<<result[i];
        cout<<" \n";
    }  
    return result; 
}

int hex2decimal(char hex[])
{
    long long decimal, place;
    int i = 0, val, len;

    decimal = 0;
    place = 1;

    /* Input hexadecimal number from user */
    //printf("Enter any hexadecimal number: ");
    //gets(hex);

    /* Find the length of total number of hex digit */
    len = strlen(hex);
    len--;

    /*
     * Iterate over each hex digit
     */
    for(i=0; hex[i]!='\0'; i++)
    {
 
        /* Find the decimal representation of hex[i] */
        if(hex[i]>='0' && hex[i]<='9')
        {
            val = hex[i] - 48;
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10;
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }
    cout<<"Hexadecimal number = is: "<< hex<<" \n";
    cout<<"Decimal number is: "<< decimal<<" \n";
    //printf("Hexadecimal number = %s\n", hex);
    //printf("Decimal number = %lld", decimal);
    return decimal;
}
int main()
{
    int val =hex2decimal("4C");
    cout<<val<<"\n";    
    char** array;
    char input[]="[\"0x4C\",\"0x11\",\"0xAE\",\"0xEA\",\"0xEE\",\"0xB4\"]";
    int size = sizeof(input);
    cout <<"Outside the function Input array size is: \n";
    cout<< size<<"\n";
    array=MACextraction(input);
    char** converted;
    converted = CharConc(array);
    for (int i=0;i<6;++i)
    {
        hex2decimal(converted[i]);
    }

    int s=0;
    cout <<"Array is: \n";
    for (int i=0;i<6;++i)
    {
         cout<<array[i][s];
         cout<<array[i][s+1];
         cout<<" \n";
    }
    return 0;
}
