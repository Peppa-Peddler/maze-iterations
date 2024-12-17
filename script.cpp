#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define N 100

using namespace std;

int n;



//    2
// 1 [ ] 3 = (1)(2)(3)(4)
//    4                  2

bool L(int MASK) { return !!(MASK & (1 << 3)); }
bool T(int MASK) { return !!(MASK & (1 << 2)); }
bool R(int MASK) { return !!(MASK & (1 << 1)); }
bool B(int MASK) { return !!(MASK & (1 << 0)); }

int setL(int MASK) { return (MASK | (1 << 3)); }
int setT(int MASK) { return (MASK | (1 << 2)); }
int setR(int MASK) { return (MASK | (1 << 1)); }
int setB(int MASK) { return (MASK | (1 << 0)); }

bool random_dfs(int i, int j, vector<vector<int> >& state, int branches, int deep = 0, bool start = false);
void newBranch(vector<vector<int> >& state, int branches);
vector<int> sources;
vector<vector<int> > C;
int V[N][N];

void show(vector<vector<int> > &state){
    for(int j = 0; j < n; j++){
        if( j == 0 ) cout << " ";
        int ST = state[ 0 ][ j ];
        if( T(ST) ) cout << "  ";
        else cout << "_ ";
    } cout << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int ST = state[ i ][ j ];
            if( j == 0 ){
                if(L(ST)) cout << " ";
                else cout << "|";
            }
            if( B(ST) ) cout << " ";
            else cout << "_";
            if( R(ST) ) cout << " ";
            else cout << "|";
        }
        cout << endl;
    }
}

void showCSV(vector<vector<int> > &state){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(j){
                cout<<",";
            }
            cout<<state[ i ][ j ];
        }
        cout << endl;
    }
}

void newBranch(vector<vector<int> >& state, int branches){
    vector< pair<int, int> > roots;
    random_device rd;
	mt19937 g(rd());
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(state[i][j])
                roots.push_back({i, j});
        }
    }
    shuffle(roots.begin(), roots.end(), g);
    roots.resize(min(10, int(roots.size())));
    for( auto root: roots ){
		int i = root.first;
		int j = root.second;
        bool path = random_dfs(i, j, state, branches);
        if(path){
            //cout << i << " " << j << endl;
            return;
        }
    }
}

bool end(vector<vector<int> >& state, int branches){
    //show(state);
    //cout << "BRANCHES: " << branches << " LEFT" << endl;
    C = state;
    if(branches > 0) newBranch(state, branches - 1);
    return true;
}

bool random_dfs(int i, int j, vector<vector<int> >& state, int branches, int deep, bool start){
    
    j = (j + n)%n;

    //cout << i << " " << j << endl;
    if( i == n - 1 ) {
        return end(state, branches);
    }
    
    random_device rd;
	mt19937 g(rd());

	vector< pair<int, int> > moves{{0, 1}, {0, -1}};

    if( deep > n/3 or start or deep%10!=0 ){
        moves.push_back({1,0});
        moves.push_back({1,0});
        moves.push_back({1,0});
        moves.push_back({1,0});
        moves.push_back({-1,0});
    } else {
        moves.push_back({0,1});
        moves.push_back({0,1});
        moves.push_back({0,1});
        moves.push_back({0,-1});
        moves.push_back({0,-1});
        moves.push_back({0,-1});
    }

	shuffle( moves.begin(), moves.end() , g);


    int ii, jj;
    bool path = false;


    //for( auto move: moves )
    //   cout << move.first << " " << move.second << endl;

    for( auto move: moves ){
		ii = move.first;
		jj = move.second;
        if( i + ii < 0 ) continue;
        
        vector<vector<int> > newState = state;
        
        bool isEnd = newState[i+ii][(j+jj+n)%n];
        bool isPrev = C[i+ii][(j+jj+n)%n];

        if( ii ) {
            if( ii == 1 ){
                if( B(newState[i][j]) ) continue;
                //cout << "B" << endl;
                newState[i][j] = setB(newState[i][j]);
                newState[i+ii][j] = setT(newState[i+ii][j]);
            } else {
                if( T(newState[i][j]) ) continue;
                //cout << "T" << endl;
                newState[i][j] = setT(newState[i][j]);
                newState[i+ii][j] = setB(newState[i+ii][j]);
            }
        }

        if( jj ){
            if( jj == 1 ){
                if( R(newState[i][j]) ) continue;
                //cout << "R" << endl;
                newState[i][j] = setR(newState[i][j]);
                newState[i][(j+jj+n)%n] = setL(newState[i][(j+jj+n)%n]);  
            } else {
                if( L(newState[i][j]) ) continue;
                //cout << "L" << endl;
                newState[i][j] = setL(newState[i][j]);
                newState[i][(j+jj+n)%n] = setR(newState[i][(j+jj+n)%n]);
            }
 
        }
		if( !isEnd or (i + ii == n - 1) ) path = path || random_dfs( i + ii, j + jj, newState, branches, deep + 1);
        if( isPrev ) return end(newState, branches);
        if( path ) break;
    }

    return path;
}

