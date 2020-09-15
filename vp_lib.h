/*---------------------------- Функции и процедуры из библиотеки В.П. Пинчука ------------------------*/
/// получить след. r-элементное подмножество из n-элементного множества А
unsigned long next_subset(int n, int r, int*& A);

void errhalt(int, char*);      /// обработка исключения

/*==================================================================================*/
unsigned long next_subset(int n, int r, int*& B){
    int i, k, q = 1;
    static int* A;
    static unsigned long m;

    m++;
    if (m == 1){
        A = new int[n];
        for (i = 0; i < n; i++) A[i] = i;
        B = A;
        return m;
    }
    k = r - 1;
    while (q){
        A[k]++;
		if (A[k] + r - k - 1 < n) q = 0;
		else k--;
        if (k < 0){
            m = 0;
            break;
        }
		for(i = k + 1; i < r; i++) A[i] = A[i - 1] + 1;
    }
    if (m == 0) delete[] A;
    else B = A;
    return m;
}

/*-------------------------------------------------------------------------------------*/
void errhalt(int q, char* mes){
    if (q){
        puts(mes);
        puts("\7\7");
        getchar();
        exit(0);
    }
}
