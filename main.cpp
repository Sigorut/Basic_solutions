#include <QCoreApplication>
#include "fraction.h"
#include <iostream>
#include <QFile>
using namespace std;

void read_matrix(QVector<QVector<fract>>& matrix){
    QFile file(":/matrix.txt");
    if(!file.size()){
        qDebug() << "File is empty!";
        exit(0);
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while(!file.atEnd()){
            QString lineStr = file.readLine();
            QStringList elMat = lineStr.split("\t");
            QVector<fract> lineEl;
            for(int i=0; i<elMat.size(); i++){
                fract oneEl;
                oneEl.u_num = elMat.at(i).toInt();
                oneEl.d_num = 1;
                lineEl.push_back(oneEl);
            }
            matrix.push_back(lineEl);
        }
    }
}

void cout_matrix(QVector<QVector<fract>> matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j <matrix[0].size(); j++){
            if(matrix[i][j].d_num == 1){
                cout<<matrix[i][j].u_num<<"\t";
            }else
                cout<<matrix[i][j].u_num<<"/"<<matrix[i][j].d_num<<"\t";
        }
        cout << endl << endl;
    }
    cout << endl;
}

struct aux_struct{
    fract f;
    int index;
};

bool comparison(fract frst, fract scnd){
    frst.u_num = scnd.d_num * frst.u_num;
    scnd.u_num = frst.d_num * scnd.u_num;
    if(abs(frst.u_num) < abs(scnd.u_num)){ //?
        return true;
    } else {
        return false;
    }
}

void sort_bubble(QVector <aux_struct>& row_num){
    for(int i = 0; i < row_num.size(); i++){
        for(int j = 0; j < row_num.size() - i - 1; j++){
            if(comparison(row_num[j].f, row_num[j + 1].f)){
                qSwap(row_num[j], row_num[j + 1]);
            }
        }
    }
}

void sort_row(QVector<QVector<fract>>& matrix, int yi, int xi){
    int row = matrix.size();
    aux_struct as;
    QVector <aux_struct> row_num;
    QVector<QVector<fract>> dop_mat = matrix;
    for(int i = yi; i < row; i++){
        as.f = matrix[i][xi];
        as.index = i;
        row_num << as;
    }
    sort_bubble(row_num);
    for(int i = yi, j = 0; i < row; i++, j++){
        matrix[i] = dop_mat[row_num[j].index];
    }
}

bool check_line_forward(QVector<fract> row){
    int cnt = 0;
    bool flag = true;
    for(int i = 0; i < row.size(); i++){
        if(row[i].u_num != 0){
            cnt++;
            flag = false;
        }
    }
    if(cnt == 1 && row[row.size() - 1].u_num != 0){
        qDebug() << "There are no solutions!";
        exit(0);
    }
    return flag;
}


void mirror(QVector<QVector<fract>>& matrix){
    int n_row = matrix.size();
    int n_col = matrix[0].size();
    int cnt = 0;
    for(int i = 0; i < n_row; i++){
        for(int j = 0; j < n_col; j++){
            cnt++;
            if(cnt <= (n_col * n_row) / 2){
                qSwap(matrix[i][j], matrix[n_row - i - 1][n_col - j - 1]);
            }
        }
    }
}

