#include <stdio.h>

#include <cmath>

unsigned long next_subset(int, int, int*&);          /// ������������� ����������� �� ���������
/*==================================================================================*/
/************************************* ������  **************************************/
/*==================================================================================*/

class  d_hasse;                         /// diagram Hasse

/*----------------------------------------------------------------------------------*/
class d_hasse {
    public:
      int   nl;                         /// hypercube dimension = number of levels
      int   nv;							/// number of vertices
      int   na;                         /// number of arcs
      int*  nlv;                        /// arrow for number of vertices per level / ������ ���������� ������ �� ������
      int*  nla;                        /// arrow for number of arcs per level /������ ���������� ����� �� ������
      int** v;                          /// the level`s vertices` decimal numbers / ���������� ������ ������ �� ������
      int** binv;                       /// vertices` binary codes / �������� ���� ������ / nv * nl
   	  int** inc;                        /// Incidence matrix / ������� ������������� / (nv - 1) * na
      int** a;                          /// �������, a[i][j] - ����� j-�� ����, ��������� �� i-�� �������(head)
      int*  vt;                         /// vt[j]  - ����� �������(tail), � ������� ������ j-� ����
      d_hasse() {
        nl = nv = na = 0;
        nlv = nla =0;
        v = 0; inc = 0;
      }
      d_hasse(int);                     /// diagram Hasse of given dimension
     ~d_hasse();
      d_hasse& write(FILE*);		    /// ����� � ��������� ���� ��� �� �����
};

/*==================================================================================*/
/**************************** ������� ������ d_hasse  *******************************/
/*==================================================================================*/
//------------------------------------------------------------------------------
d_hasse::d_hasse(int dim){
    int i, k, j, l, m;
    unsigned long r;
	nl = dim;
	nv = pow(2, nl);
    nlv  = new int[nl + 1];
    nla  = new int[nl];
    v    = new int* [nl + 1];
    binv = new int* [nv];

    for (i = 0; i < nv; i++){
        binv[i] = new int [nl];
        for (j = 0; j < nl; j++)
            binv[i][j] = 0;
    }

    nlv[0] = 1;
    na = 0;
    for (i = 0; i < nl; i++){
        nla[i] = (nl - i) * nlv[i];
        nlv[i + 1] = nla[i] / (i + 1);
        na += nla[i];
    }

    /// ��������� ������ �������� �� ���� ������ ������� � ����
    /// ���������� ����� v ������� = �� ����� � �������� ������������� binv

    int* indx;
    v[0] = new int[1];
    v[0][0] = 0;
    for (i = 1; i < nl; i++){
        l = nlv[i];
        v[i] = new int[l];
        while( r = next_subset(nl, i, indx)){
            v[i][r - 1] = 0;
            for (j = 0; j < nl; j++) binv[nv - 1][j] = 0;
            for (j = 0; j < i; j++){
                k = indx[j];
                v[i][r - 1] += pow(2, k);
                binv[nv - 1][k] = 1;
            }
            m = v[i][r - 1];
            for (j = 0; j < nl; j++) binv[m][j] = binv[nv - 1][j];
        }
    }
    /// �������� ��� ��������� ������� - ��� �������
    v[nl] = new int[1];
    v[nl][0] = nv - 1;
    for (j = 0; j < nl; j++) binv[nv - 1][j] = 1;

    /// �������� � ������������� ������� �������������;
    /// inc[i][j] = -1 - ���� j �� ������� �� ������� i
    /// inc[i][j] = k - ���� j ������� �� ������� i � ������ � ������� k
    inc = new int* [nv - 1];
    for (i = 0; i < nv - 1; i++){
        inc[i] = new int [na];
        for (k = 0; k < na; k++) inc[i][k]= -1;
    }

    /// ��������� ��� �������� �� ���� ������
    ///int* indx;
    int vx, iv, s;
    k = 0;
    for (l = 0; l < nl; l++)
   	for (m = 0; m < nlv[l]; m++){
        iv = v[l][m];
        for (j = 0; j < nlv[l + 1]; j++){
            vx = v[l + 1][j];
         	/// ���� �������� ���� ������ ���������� �� �������, �� �������� ���� ����� ����
            s = 0;
            for (i = 0; i < nl; i++){
                s += abs(binv[iv][i] - binv[vx][i]);
            }
            if (s == 1){
            	inc[iv][k] = vx;
         		k++ ;
            }
        }
    }

    /// ������� ������� ��� �� ��������-������� - ������������������ ������� �������������
	a = new int* [nv - 1];
    k = 0;                     /// ����� ������� ������� �������������, �.�. ����� ����
    for (l = 0; l < nl; l++)
        for (i = 0; i < nlv[l]; i++){
            j = v[l][i];
            a[j] = new int [nl - l];
            for (m = 0; m < nl - l; m++){
                a[j][m] = k;
                k++;
            }
        }

    /// vt[j]  - ����� �������(tail), � ������� ������ j-� ����
    vt = new int [na];
    m = 0;
    for (l = 0; l < nl; l++)
        for (i = 0; i < nlv[l]; i++){
            j = v[l][i];
            for (k = 0; k < nl - l; k++)
    			vt[m] = inc[j][m++];
   	    }
}

