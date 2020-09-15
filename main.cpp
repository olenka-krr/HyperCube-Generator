#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "dhasse.h"
#include "vp_lib.h"

using namespace std;

int main(){
    int dim;                          /// hypercube dimension

    char fnameo[7];                   /// for output file name / ���������� ��� ����� ��������� �����

    cout << "Enter dimension of hypercube m = " ;
    cin >> dim;
    errhalt(dim < 1, "Wrong dimention!");

    int m = dim;

    sprintf(fnameo, "%d", dim);         /// integer to string
    cout << fnameo << endl;
    strcat(fnameo, "n_.ogt");
    cout << fnameo << endl;
    FILE* out = fopen(fnameo, "w");
    errhalt(out == 0, "Cannot open Output File!");


    d_hasse hcube(m);

    hcube.write();                      /// ����� �� ����� ����������� �������
    hcube.write(out);                   /// ������ � �������� ogt-����

    fclose(out);


    puts("\n------- Press any key---------");
    getchar();
    return 0;
}

