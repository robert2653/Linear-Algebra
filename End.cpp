#include <bits/stdc++.h>
using namespace std;
using ll = long long int;
typedef vector<vector<pair<ll, ll>>> mat;
typedef pair<ll, ll> pll;
pll process_num(string tmps);
ll find_gcd(ll a, ll b);
pll reduction(pll x);
pll fraction_plus(pll a, pll b, char op);
pll fraction_mul(pll a, pll b);
pll find_det(mat a, int n);
pll make_cofactor(mat a, int i, int j, int size);
void print_mat(mat a);
mat matrix_mul(mat a, mat b);
bool check_is_matrix(mat matrix);
mat fast_me(mat a, int exp);
mat init_identity_matrix(int size);
mat find_inverse(mat a, pll det);

mat get_matrix(){
    //cin.ignore(numeric_limits <streamsize>::max(), '\n'); 總之就是清除輸入流，遇到問題可以下一行註解換這行
    cin.sync();
    string s; mat matrix; vector<pll> tmp_v;
    while(getline(cin, s)){
        if(s == ""){break;}
        stringstream ss; ss << s;
        string tmp_s;
        while(ss >> tmp_s){
            tmp_v.push_back(process_num(tmp_s));
        }
        matrix.push_back(tmp_v);
        tmp_v.clear();
    }
    return matrix;
}
bool check_is_matrix(mat matrix){
    int size = matrix[0].size();
    for(auto row: matrix){
        int now_size = row.size();
        if(size != now_size){
            cout << "You have entered an illegal matrix!\n";
            return 0;
        }
    }
    return 1;
}
pll process_num(string tmps){
    stringstream ss; pair<int, int> p; string a, b;
    if(tmps.find('/') != string::npos){
        replace(tmps.begin(), tmps.end(), '/', ' ');
        ss << tmps;
        ss >> a; ss >> b;
    }
    else {
        a = tmps; b = "1";
    }
    bool a_has_point = (a.find('.') != string::npos);
    bool b_has_point = (b.find('.') != string::npos);
    int x = a.size() - (a_has_point ? a.find('.')+1 : a.size());
    int y = b.size() - (b_has_point ? b.find('.')+1 : a.size());
    int m = max(x, y);
    int up = stod(a) * pow(10, m);
    int down = stod(b) * pow(10, m);
    return {up, down};
}
ll find_gcd(ll a, ll b){
    if(b == 0){
        return a;
    }
    return find_gcd(b, a%b);
}
pll reduction(pll x){
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
pll fraction_plus(pll a, pll b, char op){
    pll res;
    res.second = a.second * b.second;
    if(op == '+'){
        res.first = a.first * b.second + a.second * b.first;
    }
    else res.first = a.first * b.second - a.second * b.first;
    return reduction(res);
}
pll fraction_mul(pll a, pll b){
    pll res = {a.first * b.first, a.second * b.second};
    res = reduction(res);
    return res;
}
pll find_det(mat a, int n) {
    pll det = {0, 1};
    if (n == 1) {  // 矩陣只有一個元素，直接輸出
        return a[0][0];
    }
    if (n == 2) {  // 矩陣為2*2的情况，直接計算行列式
        pll x, y;
        x = fraction_mul(a[0][0], a[1][1]);
        y = fraction_mul(a[0][1], a[1][0]);
        return fraction_plus(x, y, '-');
    }
    for (int j = 0; j < n; j++) {
        mat submat(n-1, vector<pll>(n-1));
        for (int i = 1; i < n; i++) {
            int k = 0;
            for (int jj = 0; jj < n; jj++) {
                if (jj == j) continue;
                submat[i-1][k] = a[i][jj];
                k++;
            }
        }
        det = fraction_plus(det, fraction_mul(a[0][j], find_det(submat, n-1)), !(j&1) ? '+' : '-');
    }
    return det;
}
pll make_cofactor(mat a, int i, int j){
    int size = a.size();
    for(int k = 0; k < size; k++){
        a[k].erase(a[k].begin()+j);
    }
    a.erase(a.begin()+i);
    bool pos = !(i&1)^(j&1);
    pll res = find_det(a, size-1);
    if(!pos) res.first *= -1;
    return res;
}
void print_mat(mat a){
    int m = a.size(); int n = a[0].size();
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(a[i][j].first == 0) cout << 0 << "\t";
            else if(a[i][j].second == 1) cout << a[i][j].first << "\t";
            else cout << a[i][j].first << "/" << a[i][j].second << "\t";
        }
        cout << "\n";
    }
}
mat matrix_mul(mat a, mat b){
    int m1, n1, m2, n2;
    m1 = a.size(); n1 = a[0].size();
    m2 = b.size(); n2 = b[0].size();
    mat res(m1, vector<pll>(n2));
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n2; j++) {
            res[i][j] = {0, 1};
            for (int k = 0; k < n1; k++){
                res[i][j] = fraction_plus(res[i][j], fraction_mul(a[i][k], b[k][j]), '+');
            }
        }
    }
    return res;
}
mat init_identity_matrix(int size){
    mat a(size, vector<pll>(size, {0, 1}));
    for(int i = 0; i < size; i++){
        a[i][i] = {1, 1};
    }
    return a;
}
mat fast_me(mat a, int exp){
    mat ans = init_identity_matrix(a.size());
    while(exp > 0){
        if(exp & 1){
            ans = matrix_mul(ans, a);
        }
        a = matrix_mul(a, a);
        exp >>= 1;
    }
    return ans;
}
mat find_inverse(mat a, pll det){
    int size = a.size();
    mat adj_a(size, vector<pll>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            adj_a[j][i] = make_cofactor(a, i, j);
            adj_a[j][i].first *= det.second;
            adj_a[j][i].second *= det.first;
            adj_a[j][i] = reduction(adj_a[j][i]);
        }
    }
    return adj_a;
}
void case_1(){
    cout << "Enter matrix A (space line to stop):\n";
    mat a = get_matrix(); 
    if(!check_is_matrix(a)) return;
    int size = a.size();
    if(size != a[0].size()){
        cout << "The matrix is not invertible\n"; return;
    }
    pll det = find_det(a, size);
    if(det.first == 0){
        cout << "The matrix is not invertible\n"; return;
    }
    mat ans = find_inverse(a, det);
    cout << "------------\ndet= " << det.first;
    if(det.second != 1)
    cout << "/" << det.second;
    cout << "\n------------\ninv= \n";
    print_mat(ans);
}
void case_2(){
    int m1, n1, m2, n2;
    cout << "Enter matrix A (space line to stop):\n";
    mat a = get_matrix(); 
    if(!check_is_matrix(a)) return;
    m1 = a.size(); n1 = a[0].size();
    cout << "Enter matrix B (space line to stop):\n";
    mat b = get_matrix();
    if(!check_is_matrix(b)) return;
    m2 = b.size(); n2 = b[0].size();

    if(n1 != m2){
        cout << "Run Error\n"; return;
    }
    mat res = matrix_mul(a, b);
    cout << "------------\nThe result is: \n";
    print_mat(res);
}
void case_3(){
    int size; int m, n;
    cout << "Enter matrix A (space line to stop):\n";
    mat a = get_matrix();
    if(!check_is_matrix(a)) return;
    cout << "Enter matrix B (space line to stop):\n";
    mat b = get_matrix();
    if(!check_is_matrix(b)) return;
    size = a.size(); m = b.size(); n = b[0].size();
    if(size != a[0].size()){
        cout << "The matrix is not an square matrix\n"; return;
    }
    if(size != m || n > 1){
        cout << "Run Error\n"; return;
    }
    pll det = find_det(a, size);
    if(det.first == 0){
        cout << "Matrix A is not invertible\n"; return;
    }
    mat ans = find_inverse(a, det);
    ans = matrix_mul(ans, b);
    cout << "------------\nx = \n";
    print_mat(ans);
}
void case_4(){
    cout << "Enter matrix A (space line to stop):\n";
    mat a = get_matrix(); 
    if(!check_is_matrix(a)) return;
    int size = a.size();
    if(size != a[0].size()){
        cout << "The matrix is not an square matrix\n"; return;
    }
    cout << "Enter your exp: ";
    int exp; cin >> exp;
    if(exp >= 0){
        a = fast_me(a, exp);
    }
    else {
        a = fast_me(a, -exp);        
        pll det = find_det(a, size);
        if(det.first == 0){
        cout << "The matrix is not invertible\n"; return;
        }
        a = find_inverse(a, det);
    }
    cout << "------------\nAns = \n";
    print_mat(a);
}
int main() {
    char op;
    while(1){
        cout << "------------\nWhat would you like to do?\n";
        cout << "1: Find inverse & determinant\n";
        cout << "2: multiply matrices\n";
        cout << "3: Solve Ax = B\n";
        cout << "4: Solve A^n\n";
        cout << "Enter a number (other key to exit): ";
        cin >> op;
        if(op == '1') case_1();
        else if(op == '2') case_2();
        else if(op == '3') case_3();
        else if(op == '4') case_4();
        else break;
        //cin.ignore(numeric_limits <streamsize>::max(), '\n'); 總之就是清除輸入流，遇到問題可以下一行註解換這行
        cin.sync();
        cout << "Press enter to continue...";
        getchar();
    }
}
