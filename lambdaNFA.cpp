#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<unordered_map>
#include<cstdlib>
#define MAX_VERTICES 10000 // numarul maxim de stari
#define SIGMA 300 // lungimea alfabetului
#define MAX_WORDS 1000 // numarul maxim de cuvinte testate
using namespace std;

vector<pair<int, char>>g[MAX_VERTICES];
string P[MAX_WORDS];
set<char> alphabet;
bool isFinal[MAX_VERTICES];
int n, m, start, k, l;
bool ok;

void readAutomata() {
    char ch;
    int x, y;
    cout << "Introduceti numarul de stari : ";
    cin >> n;
    cout << "Introduceti numarul de caractere din alfabet : ";
    cin >> m;
    cout << "Introduceti cele " << m << " caractere ale alfabetului : ";
    for(int i = 0; i < m; ++i) {
        cin >> ch;
        alphabet.insert(ch);
    }
    cout << "Starea initiala este : ";
    cin >> start;
    cout << "Numarul de stari finale este : ";
    cin >> k;
    cout << "Cele " << k << " stari finale sunt : ";
    for(int i = 0; i < k; ++i) {
        cin >> x;
        isFinal[x] = true;
    }
    cout << "Numarul de tranzitii este : ";
    cin >> l;
    cout << "Introduceti cele " << l << " tranzitii : ";
    for(int i = 0; i < l; ++i) {
        cin >> x >> ch >> y;
        if(x < 0 || x > n - 1 || y < 0 || y > n - 1) {
            cout << "Incercati sa introduceti o muchie invalida!!!\n";
            exit(1);
        }
        if(ch == '$' || alphabet.find(ch) != alphabet.end())
            g[x].push_back(make_pair(y, ch));
        else {
            cout << "Caracterul introdus nu exista in alfabet!!!\n";
            exit(1);
        }
    }
}

void DFS(int node, vector<int>&lambdaNodes) {
    vector<pair<int, char>>::iterator it;
    for(it = g[node].begin(); it != g[node].end(); ++it) {
        if((*it).second == '$') {
            lambdaNodes.push_back((*it).first);
            DFS((*it).first, lambdaNodes);
        }
    }
}

bool evaluate(string& word) {
    bool ok;
    int len = word.size();
    vector<int> states; // vectorul care retine starile curente
    states.push_back(start); // adaug in acest vector starea initiala ( nodul de start )
    vector<int>v;
    vector<int>lambdaNodes;
    /*
        Tratam cazul cand exista muchii etichetate cu caracterul $ care au o extremitate
        in nodul de start - retinem aceste stari in care putem ajunge in vectorul lambdaNodes
    */
    DFS(start, lambdaNodes);
    for(unsigned k = 0; k < lambdaNodes.size(); ++k)
        states.push_back(lambdaNodes[k]);
    for(int pos = 0; pos < len; ++pos) {
        for(unsigned i = 0; i < states.size(); ++i) { // trec prin toate starile
            for(unsigned j = 0; j < g[states[i]].size(); ++j)
                if(word[pos] == g[states[i]][j].second) // muchia este etichetata cu caracterul word[pos]
                    v.push_back(g[states[i]][j].first); // adaug nodul in vectorul care retine starile viitoare
        }
        // acum trebuie sa adaugam in vectorul v si starile in care ajungem cu lambda tranzitie
        // vom face o parcurgere in adancime care va merge doar pe muchii etichetate cu lambda
        // pornim parcurgerea din fiecare nod din vectorul v
        for(unsigned k = 0; k < v.size(); ++k)
            DFS(v[k], lambdaNodes);
        // copiez in vectorul v starile in care ajung cu lambda tranzitie
        for(unsigned k = 0; k < lambdaNodes.size(); ++k)
            v.push_back(lambdaNodes[k]);
        // actualizam starile curente
        states = v;
        v.clear();
        lambdaNodes.clear();
    }
    ok = false;
    for(unsigned i = 0; i < states.size() && !ok; ++i)
        if(isFinal[states[i]])
            ok = true;
    return ok;
}

int main() {
    unordered_map<bool, string> umap;
    umap[0] = "FALSE";
    umap[1] = "TRUE";
    int nrwords;
    readAutomata();
    cout << "Introduceti numarul de cuvinte : ";
    cin >> nrwords;
    cout << "Introduceti cele " << nrwords << " cuvinte :\n";
    for(int i = 0; i < nrwords; ++i)
        cin >> P[i];
    for(int i = 0; i < nrwords; ++i) {
        cout << P[i] << " " << umap[evaluate(P[i])] << "\n";
    }
    return 0;
}
