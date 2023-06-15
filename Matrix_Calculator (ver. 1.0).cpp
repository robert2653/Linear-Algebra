#include <bits/stdc++.h>
using namespace std;
const int MAX = 100;
using ll = long long int;
typedef vector<vector<pair<ll, ll>>> mat;
typedef pair<ll, ll> p;

p process(string tmps){
    stringstream ss; pair<int, int> p; string a, b;
    if(tmps.find('/') != string::npos){
        replace(tmps.begin(), tmps.end(), '/', ' ');
        ss << tmps;
        ss >> a; ss >> b;
    }
    else {
        a = tmps; b = "1";
    }
    bool a_has_point = a.find('.'), b_has_point = b.find('.');
    int x = a.size() - (a_has_point ? a.find('.') : a.size());
    int y = b.size() - (b_has_point ? b.find('.') : a.size());
    int m = max(x, y);
    int up = stod(a) * pow(10, m);
    int down = stod(b) * pow(10, m);
    return {up, down};
}
int find_gcd(int a, int b){
    if(b == 0){
        return a;
    }
    return find_gcd(b, a%b);
}
p reduction(p x){
    int gcd = find_gcd(abs(x.first), abs(x.second));
    if(gcd != 0){
        x.first /= gcd; x.second /= gcd;
    }
    if(x.second < 0) {
        x.first *= -1;
        x.second *= -1;
    }
    return x;
}
p fraction_puls(p a, p b, char op){
    p res;
    res.second = a.second * b.second;
    if(op == '+'){
        res.first = a.first * b.second + a.second * b.first;
    }
    else res.first = a.first * b.second - a.second * b.first;
    return reduction(res);
}
p fraction_mul(p a, p b){
    p res = {a.first * b.first, a.second * b.second};
    res = reduction(res);
    return res;
}
p find_det(mat a, int n) {
    p det = {0, 1};
    if (n == 1) {  // 矩陣只有一個元素，直接輸出
        return a[0][0];
    }
    if (n == 2) {  // 矩陣為2*2的情况，直接計算行列式
        p x, y;
        x = fraction_mul(a[0][0], a[1][1]);
        y = fraction_mul(a[0][1], a[1][0]);
        return fraction_puls(x, y, '-');
    }
    for (int j = 0; j < n; j++) {
        mat submat(n-1, vector<p>(n-1));
        for (int i = 1; i < n; i++) {
            int k = 0;
            for (int jj = 0; jj < n; jj++) {
                if (jj == j) continue;
                submat[i-1][k] = a[i][jj];
                k++;
            }
        }
        det = fraction_puls(det, fraction_mul(a[0][j], find_det(submat, n-1)), !(j&1) ? '+' : '-');
    }
    return det;
}
p make_cofactor(mat a, int i, int j, int size){
    for(int k = 0; k < size; k++){
        a[k].erase(a[k].begin()+j);
    }
    a.erase(a.begin()+i);
    bool pos = !(i&1)^(j&1);
    p res = find_det(a, size-1);
    if(!pos) res.first *= -1;
    return res;
}
void print_mat(mat a, p det, int m, int n){
    p res;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            res.first = a[i][j].first*det.second;
            res.second = a[i][j].second*det.first;
            res = reduction(res);
            if(res.first == 0) cout << 0 << "\t";
            else if(res.second == 1) cout << res.first << "\t";
            else cout << res.first << "/" << res.second << "\t";
        }
        cout << "\n";
    }
}
mat matrix_mul(mat a, mat b, int m1, int n1, int m2, int n2){
    mat res(m1, vector<p>(n2));
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n2; j++) {
            res[i][j] = {0, 1};
            for (int k = 0; k < n1; k++){
                res[i][j] = fraction_puls(res[i][j], fraction_mul(a[i][k], b[k][j]), '+');
            }
        }
    }
    return res;
}
void case_1(){
    int size;
    cout << "Enter dimensions of matrix A (size x size): ";
    cin >> size;
    cout << "Enter elements of matrix A:\n";
    string tmps;
    mat a(size, vector<p>(size));
    mat adj_a(size, vector<p>(size)); p det;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cin >> tmps;
            a[i][j] = process(tmps);
            tmps.clear();
        }
    }
    det = find_det(a, size);
    if(det.first == 0){
        cout << "The matrix is not invertible\n"; return;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            adj_a[j][i] = make_cofactor(a, i, j, size);
        }
    }
    cout << "------------\nadj= \n";
    print_mat(adj_a, {1,1}, size, size);
    cout << "------------\ndet= " << det.first;
    if(det.second != 1)
    cout << "/" << det.second;
    cout << "\n------------\ninv= \n";
    print_mat(adj_a, det, size, size);
    cout << "------------";
}
void case_2(){
    int m1, n1, m2, n2;
    cout << "Enter dimensions of matrix A (m1 x n1): ";
    cin >> m1 >> n1;
    cout << "Enter dimensions of matrix B (m2 x n2): ";
    cin >> m2 >> n2;
    if(n1 != m2){
        cout << "\nRun Error\n"; return;
    }
    string tmps;
    mat a(m1, vector<p>(n1));
    mat b(m2, vector<p>(n2));
    mat res;
    cout << "Enter elements of matrix A:\n";
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n1; j++) {
            cin >> tmps;
            a[i][j] = process(tmps);
            tmps.clear();
        }
    }
    cout << "Enter elements of matrix B:\n";
    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            cin >> tmps;
            b[i][j] = process(tmps);
            tmps.clear();
        }
    }
    res = matrix_mul(a, b, m1, n1, m2, n2);
    cout << "------------\nThe result is: \n";
    print_mat(res, {1,1}, m1, n2);
    cout << "------------";
}
void case_3(){
    int size; int m, n;
    cout << "Enter dimensions of matrix A (size x size): ";
    cin >> size;
    cout << "Enter dimensions of matrix B (m x n): ";
    cin >> m >> n;
    if(size != m){
        cout << "\nRun Error"; return;
    }
    string tmps;
    mat a(size, vector<p>(size));
    mat adj_a(size, vector<p>(size)); p det;
    mat b(m, vector<p>(n));

    cout << "Enter elements of matrix A:\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cin >> tmps;
            a[i][j] = process(tmps);
            tmps.clear();
        }
    }
    cout << "Enter elements of matrix B:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> tmps;
            b[i][j] = process(tmps);
            tmps.clear();
        }
    }
    det = find_det(a, size);
    if(det.first == 0){
        cout << "Matrix A is not invertible\n"; return;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            adj_a[j][i] = make_cofactor(a, i, j, size);
        }
    }
    mat res;
    res = matrix_mul(adj_a, b, size, size, m, n);
    cout << "------------\nx = \n";
    print_mat(res, det, m, n);
}
int main() {
    cout << "What do you want to do?\n1: Find inverse & determinant\n2: multiply matrices\n3: Solve Ax = B\nEnter a number: ";
    char op; cin >> op;
    switch (op){
    case '1':
        case_1();
        break;
    case '2':
        case_2();
        break;
    case '3':
        case_3();
        break;
    default:
        break;
    }
}