void unit_matrix_forward_reverse(QVector<QVector<fract>>& matrix){
    Fraction obj;
    int n_row = matrix.size();
    int n_col = matrix[0].size();
    fract f_num, diag_num;
    for(int yi = 0, xi = 0, iter = 1; yi < n_row; yi++, xi++, iter++){
        sort_row(matrix, yi, xi);
        diag_num = matrix[yi][xi];
        if(diag_num.u_num == 0){
            if(check_line_forward(matrix[yi])){
                if(xi < n_col - 1 && yi < n_row - 1){
                    xi++;
                    yi++;
                } else {
                    return;
                }
                diag_num = matrix[yi][xi];
                while(check_line_forward(matrix[yi])){
                    if(xi < n_col - 1 && yi < n_row - 1){
                        xi++;
                        yi++;
                    } else {
                        return;
                    }
                    diag_num = matrix[yi][xi];
                }
            } else {
                while(diag_num.u_num == 0){
                    xi++;
                    sort_row(matrix, yi, xi);
                    diag_num = matrix[yi][xi];
                }
            }
        }
        for(int i = yi; i < n_col; i++){
            matrix[yi][i] = obj.mult(matrix[yi][i], diag_num, 1); //str(i) / A(xi)
        }

        for(int i = 0; i < n_row; i++){
            f_num = matrix[i][xi];
            if(yi == i) continue;
            for(int j = xi; j < n_col; j++){
                matrix[i][j] = obj.sum(matrix[i][j], obj.mult(matrix[yi][j],f_num), 1);
            }
        }
        cout<<"IT: "<<iter<<endl;
        cout_matrix(matrix);
    }
}
int countNullRow(QVector<QVector<fract>>matrix){
    int count = 0;
    for(int i = 0; i < matrix.size(); i++){
        if(check_line_forward(matrix[i])){
            count++;
        }
    }
    return count;
}
int idxNullRow(QVector<QVector<fract>>matrix){
    for(int i = 0; i < matrix.size(); i++){
        if(check_line_forward(matrix[i])){
            return i;
        }
    }
    return -1;
}
int num_glues(int rang){
    if(rang%2 != 0){
        rang-=1;
    }
    return rang/2;
}
QVector<QVector<int>> combinations(QVector<QVector<int>> vec_pair, int rang){
    QVector<int> comb;
    QVector<QVector<int>> vec_rang, vec_rang_ass, vec_pair_local = vec_pair;
    for(int countrang = 0; countrang < rang-2; countrang++){
        vec_rang.clear();
        for(int i = 0; i < vec_pair.size(); i++){
            for(int j = 0; j < vec_pair_local.size(); j++){
                for(int k = 0; k < 2+countrang; k++){
                    comb.push_back(vec_pair[i][k]);
                }

                for(int k = 0; k < vec_pair_local[0].size(); k++){
                    comb.push_back(vec_pair_local[j][k]);
                }


                vec_rang.push_back(comb);
                comb.clear();
            }
        }
        for(int i = 0; i < vec_rang.size(); i++){
        }
        for(int i = 0; i < vec_rang.size(); i++){
            if(vec_rang[i][vec_rang[i].size()-2] == vec_rang[i][vec_rang[i].size()-3]){
                vec_rang_ass.push_back(vec_rang[i]);
            }
        }
        for(int i = 0; i < vec_rang_ass.size(); i++){
        }
        for(int i = 0; i < vec_rang_ass.size(); i++){
            auto iter = vec_rang_ass[i].cbegin(); // указатель на первый элемент
            vec_rang_ass[i].erase(iter + (vec_rang_ass[i].size()-3));
        }
        vec_pair = vec_rang_ass;
        vec_rang_ass.clear();
    }
return vec_pair;
}
void basic_solutions(QVector<QVector<fract>>matrix){
    int countNullR = countNullRow(matrix);
    for(int i = 0; i < countNullR; i++){
        auto iter = matrix.cbegin();
        matrix.erase(iter + idxNullRow(matrix));
    }
    cout<< "delete null"<<endl;
    cout_matrix(matrix);
    cout<< "combination"<<endl;
    QVector<int> pair;
    QVector<QVector<int>> vec_pair;
    for(int i = 0; i < matrix[0].size()-2; i++){
        for(int j = i+1; j < matrix[0].size()-1; j++){
            pair.push_back(i);
            pair.push_back(j);
            vec_pair.push_back(pair);
            pair.clear();
        }
    }
    for(int i = 0; i <vec_pair.size(); i++){
        qDebug()<<vec_pair[i];
    }
    QVector<QVector<int>> comb = combinations(vec_pair, 4);
    for(int i = 0; i <comb.size(); i++){
        qDebug()<<comb[i];
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QVector<QVector<fract>> matrix;
    read_matrix(matrix);
    cout_matrix(matrix);
    //    unit_matrix_forward(matrix, f_col);
    //    cout_matrix(matrix);
    //    mirror(matrix);
    //cout_matrix(matrix);
    //    unit_matrix_reverse(matrix);
    unit_matrix_forward_reverse(matrix);
    cout<<"ANSWER"<<endl;
    cout_matrix(matrix);
    basic_solutions(matrix);

    return a.exec();
}