void checkDFS(int i,int j){
    if( V[i][j] == 2 ) return;
    V[i][j] = 2;
    if( i + 1 < n && B(C[i][j]) )
        checkDFS(i+1, j);
    if( i - 1 >= 0 && T(C[i][j]) )
        checkDFS(i-1, j);
    if( R(C[i][j]) )
        checkDFS(i,(j+1)%n);
    if( L(C[i][j]) )
        checkDFS(i,(j-1+n)%n);
}

void newPaths(int i, int j){
    if( V[i][j] ) return;
    V[i][j] = 1;
    if( !C[i][(j+1)%n] ){
        C[i][j] = setR(C[i][j]);
        C[i][(j+1)%n] = setL(C[i][(j+1)%n]);
        return newPaths(i,(j+1)%n);
    }
    if( !C[i][(j-1+n)%n] ){
        C[i][j] = setL(C[i][j]);
        C[i][(j-1+n)%n] = setR(C[i][(j-1+n)%n]);
        return newPaths(i,(j-1+n)%n);
    }
    if( i + 1 < n && !C[i+1][j] ){
        C[i][j] = setB(C[i][j]);
        C[i+1][j] = setT(C[i+1][j]);
        return newPaths(i + 1, j);
    }

    if( !B(C[i][j]) && i+1<n && V[i+1][j] == 2 ){
        C[i][j] = setB(C[i][j]);
        C[i+1][j] = setT(C[i+1][j]);
    }
    if( !T(C[i][j]) && i-1>=0 && V[i-1][j] == 2 ){
        C[i][j] = setT(C[i][j]);
        C[i-1][j] = setB(C[i-1][j]);
    }
    if( !R(C[i][j]) && V[i][(j+1)%n] == 2 ){
        C[i][j] = setR(C[i][j]);
        C[i][(j+1)%n] = setL(C[i][(j+1)%n]);
    }
    if( !L(C[i][j]) && V[i][(j-1+n)%n] == 2 ){
        C[i][j] = setL(C[i][j]);
        C[i][(j-1+n)%n] = setR(C[i][(j-1+n)%n]);    
    }

}

vector<int> getRandomNumbers(int m) {
    std::vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        numbers[i] = i;
    }

    // Shuffle the vector
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(numbers.begin(), numbers.end(), gen);

    // Take the first 'm' numbers
    numbers.resize(m);
    return numbers;
}


int main(){
    
    n = 10;
    cin >> n;

    int m = n/5;

    vector<vector<int> >START(n, vector<int>(n, 0));
    
    C = START;

    sources = getRandomNumbers(m);
    
    for(int i = 0; i < n; i++){
        C[n - 1][ i ] = 0b1010;
    }


    for(int i = 0; i < m; i++){
        C[0][ sources[i] ] = 0b0100;
    } 

    for(int i = 0; i < m; i++){
        //cout << "NEW SOURCE AT: 0 " << sources[i] << " (" << m - i - 1 <<" LEFT)"<< endl;
        random_dfs(0, sources[i], C, n/3, 0 ,true);
    }

    //showCSV(C);    
    //show(C);

    checkDFS(n-1, 0);

    for(int i = 0; i < n - 1; i++){
        for(int j = 0; j < n; j++){
            if( C[i][j] and !C[i][(j+1)%n] ){
                C[i][j] = setR(C[i][j]);
                C[i][(j+1)%n] = setL(C[i][(j+1)%n]);
                newPaths(i, (j+1)%n);
                checkDFS(i, (j+1)%n);
            }
        }
    }

    //show(C);
    showCSV(C);

    /*for(int i = 0; i < n;i++){
        for(int j = 0; j < n; j++){
            cout<<V[i][j];
        }cout<<endl;
    }*/

    return 0;
}