//------------------------------------------------------------------------------
d_hasse::~d_hasse(){
    int i;
    if (nl) {
        for (i = 0; i < nv - 1; i++)  delete[] inc[i];
	    delete[] inc;

        for (i = 0; i < nv - 1; i++)  delete[] a[i];
	    delete[] a;

    	for (i = 0; i < nl; i++)  delete[] v[i];
	    delete[] v;

        for (i = 0; i < nv; i++)  delete[] binv[i];
	    delete[] binv;

        delete[] vt;
        delete[] nla;
        delete[] nlv;
        na = 0;
        nv = 0;
    }
    nlv = nla = 0;
    inc = 0;     a = 0;
    v = 0;  binv = 0;
}

//------------------------------------------------------------------------------
d_hasse& d_hasse::write(FILE* f = stdout){
    int i, j, k, r;

    /// ����������� ��������� = ���-�� ������, ����� ���-�� ������, ����� ���-�� ���
    fprintf(f, "%2d  %3d  %3d\n\n", nl, nv, na);

    /// ����� �����, ���-�� ������ �� �����, ���-�� ��� �� ���� ����� (��������� �� ������)
    for (i = 0; i < nl; i++)
   	fprintf(f, "%2d    %2d  %2d\n", i, nlv[i], nla[i]);
    fprintf(f, "\n");

    /// ����� ������� ������ �� ������
    fprintf(f, "\n 0 level 1 vertex\n", 0, nlv[0]);
    for (j = 0; j < nlv[0]; j++) fprintf(f, "%2d ", v[0][j]);
    fprintf(f, "\n");
	for (i = 1; i < nl; i++){
        fprintf(f, "\n%2d level  %2d vertices\n", i, nlv[i]);
        for (j = 0; j < nlv[i]; j++) fprintf(f, "%2d ", v[i][j]);
        fprintf(f, "\n");
    }

    fprintf(f, "\n");
    /// ����� �������� ����� ������ �� ������
    for (i = 0; i < nl; i++){
        fprintf(f, "\n%2d level  %2d vert.\n", i, nlv[i]);
        for (j = 0; j < nlv[i]; j++){
            r = v[i][j];
      	    for (k = 0; k < nl; k++) fprintf(f, "%1d", binv[r][k]);
            fprintf(f, " ");
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n    ");
    /// ����� ������� �������������
    for (j = 0; j < na; j++) fprintf(f, " %4d", j);
    fprintf(f, "\n\n");
    for (i = 0; i < nl; i++)
   	for (k = 0; k < nlv[i]; k++){
        r = v[i][k];
   		fprintf(f, "%2d  ", r);
      	for (j = 0; j < na; j++) fprintf(f, " %4d", inc[r][j]);
      	fprintf(f, "\n");
   	}
    fprintf(f, "\n\n");

    /// ����� ������� ������� ��� �� ��������� ��������
    for (r = 0; r < nl; r++)
        for (i = 0; i < nlv[r]; i++){
            j = v[r][i];
            fprintf(f, "%3d   ", j);
            for (k = 0; k < nl - r; k++) fprintf(f, " %4d", a[j][k]);
            fprintf(f, "\n");
   	    }
    fprintf(f, "\n\n");

    /// ����� ������ �������� ������� ��� ������ ����
    for (j = 0; j < na; j++) fprintf(f, " %4d", j);
    fprintf(f, "\n");
    for (j = 0; j < na; j++) fprintf(f, " %4d", vt[j]);

    fprintf(f, "\n");
    return *this;
}

