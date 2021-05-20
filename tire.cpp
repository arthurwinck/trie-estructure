#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>

using namespace std;
struct Trie {
    struct NoTrie {
        char           letra;        //opcional
        NoTrie        *filhos[26];   //pode ser uma 'LinkedList' de ponteiros
        unsigned long  posicao;      //posicao absoluta da palavra em colchetes no arquivo     
        unsigned long  comprimento;  //se maior que zero, indica último caracter de uma palavra

        
        // Função de criar um novo no para a árvore (Não é chamado pelo main())

        struct Trie::NoTrie *criarNo(int index) {
            struct Trie::NoTrie *novo_no = new Trie::NoTrie;
            novo_no->posicao = 0;
            novo_no->comprimento = 0;
            novo_no->letra = index + 97;

            for (int i = 0; i < 26; i++) {
                novo_no->filhos[i] == NULL;
            }

            return novo_no;

        }
        
        // Função de inserir um nó na árvore (esse método chama criarNó)
        
        void inserir(struct Trie::NoTrie *raiz, string chave , int comprimento, int posicao) {
            struct Trie::NoTrie *ptr = raiz;
            cout << chave << endl;

            // Itera sobre a string, ex: (a,c,h,a,r)
            for (int i = 0; i < chave.length(); i++) {
                // a - a = 0; a - b = 1....
                int index = chave[i] - 'a';
                // Se o nó dessa letra não existe eu crio um novo para ele
                if (ptr->filhos[index] == NULL) {
                    //cout << chave[i] << endl;
                    ptr->filhos[index] = criarNo(index);
                    //ptr->filhos[i] = getNode(i);
                }

                //Se ele existe eu só passo para o próximo
                ptr = ptr->filhos[index];
            }

            /*
            Recebendo o comprimento da linha de que foi retirado e a posicao absoluta da palavra
            no arquivo
            */
            ptr->comprimento = comprimento;
            ptr->posicao = posicao;

        }
        
        Trie::NoTrie* search(struct Trie::NoTrie *raiz, string chave) {
            struct Trie::NoTrie *ptr = raiz;

            // Itera sobre a string, ex: (a,c,h,a,r)
            for (int i = 0; i < chave.length(); i++) {
                // a - a = 0; a - b = 1....
                int index = chave[i] - 'a';
                // Se o nodo dessa letra não existe então a palavra não existe
                if (!ptr->filhos[index]) {
                    return nullptr;
                }

                ptr = ptr->filhos[index];
            }
            //cout << ptr->letra << endl;
            
            if (ptr->comprimento) {
                return ptr;       
            } else {
                return nullptr;
            }

        }

        void search_iterate(struct Trie::NoTrie *raiz, int* num_prefix) {
            struct Trie::NoTrie *ptr = raiz;
            
            if (ptr != NULL) {
                if (ptr->comprimento) {
                    *num_prefix = *num_prefix + 1;
                }

                for (int i = 0; i < 26; i ++) {
                    if (ptr->filhos[i] != NULL) {
                        search_iterate(ptr->filhos[i], num_prefix);
                    }
                }

            }
            return;
        }

        void search_prefix(struct Trie::NoTrie *raiz, string chave, int* num_prefix) {
            struct Trie::NoTrie *ptr = raiz;
            int status = 0;

            // Itera sobre a string, ex: (a,c,h,a,r)
            for (int i = 0; i < chave.length(); i++) {
                // a - a = 0; a - b = 1....
                int index = chave[i] - 'a';
                // Se o nodo dessa letra não existe então a palavra não existe
                if (ptr->filhos[index] != NULL) {
                    ptr = ptr->filhos[index];
                    status ++;
                }
            }

            if (status != chave.length()){
                *num_prefix = 0; 
                return;
            }

            search_iterate(ptr, num_prefix);
            return;

        }
    };

    Trie() {
        struct Trie::NoTrie *novo_no = new Trie::NoTrie;
        novo_no->posicao = 0;
        novo_no->comprimento = 0;
            
        for (int i = 0; i < 26; i++) {
            novo_no->filhos[i] == NULL;
        }
        
        raiz = novo_no;
    }

    NoTrie* raiz;
};


int main() {
    
    string filename;
    string word;

    cin >> filename;  // entrada
    

    //cout << filename << endl;  // esta linha deve ser removida

    vector<string> vetor_palavras;

    while (1) {  // leitura das palavras ate' encontrar "0"
        cin >> word;
        
        if (word.compare("0") == 0) {
            break;
        }
        vetor_palavras.push_back(word);
        //cout << word << endl;
    }

    int tam_lista = vetor_palavras.size();
    string lista_palavras[tam_lista];

    for (int i = 0; i < tam_lista; i++) {
        lista_palavras[i] = vetor_palavras.at(i);
    }

    // Temos nossa lista de palavras, agora hora de iterar sobre o nosso dicionário!
    ifstream dic;
    dic.open(filename);

    //Hora de criarmos a nossa árvore também (por enquanto somente a raiz)
    Trie* arvore = new Trie();


    if (dic.is_open()){
        string linha;
        double posicao = 0;
        double comprimento = 0;        

        // Processo de inserção de todas as entradas no dicionário
        while (getline(dic, linha)) {
            string tag;
            int pos_na_linha;

            // Aqui pegamos as tags do dicionário para compará-las as entradas 
            for (int i = 0; i < linha.length(); i ++) {
                if (linha[i] == '[') {
                    pos_na_linha = i;
                }

                tag += linha[i];

                if (linha[i] == ']') {
                    break;
                }

            }
            comprimento = linha.length();
            arvore->raiz->inserir(arvore->raiz, tag.substr(1,tag.length()-2), comprimento, posicao);
            posicao = posicao + 1;
            //Passamos para a linha de baixo e com isso incrementamos a nossa var posicao
            posicao += comprimento;
            
        }
    }

    
    // Procurar pelas entradas pedidas
    for (int i = 0; i < tam_lista; i++ ){
        Trie::NoTrie* ptr = arvore->raiz->search(arvore->raiz, lista_palavras[i]);
        int num_prefix = 0;
        arvore->raiz->search_prefix(arvore->raiz, lista_palavras[i], &num_prefix); 
        
        if (num_prefix) {
            cout << lista_palavras[i] + " is prefix of " + to_string(num_prefix) + " words" << endl;
        } else {
            cout << lista_palavras[i] + " is not prefix" << endl;
        }
        
        if (ptr != nullptr) {
            cout << lista_palavras[i] + " is at (" + to_string(ptr->posicao) + "," + to_string(ptr->comprimento) + ")" << endl;
        }
    }
    


    return 0;
}